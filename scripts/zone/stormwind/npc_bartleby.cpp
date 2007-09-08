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
#include "../../../../../game/Player.h"

struct MANGOS_DLL_DECL npc_bartlebyAI : public ScriptedAI
{
    npc_bartlebyAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    Unit* PlayerHolder;
    
    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        m_creature->setFaction(11);
        DoGoHome();
        m_creature->setEmoteState(7);

        PlayerHolder = NULL;
    }

    void JustDied(Unit *who)
    {
        PlayerHolder = NULL;
    }
    
    void DamageTaken(Unit *done_by, uint32 & damage)
    { 
        if ((m_creature->GetHealth() - damage)*100 / m_creature->GetMaxHealth() < 15)
        {
            //Take 0 damage
            damage = 0;
            
            if (done_by->GetTypeId() == TYPEID_PLAYER)
            {
                ((Player*)done_by)->AttackStop();
                ((Player*)done_by)->CompleteQuest(1640);
            }
            m_creature->CombatStop();
            EnterEvadeMode();
            }
        AttackedBy(done_by);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
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

            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady())
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};

bool QuestAccept_npc_bartleby(Player *player, Creature *_Creature, Quest *_Quest)
{
    if(_Quest->GetQuestId() == 1640)
    {
        _Creature->setFaction(168);
        ((npc_bartlebyAI*)_Creature->AI())->PlayerHolder = (Player*)player;
        ((npc_bartlebyAI*)_Creature->AI())->AttackStart(player);
    }
    return true;
}

CreatureAI* GetAI_npc_bartleby(Creature *_creature)
{
    return new npc_bartlebyAI(_creature);
}

void AddSC_npc_bartleby()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_bartleby";
    newscript->GetAI = GetAI_npc_bartleby;
    newscript->pQuestAccept = &QuestAccept_npc_bartleby;
    m_scripts[nrscripts++] = newscript;
}
