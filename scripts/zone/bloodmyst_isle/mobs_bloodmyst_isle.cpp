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
SDName: Mobs_Bloodmyst_Isle
SD%Complete: 100
SDComment: Quest Support: 9670
SDCategory: Bloodmyst Isle
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"

/*######
## mob_webbed_creature
######*/

//possible creatures to be spawned
const uint32 possibleSpawns[32] = {17322, 17661, 17496, 17522, 17340, 17352, 17333, 17524, 17654, 17348, 17339, 17345, 17359, 17353, 17336, 17550, 17330, 17701, 17321, 17680, 17325, 17320, 17683, 17342, 17715, 17334, 17341, 17338, 17337, 17346, 17344, 17327};

struct MANGOS_DLL_DECL mob_webbed_creatureAI : public ScriptedAI
{
    mob_webbed_creatureAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void JustDied(Unit* Killer)
    {
        uint32 spawnCreatureID;

        switch(rand()%3)
        {
        case 0:
            spawnCreatureID = 17681;
            if (Killer->GetTypeId() == TYPEID_PLAYER)
                ((Player*)Killer)->KilledMonster(spawnCreatureID, m_creature->GetGUID());
            break;
        case 1:
        case 2:
            spawnCreatureID = possibleSpawns[rand()%31];
            break;
        }

        if(spawnCreatureID)
            DoSpawnCreature(spawnCreatureID,0,0,0,m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
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
            }
        }
    }
};
CreatureAI* GetAI_mob_webbed_creature(Creature *_Creature)
{
    return new mob_webbed_creatureAI (_Creature);
}

void AddSC_mobs_bloodmyst_isle()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_webbed_creature";
    newscript->GetAI = GetAI_mob_webbed_creature;
    m_scripts[nrscripts++] = newscript;
}
