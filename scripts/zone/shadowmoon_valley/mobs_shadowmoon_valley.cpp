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

/* ScriptData
SDName: mobs_shadowmoon_valley
SD%Complete: 100
SDComment: misc mobs, mostly related to quest
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"

/*######
## mobs_dragonmaw_orcs
######*/

struct MANGOS_DLL_DECL mobs_dragonmaw_orcAI : public ScriptedAI
{
    mobs_dragonmaw_orcAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(22197, m_creature->GetGUID());
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
CreatureAI* GetAI_mobs_dragonmaw_orc(Creature *_Creature)
{
    return new mobs_dragonmaw_orcAI (_Creature);
}

/*######
## mobs_shadowmoon_valley_wildlife
######*/

struct MANGOS_DLL_DECL mobs_shadowmoon_valley_wildlifeAI : public ScriptedAI
{
    mobs_shadowmoon_valley_wildlifeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(21978, m_creature->GetGUID());
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
CreatureAI* GetAI_mobs_shadowmoon_valley_wildlife(Creature *_Creature)
{
    return new mobs_shadowmoon_valley_wildlifeAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_shadowmoon_valley()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_dragonmaw_orc";
    newscript->GetAI = GetAI_mobs_dragonmaw_orc;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_shadowmoon_valley_wildlife";
    newscript->GetAI = GetAI_mobs_shadowmoon_valley_wildlife;
    m_scripts[nrscripts++] = newscript;
}
