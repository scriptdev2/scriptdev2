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



//If they being zapped with Egan's Blaster they aggro but die after 10 sec
//OnDie they spawn 1-4 restless souls
//Restless Souls are zapped once to spawn one Freed Soul each

//If aggroed without the zapper being used, type /dance to loose aggro


// http://www.wowwiki.com/Quest:The_Restless_Souls_(Part_2)

//#define SPELL_EGANSBLASTER    17368 //Egan's Blaster :: Detect is this being used. If used, die after 10 sec
//#define SPELL_EGANSBLASTEREFFECT    17372 //Egan's Blaster Effect :: Summons 1 Freed Soul if used on this NPC

#define SPELL_VEILOFSHADOW    22687
#define SPELL_HAUNTINGPHANTOMS    16336
#define SPELL_DAZED    1604

struct MANGOS_DLL_DECL mob_ghostly_citizenAI : public ScriptedAI
{
    mob_ghostly_citizenAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 VeilOfShadow_Timer;
    uint32 HauntingPhantoms_Timer;
    uint32 Dazed_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        VeilOfShadow_Timer = 11000;
        HauntingPhantoms_Timer = 8000;
        Dazed_Timer = 14000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            InCombat = true;
        }
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
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

            //VeilOfShadow
            if (VeilOfShadow_Timer < diff)
            {
                //Cast
                if (rand()%100 < 70) //70% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_VEILOFSHADOW);
                }
                //13 seconds until we should cast this again
                VeilOfShadow_Timer = 13000;
            }else VeilOfShadow_Timer -= diff;

            //HauntingPhantoms
            if (HauntingPhantoms_Timer < diff)
            {
                //Cast
                if (rand()%100 < 35) //35% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_HAUNTINGPHANTOMS);
                }
                //11 seconds until we should cast this again
                HauntingPhantoms_Timer = 11000;
            }else HauntingPhantoms_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //15 seconds until we should cast this again
                Dazed_Timer = 15000;
            }else Dazed_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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
CreatureAI* GetAI_mob_ghostly_citizen(Creature *_Creature)
{
    return new mob_ghostly_citizenAI (_Creature);
}

bool ReciveEmote_mob_ghostly_citizen(Player *player, Creature *_Creature, uint32 emote)
{
    _Creature->HandleEmoteCommand(emote);

    if (emote == EMOTE_STATE_DANCE)
        ((mob_ghostly_citizenAI*)_Creature->AI())->EnterEvadeMode();

    return true;
}

void AddSC_mob_ghostly_citizen()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_ghostly_citizen";
    newscript->GetAI = GetAI_mob_ghostly_citizen;
    newscript->pReceiveEmote = &ReciveEmote_mob_ghostly_citizen;
    m_scripts[nrscripts++] = newscript;
}