/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "../../../sc_defines.h"

#define SPELL_SHADOW_BOLT_VOLLEY        29924
#define SPELL_BURNING_NOVA                30940
#define SPELL_FIRE_NOVA                    30941 // ?????
#define SPELL_CORRUPTION                30938


#define SAY_KILL_1                "Just as you deserve!" 
#define SOUND_KILL_1            10169
#define SAY_KILL_2                "Your friends will soon be joining you."
#define SOUND_KILL_2            10170        

#define SAY_AGGRO_1                "You mustn't let him loose!"
#define SOUND_AGGRO_1            10166
#define SAY_AGGRO_2                "Ignorant whelps!" 
#define SOUND_AGGRO_2            10167
#define SAY_AGGRO_3                "You fools! He'll kill us all!"    
#define SOUND_AGGRO_3            10168

#define SAY_NOVA                "Closer... Come closer.. and burn!"
#define SOUND_BURN                10165

#define SAY_DIE                    "Good luck... you'll need it.." 
#define SOUND_DIE                10171


// 10164,10 "A_HELL_Keli_Wake01"

struct MANGOS_DLL_DECL boss_kelidan_the_breakerAI : public ScriptedAI
{
    boss_kelidan_the_breakerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}   

    uint32 ShadowVolley_Timer;
    uint32 BurningNova_Timer;
    uint32 Firenova_Timer;
    uint32 Corrupion_Timer;
    bool Firenova;

    bool InCombat;

    void EnterEvadeMode()
    {   
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);

        ShadowVolley_Timer = 1000;
        BurningNova_Timer = 10000;
        Corrupion_Timer = 5000;
        Firenova = false;


        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                switch(rand()%3)
                {
                case 0:
                    DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                    break;

                case 1:
                    DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                    break;  

                case 2:
                    DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                    break;  
                }
                InCombat = true;
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_1);
            break;

        case 1:
            DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_2);
            break;               
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL); 
        DoPlaySoundToSet(m_creature,SOUND_DIE);
    }

    void MoveInLineOfSight(Unit *who)
    {  
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH); 
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    switch(rand()%3)
                    {
                    case 0:
                        DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                        break;

                    case 1:
                        DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                        break;  

                    case 2:
                        DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                        break;
                    }
                    InCombat = true;
                }
            }
        }
    }  

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!Firenova)
        {
            if(ShadowVolley_Timer < diff)
            {
                Unit* target;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_SHADOW_BOLT_VOLLEY);

                ShadowVolley_Timer = 4000+rand()%8000;
            }else ShadowVolley_Timer -=diff;

            if(BurningNova_Timer < diff)
            {
                DoYell(SAY_NOVA, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_BURN);

                DoCast(m_creature,SPELL_BURNING_NOVA);    

                BurningNova_Timer = 10000+rand()%8000;
                Firenova_Timer= 5000;
                Firenova = true;
            }else BurningNova_Timer -=diff;

            if(Corrupion_Timer < diff)
            {
                Unit* target;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_CORRUPTION);

                Corrupion_Timer = 4000+rand()%8000;
            }else Corrupion_Timer -=diff;

            DoMeleeAttackIfReady();
        }

        if(Firenova)
            if(Firenova_Timer < diff)
            {
                DoCast(m_creature,SPELL_FIRE_NOVA);
                Firenova = false;
            }else Firenova_Timer -=diff;
    }

};

CreatureAI* GetAI_boss_kelidan_the_breakerAI(Creature *_Creature)
{
    return new boss_kelidan_the_breakerAI (_Creature);
}


void AddSC_boss_kelidan_the_breaker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kelidan_the_breaker";
    newscript->GetAI = GetAI_boss_kelidan_the_breakerAI;
    m_scripts[nrscripts++] = newscript;
}
