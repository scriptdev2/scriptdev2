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
SDName: Boss_Watchkeeper_Gargolmar
SD%Complete: 70
SDComment: Missing adds to heal him
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "sc_creature.h"

//Adds NYI

#define SPELL_MORTAL_WOUND        30641
#define SPELL_SURGE                25787
#define SPELL_RETALIATION        40546

#define SAY_TAUNT                "Do you smell that? Fresh meat has somehow breached our citadel. Be wary of any intruders." 
#define SOUND_TAUNT                

#define SAY_AGGRO_1                "Heh... this may hurt a little."
#define SOUND_AGGRO_1            10332
#define SAY_AGGRO_2                "What do we have here? ..." 
#define SOUND_AGGRO_2            10331
#define SAY_AGGRO_3                "" 
#define SOUND_AGGRO_3            10333


#define SAY_SURGE                "Back off, pup!" 
#define SOUND_SURGE                10330    

#define SAY_HEAL                "Heal me! QUICKLY!" 
#define SOUND_HEAL                10329

#define SOUND_DIE                10336

struct MANGOS_DLL_DECL boss_watchkeeper_gargolmarAI : public ScriptedAI
{
    boss_watchkeeper_gargolmarAI(Creature *c) : ScriptedAI(c) {Reset();}   

    uint32 Surge_Timer;
    uint32 MortalWound_Timer;
    uint32 Retaliation_Timer;

    bool HasTaunted;
    bool YelledForHeal;

    void Reset()
    {   
        Surge_Timer = 5000;
        MortalWound_Timer = 4000;
        Retaliation_Timer = 5000;

        HasTaunted = false;
        YelledForHeal = false;

    }

    void Aggro(Unit *who)
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
                }
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {
                DoYell(SAY_TAUNT, LANG_UNIVERSAL, NULL);

                HasTaunted = true;
            }

        }
    }  

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(MortalWound_Timer < diff)
        {                   
            DoCast(m_creature->getVictim(),SPELL_MORTAL_WOUND);

            MortalWound_Timer = 5000+rand()%8000;
        }else MortalWound_Timer -= diff;

        if(Surge_Timer < diff)
        {
            DoYell(SAY_SURGE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SURGE);

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            DoCast(target,SPELL_SURGE);

            Surge_Timer = 5000+rand()%8000;
        }else Surge_Timer -= diff;

        if((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 20)
        {
            if(Retaliation_Timer < diff)
            {              
                DoCast(m_creature->getVictim(),SPELL_RETALIATION);
                Retaliation_Timer = 5000;
            }else Retaliation_Timer -= diff;
        }

        if(!YelledForHeal)
            if((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 40)
            {
                DoYell(SAY_HEAL, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_HEAL);

                //m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth()*0.3);
                YelledForHeal = true;        
            }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_watchkeeper_gargolmarAI(Creature *_Creature)
{
    return new boss_watchkeeper_gargolmarAI (_Creature);
}


void AddSC_boss_watchkeeper_gargolmar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_watchkeeper_gargolmar";
    newscript->GetAI = GetAI_boss_watchkeeper_gargolmarAI;
    m_scripts[nrscripts++] = newscript;
}
