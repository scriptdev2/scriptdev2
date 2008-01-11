/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Omar_The_Unscarred
SD%Complete: 70
SDComment: Missing MT air knockback and float
EndScriptData */

#include "../../../sc_defines.h"

#define SPELL_SHADOW_WHIP        30638
#define SPELL_TREACHEROUS_AURA    30695
#define SPELL_REFLECT            23920
#define SPELL_ORBITAL_STRIKE     30637

#define SAY_AGGRO_1                "You dare challenge me?!" 
#define SOUND_AGGRO_1            10280
#define SAY_AGGRO_2                "I will not be defeated!"
#define SOUND_AGGRO_2            10279
#define SAY_AGGRO_3             ""
#define SOUND_AGGRO_3            10281


#define SAY_SUMMON                "Achor-she-ki! Feast my pet! Eat your fill!"
#define SOUND_SUMMON            10277

#define SAY_CURSE                "A-Kreesh!"    
#define SOUND_CURSE                10278

#define SAY_KILL_1                "Die, weakling!"
#define SOUND_KILL_1            10282            

#define SAY_DIE                    "It is... not over." 
#define SOUND_DIE                10284

#define SAY_WIPE                "I am victorious!"     
#define SOUND_WIPE                10283    


#define ADD_X1            -1133.756
#define ADD_Y1             1720.647
#define ADD_Z1             89.671
#define ADD_X2          -1125.418
#define ADD_Y2           1704.123
#define ADD_Z2             89.867


struct MANGOS_DLL_DECL boss_omor_the_unscarredAI : public ScriptedAI
{
    boss_omor_the_unscarredAI(Creature *c) : ScriptedAI(c) {InCombat = false; EnterEvadeMode();}   

    uint32 ShadowWhip_Timer;
    uint32 TreacherousAura_Timer;
    uint32 Reflect_Timer;
    uint32 Summon_Timer;
    uint32 OrbitalStrike_Timer;
    bool Summoned;

    bool InCombat;

    void EnterEvadeMode()
    {   
        if (InCombat)
        {
            DoYell(SAY_WIPE,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_WIPE);
        }

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        InCombat = false;
        ShadowWhip_Timer = 1000;
        Reflect_Timer = 1000;
        TreacherousAura_Timer = 2000;
        Summon_Timer = 10000;
        OrbitalStrike_Timer = 5000;
        Summoned = false;
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL); 
        DoPlaySoundToSet(m_creature,SOUND_DIE);
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%2)
            return;

        DoYell(SAY_KILL_1,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_KILL_1);
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
                switch(rand()%2)
                {
                case 0:
                    DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                    break;

                case 1:
                    DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                    break;                    
                }
                InCombat = true;
            }
        }
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
                    switch(rand()%2)
                    {
                    case 0:
                        DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                        break;

                    case 1:
                        DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
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

        if(ShadowWhip_Timer < diff)
        {   
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_SHADOW_WHIP);

            ShadowWhip_Timer = 2000+rand()%8000;
        }else ShadowWhip_Timer -= diff;

        if((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 20)
        {
            if(Reflect_Timer < diff)
            {              
                DoCast(m_creature,SPELL_REFLECT);

                Reflect_Timer = 5000;
            }else Reflect_Timer -= diff;
        }

        if(TreacherousAura_Timer < diff)
        {   
            DoYell(SAY_CURSE,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_CURSE);

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            DoCast(target,SPELL_TREACHEROUS_AURA);

            TreacherousAura_Timer = 5000+rand()%8000;
        }else TreacherousAura_Timer -= diff;

        if(OrbitalStrike_Timer < diff)
        {   
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
            {
                DoCast(target,SPELL_ORBITAL_STRIKE);
                OrbitalStrike_Timer = 15000+rand()%5000;
            }
        }else TreacherousAura_Timer -= diff;

        if(!Summoned)
            if (Summon_Timer < diff)
            {
                DoYell(SAY_SUMMON,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_SUMMON);

                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                Creature* pSummoned = NULL;

                //Summoned = m_creature->SummonCreature(17540,ADD_X1,ADD_Y1,ADD_Z1,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,900000);
                pSummoned = DoSpawnCreature(17540, 0,0,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);
                if (pSummoned && target)
                    pSummoned->AI()->AttackStart(target);

                pSummoned = DoSpawnCreature(17540, 0,0,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);
                if (pSummoned && target)
                    pSummoned->AI()->AttackStart(target);

                Summoned = true;

            }else Summon_Timer -= diff;
    }   
};

CreatureAI* GetAI_boss_omor_the_unscarredAI(Creature *_Creature)
{
    return new boss_omor_the_unscarredAI (_Creature);
}


void AddSC_boss_omor_the_unscarred()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_omor_the_unscarred";
    newscript->GetAI = GetAI_boss_omor_the_unscarredAI;
    m_scripts[nrscripts++] = newscript;
}
