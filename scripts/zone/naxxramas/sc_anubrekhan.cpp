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

#define SAY_AGGRO1  "Just a little taste..."
#define SAY_AGGRO2  "There is no way out."
#define SAY_AGGRO3  "Yes, Run! It makes the blood pump faster!"
#define SAY_GREET   "Ahh... welcome to my parlor"
#define SAY_TAUNT1  "I hear little hearts beating. Yesss... beating faster now. Soon the beating will stop."
#define SAY_TAUNT2  "Where to go? What to do? So many choices that all end in pain, end in death."
#define SAY_TAUNT3  "Which one shall I eat first? So difficult to choose... the all smell so delicious."
#define SAY_TAUNT4  "Closer now... tasty morsels. I've been too long without food. Without blood to drink."
#define SAY_SLAY    "Shh... it will all be over soon."

#define SOUND_AGGRO1  8785
#define SOUND_AGGRO2  8786
#define SOUND_AGGRO3  8787
#define SOUND_GREET   8788
#define SOUND_TAUNT1  8790
#define SOUND_TAUNT2  8791
#define SOUND_TAUNT3  8792
#define SOUND_TAUNT4  8793
#define SOUND_SLAY    8789

#define SPELL_IMPALE        28783   //May be wrong spell id. Causes more dmg than I expect
#define SPELL_LOCUSTSWARM   28785   //This is a self buff that triggers the dmg debuff
#define SPELL_SUMMONGUARD   29508   //Summons 1 crypt guard at targeted location

#define SPELL_SELF_SPAWN_5  29105   //This spawns 5 corpse scarabs ontop of us (most likely the player casts this on death)
#define SPELL_SELF_SPAWN_10 28864   //This is used by the crypt guards when they die

struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature *c) : ScriptedAI(c) {Reset();HasTaunted = false;}

    uint32 Impale_Timer;
    uint32 LocustSwarm_Timer;
    uint32 Summon_Timer;
    bool InCombat;
    bool HasTaunted;

    void Reset()
    {
        Impale_Timer = 15000;                       //15 seconds
        LocustSwarm_Timer = 80000 + (rand()%40000); //Random time between 80 seconds and 2 minutes for initial cast      
        Summon_Timer = LocustSwarm_Timer + 45000;   //45 seconds after initial locust swarm
        InCombat = false;

        if (m_creature)
            EnterEvadeMode();
    }

    void KilledUnit(Unit* Victim)
    {
        //Force the player to spawn corpse scarabs via spell
        Victim->CastSpell(Victim, SPELL_SELF_SPAWN_5, true);

        if (rand()%5)
            return;

        DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SLAY);
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
                switch(rand()%3)
                {
                    case 0:
                    DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                    break;

                    case 1:
                    DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                    break;

                    case 2:
                    DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
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

        return m_creature->IsWithinDist(who, 100.0f) && who->isVisibleFor(m_creature,true);
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
                    switch(rand()%3)
                    {
                        case 0:
                        DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                        break;

                        case 1:
                        DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                        break;

                        case 2:
                        DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
                        break;
                    }
                    InCombat = true;
                }
            }
            else if (!HasTaunted && m_creature->IsWithinDist(who, 60.0f))
            {
                switch(rand()%5)
                {
                    case 0:
                    DoYell(SAY_TAUNT1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT1);
                    break;

                    case 1:
                    DoYell(SAY_TAUNT2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT2);
                    break;

                    case 2:
                    DoYell(SAY_TAUNT3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT3);
                    break;

                    case 3:
                    DoYell(SAY_TAUNT4, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT4);
                    break;

                    case 4:
                    DoYell(SAY_GREET, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_GREET);
                    break;
                }
                HasTaunted = true;
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
            
            //Impale_Timer
            if (Impale_Timer < diff)
            {
                //Cast Impale on a random target
                //Do NOT cast it when we are afflicted by locust swarm
                if (!m_creature->HasAura(SPELL_LOCUSTSWARM,1))
                    DoCast(m_creature->getVictim(),SPELL_IMPALE);

                //15 seconds until we should cast this agian
                Impale_Timer = 15000;
            }else Impale_Timer -= diff;

            //LocustSwarm_Timer
            if (LocustSwarm_Timer < diff)
            {
                //Cast Locust Swarm buff on ourselves
                DoCast(m_creature, SPELL_LOCUSTSWARM);

                //90 seconds until we should cast this agian
                LocustSwarm_Timer = 90000;
            }else LocustSwarm_Timer -= diff;

            //Summon_Timer
            if (Summon_Timer < diff)
            {
                //Summon a crypt guard
                DoCast(m_creature, SPELL_SUMMONGUARD);

                //45 seconds until we should cast this agian
                Summon_Timer = 45000;
            }else Summon_Timer -= diff;

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
CreatureAI* GetAI_boss_anubrekhan(Creature *_Creature)
{
    return new boss_anubrekhanAI (_Creature);
}


void AddSC_boss_anubrekhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_anubrekhan";
    newscript->GetAI = GetAI_boss_anubrekhan;
    m_scripts[nrscripts++] = newscript;
}