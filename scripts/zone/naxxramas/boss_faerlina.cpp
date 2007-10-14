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

#define SAY_GREET           "Your old lives, your mortal desires, mean nothing. You are acolytes of the master now, and you will serve the cause without question! The greatest glory is to die in the master's service!"
#define SAY_AGGRO1          "Slay them in the master's name!"
#define SAY_AGGRO2          "You cannot hide from me!"
#define SAY_AGGRO3          "Kneel before me, worm!"
#define SAY_AGGRO4          "Run while you still can!"
#define SAY_SLAY1           "You have failed!"
#define SAY_SLAY2           "Pathetic wretch!"
#define SAY_DEATH           "The master... will avenge me!"
#define SAY_RANDOM_AGGRO    "???"

#define SOUND_GREET         8799
#define SOUND_AGGRO1        8794
#define SOUND_AGGRO2        8795
#define SOUND_AGGRO3        8796
#define SOUND_AGGRO4        8797
#define SOUND_SLAY1         8800
#define SOUND_SLAY2         8801
#define SOUND_DEATH         8798
#define SOUND_RANDOM_AGGRO  8955

#define SPELL_POSIONBOLT_VOLLEY     28796
#define SPELL_RAINOFFIRE            28794   //Not sure if targeted AoEs work if casted directly upon a player
#define SPELL_ENRAGE                26527


struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 PoisonBoltVolley_Timer;
    uint32 RainOfFire_Timer;
    uint32 Enrage_Timer;
    bool InCombat;
    bool HasTaunted;

    void EnterEvadeMode()
    {       
        PoisonBoltVolley_Timer = 8000;
        RainOfFire_Timer = 16000;
        Enrage_Timer = 60000;
        InCombat = false;
        HasTaunted = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            //Say our dialog on initial aggro
            if (!InCombat)
            {
                switch (rand()%4)
                {
                case 0:
                    DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
                    break;
                case 1:
                    DoYell(SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
                    break;
                case 2:
                    DoYell(SAY_AGGRO3,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
                    break;
                case 3:
                    DoYell(SAY_AGGRO4,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO4);
                    break;
                }
                InCombat = true;
            }
        }
    }

    //Extended Visbility range for taunts
    bool IsVisible(Unit *who) const
    {
        if (!who)
            return false;

        return m_creature->IsWithinDistInMap(who, 100.0f) && who->isVisibleForOrDetect(m_creature,true);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                //Say our dialog on initial aggro
                if (!InCombat)
                {
                    switch (rand()%4)
                    {
                    case 0:
                        DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
                        break;
                    case 1:
                        DoYell(SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
                        break;
                    case 2:
                        DoYell(SAY_AGGRO3,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
                        break;
                    case 3:
                        DoYell(SAY_AGGRO4,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO4);
                        break;
                    }
                    InCombat = true;
                }
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {

                DoYell(SAY_GREET, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_GREET);
                HasTaunted = true;
            }
        }
    }


    void KilledUnit(Unit* victim)
    {

        switch (rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY1);
            break;
        case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY2);
            break;
        }

    }


    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //PoisonBoltVolley_Timer
        if (PoisonBoltVolley_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_POSIONBOLT_VOLLEY);

            //11 seconds
            PoisonBoltVolley_Timer = 11000;
        }else PoisonBoltVolley_Timer -= diff;

        //RainOfFire_Timer
        if (RainOfFire_Timer < diff)
        {

            //Cast Immolate on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_RAINOFFIRE);

            //16 seconds until we should cast this agian
            RainOfFire_Timer = 16000;
        }else RainOfFire_Timer -= diff;

        //Enrage_Timer
        if (Enrage_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_ENRAGE);

            //61 seconds until we should cast this agian
            Enrage_Timer = 61000;
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_faerlina(Creature *_Creature)
{
    return new boss_faerlinaAI (_Creature);
}


void AddSC_boss_faerlina()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_faerlina";
    newscript->GetAI = GetAI_boss_faerlina;
    m_scripts[nrscripts++] = newscript;
}


