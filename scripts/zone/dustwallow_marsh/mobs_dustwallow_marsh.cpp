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
SDName: mobs_dustwallow_marsh
SD%Complete: 100
SDComment: misc mobs, mostly related to quest
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"

/*######
## mobs_direhorn_grimtotem
######*/

struct MANGOS_DLL_DECL mobs_direhorn_grimtotemAI : public ScriptedAI
{
    mobs_direhorn_grimtotemAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(23594, m_creature->GetGUID());
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
};
CreatureAI* GetAI_mobs_direhorn_grimtotem(Creature *_Creature)
{
    return new mobs_direhorn_grimtotemAI (_Creature);
}


/*######
## mobs_risen_husk_spirit
######*/

#define SPELL_SUMMON_RESTLESS_APPARITION 42511

struct MANGOS_DLL_DECL mobs_risen_husk_spiritAI : public ScriptedAI
{
    mobs_risen_husk_spiritAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if (m_creature->GetHealth() <= damage)
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER && ((Player*)done_by)->GetQuestStatus(11180) == QUEST_STATUS_INCOMPLETE)
                m_creature->CastSpell(m_creature,SPELL_SUMMON_RESTLESS_APPARITION,true);
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
};
CreatureAI* GetAI_mobs_risen_husk_spirit(Creature *_Creature)
{
    return new mobs_risen_husk_spiritAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_dustwallow_marsh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_direhorn_grimtotem";
    newscript->GetAI = GetAI_mobs_direhorn_grimtotem;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_risen_husk_spirit";
    newscript->GetAI = GetAI_mobs_risen_husk_spirit;
    m_scripts[nrscripts++] = newscript;
}
