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

// **** This script is still under Developement ****
// Adds NYI and the intro dialog is NYI

#define SPELL_HANDOFRAGNAROS        19780
#define SPELL_WRATHOFRAGNAROS       20566
#define SPELL_LAVABURST             21158

#define SPELL_MAGMABURST            20565       //Ranged attack

#define SPELL_SONSOFFLAME_DUMMY     21108       //Server side effect
#define SPELL_RAGSUBMERGE           21107       //Stealth aura

#define SAY_ARRIVAL_1       "TOO SOON! YOU HAVE AWAKENED ME TOO SOON, EXECUTUS! WHAT IS THE MEANING OF THIS INTRUSION?"
#define SAY_ARRIVAL_3       "FOOL! YOU ALLOWED THESE INSECTS TO RUN RAMPANT THROUGH THE HALLOWED CORE, AND NOW YOU LEAD THEM TO MY VERY LAIR? YOU HAVE FAILED ME, EXECUTUS! JUSTICE SHALL BE MET, INDEED!"
#define SAY_ARRIVAL_5       "NOW FOR YOU, INSECTS. BOLDLY YOU SAUGHT THE POWER OF RAGNAROS NOW YOU SHALL SEE IT FIRST HAND."
#define SAY_REINFORCEMENTS1 "COME FORTH, MY SERVANTS! DEFEND YOUR MASTER!"
#define SAY_REINFORCEMENTS2 "YOU CANNOT DEFEAT THE LIVING FLAME! COME YOU MINIONS OF FIRE! COME FORTH YOU CREATURES OF HATE! YOUR MASTER CALLS!"
#define SAY_HAND            "BY FIRE BE PURGED!"
#define SAY_WRATH           "TASTE THE FLAMES OF SULFURON!"
#define SAY_KILL            "DIE INSECT!"
#define SAY_MAGMABURST      "MY PATIENCE IS DWINDILING! COME NATS TO YOUR DEATH!"

#define SOUND_ARRIVAL_1         8043
#define SOUND_ARRIVAL_3         8044
#define SOUND_ARRIVAL_5         8045
#define SOUND_REINFORCEMENTS1   8049
#define SOUND_REINFORCEMENTS2   8050
#define SOUND_HAND              8046
#define SOUND_WRATH             8047
#define SOUND_KILL              8051
#define SOUND_MAGMABURST        8048

struct MANGOS_DLL_DECL boss_ragnarosAI : public ScriptedAI
{
    boss_ragnarosAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 WrathOfRagnaros_Timer;
    uint32 HandOfRagnaros_Timer;
    uint32 LavaBurst_Timer;
    uint32 MagmaBurst_Timer;
    uint32 Submerge_Timer;
    bool HasYelledMagmaBurst;
    bool HasSubmergedOnce;
    bool InCombat;

    void EnterEvadeMode()
    {
        WrathOfRagnaros_Timer = 30000;
        HandOfRagnaros_Timer = 25000;
        LavaBurst_Timer = 10000;
        MagmaBurst_Timer = 12000;
        Submerge_Timer = 180000;
        HasYelledMagmaBurst = false;
        HasSubmergedOnce = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        DoYell(SAY_KILL, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_KILL);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin ranged attack because Ragnaros is rooted at all times
            DoStartRangedAttack(who);

            //Say our dialog on initial aggro
            if (!InCombat)
            {
                DoYell(SAY_ARRIVAL_5,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_ARRIVAL_5);
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

                //Begin ranged attack because Ragnaros is rooted at all times
                DoStartRangedAttack(who);

                //Say our dialog on initial aggro
                if (!InCombat)
                {
                    DoYell(SAY_ARRIVAL_5,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_ARRIVAL_5);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //WrathOfRagnaros_Timer
            if (WrathOfRagnaros_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_WRATHOFRAGNAROS);
                
                if (rand()%2 == 0)
                {
                    //Say our dialog
                    DoYell(SAY_WRATH,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_WRATH);
                }

                //60 seconds until we should cast this agian
                WrathOfRagnaros_Timer = 30000;
            }else WrathOfRagnaros_Timer -= diff;

            //HandOfRagnaros_Timer
            if (HandOfRagnaros_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_HANDOFRAGNAROS);

                if (rand()%2==0)
                {
                    //Say our dialog
                    DoYell(SAY_HAND,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_HAND);
                }

                //60 seconds until we should cast this agian
                HandOfRagnaros_Timer = 25000;
            }else HandOfRagnaros_Timer -= diff;

            //LavaBurst_Timer
            if (LavaBurst_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_LAVABURST);

                //10 seconds until we should cast this agian
                LavaBurst_Timer = 10000;
            }else LavaBurst_Timer -= diff;

            //Submerge_Timer
            if (Submerge_Timer < diff)
            {
                //Creature spawning and ragnaros becomming unattackable
                //is not very well supported in the core
                //so for now we are just going to yell some lines
                
                if (!HasSubmergedOnce)
                {
                    //Say our dialog
                    DoYell(SAY_REINFORCEMENTS1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS1);
                    HasSubmergedOnce = true;
                }else
                {
                    //Say our dialog
                    DoYell(SAY_REINFORCEMENTS2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS2);
                }

                //3 minutes until we should cast this agian
                Submerge_Timer = 180000;
            }else Submerge_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }else
            {
                //MagmaBurst_Timer
                if (MagmaBurst_Timer < diff)
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_MAGMABURST);
                    
                    if (!HasYelledMagmaBurst)
                    {
                        //Say our dialog
                        DoYell(SAY_MAGMABURST,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_MAGMABURST);
                        HasYelledMagmaBurst = true;
                    }

                    //8 seconds until we should cast this agian
                    MagmaBurst_Timer = 8000;
                }else MagmaBurst_Timer -= diff;
            }
        }
    }
}; 
CreatureAI* GetAI_boss_ragnaros(Creature *_Creature)
{
    return new boss_ragnarosAI (_Creature);
}


void AddSC_boss_ragnaros()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ragnaros";
    newscript->GetAI = GetAI_boss_ragnaros;
    m_scripts[nrscripts++] = newscript;
}