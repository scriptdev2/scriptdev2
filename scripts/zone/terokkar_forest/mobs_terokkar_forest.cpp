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
#include "../../../../../game/GossipDef.h"
#include "../../../../../game/QuestDef.h"

/*######
## mob_infested_root_walker
######*/

struct MANGOS_DLL_DECL mob_infested_root_walkerAI : public ScriptedAI
{
    mob_infested_root_walkerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //m_creature->CastSpell(m_creature,7941,true);//Immunity: Nature
    }

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if( ((Player*)done_by)->GetQuestStatus(10896) == QUEST_STATUS_INCOMPLETE && !((Player*)done_by)->GetReqKillOrCastCurrentCount(10896, m_creature->GetEntry()) )
        {
            if (m_creature->GetHealth() <= damage)
            {
                if (rand()%100 < 75)
                    m_creature->CastSpell(m_creature,39130,true);//Summon Wood Mites
            }
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
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
};
CreatureAI* GetAI_mob_infested_root_walker(Creature *_Creature)
{
    return new mob_infested_root_walkerAI (_Creature);
}

/*######
## mob_rotting_forest_rager
######*/

struct MANGOS_DLL_DECL mob_rotting_forest_ragerAI : public ScriptedAI
{
    mob_rotting_forest_ragerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //m_creature->CastSpell(m_creature,7941,true);//Immunity: Nature
    }

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if( ((Player*)done_by)->GetQuestStatus(10896) == QUEST_STATUS_INCOMPLETE && !((Player*)done_by)->GetReqKillOrCastCurrentCount(10896, m_creature->GetEntry()) )
        {
            if (m_creature->GetHealth() <= damage)
            {
                if (rand()%100 < 75)
                    m_creature->CastSpell(m_creature,39134,true);//Summon Lots of Wood Mights
            }
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
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
};
CreatureAI* GetAI_mob_rotting_forest_rager(Creature *_Creature)
{
    return new mob_rotting_forest_ragerAI (_Creature);
}
/*######
## mob_netherweb_victim
######*/

//various mobs to summon: Bonelasher, Broken Skeleton, Auchenai Death-Speaker, Skithian Dreadhawk, Mature Bone Sifter, Auchenai Doomsayer
const uint32 netherwebVictims[6] = 
{
    18470,
    16805,
    21242,
    18452,
    22482,
    21285
};
struct MANGOS_DLL_DECL mob_netherweb_victimAI : public ScriptedAI
{
    mob_netherweb_victimAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void SummonVictims(Unit* victim)
    {
    int Rand;
    int RandX;
    int RandY;

        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        DoSpawnCreature(netherwebVictims[rand()%6], RandX, RandY, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
        {
            //check quest status
            if( ((Player*)Killer)->GetQuestStatus(10873) == QUEST_STATUS_INCOMPLETE && !((Player*)Killer)->GetReqKillOrCastCurrentCount(10873, m_creature->GetEntry()) )
            {
                if (rand()%100 < 25)
                {
                    DoSpawnCreature(22459, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                    ((Player*)Killer)->KilledMonster(22459, m_creature->GetGUID());
                }
                SummonVictims(m_creature->getVictim());
                SummonVictims(m_creature->getVictim());
                if (rand()%100 < 75)
                    SummonVictims(m_creature->getVictim());

            }
        }
    }
    void AttackStart(Unit *who)
    {
        return;
    }
    void MoveInLineOfSight(Unit *who)
    {
        return;
    }
};
CreatureAI* GetAI_mob_netherweb_victim(Creature *_Creature)
{
    return new mob_netherweb_victimAI (_Creature);
}

/*######
## mobs_shadow_council_covert
######*/

#define GOSSIP_ITEM_SHA "Speak"

bool GossipHello_mobs_shadow_council_covert(Player *player, Creature *_Creature )
{
    if (player->GetTeam() == ALLIANCE)
    {
        if ( _Creature->GetEntry() == 18716 && !player->GetReqKillOrCastCurrentCount(10040, 18716) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        if ( _Creature->GetEntry() == 18717 && !player->GetReqKillOrCastCurrentCount(10040, 18717) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        if ( _Creature->GetEntry() == 18719 && !player->GetReqKillOrCastCurrentCount(10040, 18719) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    }
    if (player->GetTeam() == HORDE)
    {
        if ( _Creature->GetEntry() == 18716 && !player->GetReqKillOrCastCurrentCount(10041, 18716) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        if ( _Creature->GetEntry() == 18717 && !player->GetReqKillOrCastCurrentCount(10041, 18717) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        if ( _Creature->GetEntry() == 18719 && !player->GetReqKillOrCastCurrentCount(10041, 18719) )
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_SHA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    }

    player->SEND_GOSSIP_MENU(384,_Creature->GetGUID());

    return true;
}
bool GossipSelect_mobs_shadow_council_covert(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->SEND_GOSSIP_MENU(384, _Creature->GetGUID());
            player->KilledMonster(18716, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->SEND_GOSSIP_MENU(384, _Creature->GetGUID());
            player->KilledMonster(18717, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_GOSSIP_MENU(384, _Creature->GetGUID());
            player->KilledMonster(18719, _Creature->GetGUID());
            break;
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_mobs_terokkar_forest()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_infested_root_walker";
    newscript->GetAI = GetAI_mob_infested_root_walker;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_rotting_forest_rager";
    newscript->GetAI = GetAI_mob_rotting_forest_rager;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_netherweb_victim";
    newscript->GetAI = GetAI_mob_netherweb_victim;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_shadow_council_covert";
    newscript->pGossipHello =  &GossipHello_mobs_shadow_council_covert;
    newscript->pGossipSelect = &GossipSelect_mobs_shadow_council_covert;
    m_scripts[nrscripts++] = newscript;
}
