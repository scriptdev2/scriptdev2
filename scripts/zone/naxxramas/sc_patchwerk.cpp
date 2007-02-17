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

#include "../../sc_defines.h"

#define SAY_AGGRO1      "Patchwerk want to play!"
#define SAY_AGGRO2      "Kel'Thuzad make Patchwerk his Avatar of War!"
#define SAY_SLAY        "No more play?"
#define SAY_DEATH       "What happened to... Patch..."

#define SOUND_AGGRO1    8909
#define SOUND_AGGRO2    8910
#define SOUND_SLAY      8912
#define SOUND_DEATH     8911

#define SPELL_HATEFULSTRIKE     28308
#define SPELL_ENRAGE            29691
#define SPELL_BERSERK           27680
#define SPELL_SLIMEBOLT         32309

struct MANGOS_DLL_DECL boss_patchwerkAI : public ScriptedAI
{
    boss_patchwerkAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 HatefullStrike_Timer;
    uint32 Enrage_Timer;
    uint32 Slimebolt_Timer;
    bool Enraged;
    bool InCombat;

    void Reset()
    {
        HatefullStrike_Timer = 1200;        //1.2 seconds
        Enrage_Timer = 60000;               //7 minutes 420,000
        Slimebolt_Timer = 90000;            //7.5 minutes 450,000
        Enraged = false;
        InCombat = false;

        if (m_creature)
            EnterEvadeMode();
    }

    void KilledUnit(Unit* Victim)
    {
        if (rand()%5)
            return;

        DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                if (rand()%2)
                {
                    DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                }
                else
                {
                    DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
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
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    if (rand()%2)
                    {
                        DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                    }
                    else
                    {
                        DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                    }
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the player died from some unknown soruce
        //But we still need to reset
        if (InCombat && !m_creature->SelectHostilTarget())
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            
            //HatefullStrike_Timer
            if (HatefullStrike_Timer < diff)
            {
                //Cast Hateful strike on the player with the highest
                //amount of HP within melee distance
                //Currently target selection not supported by the core
                DoCast(m_creature->getVictim(),SPELL_HATEFULSTRIKE);

                //1.2 seconds until we should cast this agian
                HatefullStrike_Timer = 1200;
            }else HatefullStrike_Timer -= diff;

            //Enrage_Timer
            if (Enrage_Timer < diff)
            {
                //Cast Berserker Rage
                DoCast(m_creature, SPELL_BERSERK);

                //5 minutes until we should cast this agian
                Enrage_Timer = 300000;
            }else Enrage_Timer -= diff;

            //Slimebolt_Timer
            if (Slimebolt_Timer < diff)
            {
                //Cast Slime bolt
                DoCast(m_creature->getVictim(),SPELL_SLIMEBOLT);

                //5 seconds until we should cast this agian
                Slimebolt_Timer = 5000;
            }else Slimebolt_Timer -= diff;

            //Enrage if not already enraged and below 5%
            if (!Enraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 5)
            {
                DoCast(m_creature,SPELL_ENRAGE);
                Enraged = true;
            }

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};
CreatureAI* GetAI_boss_patchwerk(Creature *_Creature)
{
    return new boss_patchwerkAI (_Creature);
}


void AddSC_boss_patchwerk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_patchwerk";
    newscript->GetAI = GetAI_boss_patchwerk;
    m_scripts[nrscripts++] = newscript;
}