/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Areatrigger_Scripts
SD%Complete: 100
SDComment: Quest support: 4291, 6681, 11686, 10589/10604, 12741, 12548, 13315/13351, 12575
SDCategory: Areatrigger
EndScriptData */

/* ContentData
at_aldurthar_gate               5284,5285,5286,5287
at_coilfang_waterfall           4591
at_legion_teleporter            4560 Teleporter TO Invasion Point: Cataclysm
at_ravenholdt
at_spearborn_encampment         5030
at_warsong_farms
at_stormwright_shelf            5108
at_childrens_week_spot          3546,3547,3548,3552,3549,3550
at_scent_larkorwi               1726,1727,1728,1729,1730,1731,1732,1733,1734,1735,1736,1737,1738,1739,1740
at_murkdeep                     1966
EndContentData */

#include "precompiled.h"
#include "world_map_scripts.h"

static uint32 TriggerOrphanSpell[6][3] =
{
    {3546, 14305, 65056},                                   // The Bough of the Eternals
    {3547, 14444, 65059},                                   // Lordaeron Throne Room
    {3548, 14305, 65055},                                   // The Stonewrought Dam
    {3549, 14444, 65058},                                   // Gateway to the Frontier
    {3550, 14444, 65057},                                   // Down at the Docks
    {3552, 14305, 65054}                                    // Spooky Lighthouse
};

bool AreaTrigger_at_childrens_week_spot(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    for (uint8 i = 0; i < 6; ++i)
    {
        if (pAt->id == TriggerOrphanSpell[i][0] &&
                pPlayer->GetMiniPet() && pPlayer->GetMiniPet()->GetEntry() == TriggerOrphanSpell[i][1])
        {
            pPlayer->CastSpell(pPlayer, TriggerOrphanSpell[i][2], true);
            return true;
        }
    }
    return false;
}

/*######
## Quest 13315/13351
######*/

enum
{
    TRIGGER_SOUTH               = 5284,
    TRIGGER_CENTRAL             = 5285,
    TRIGGER_NORTH               = 5286,
    TRIGGER_NORTHWEST           = 5287,

    NPC_SOUTH_GATE              = 32195,
    NPC_CENTRAL_GATE            = 32196,
    NPC_NORTH_GATE              = 32197,
    NPC_NORTHWEST_GATE          = 32199
};

bool AreaTrigger_at_aldurthar_gate(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    switch (pAt->id)
    {
        case TRIGGER_SOUTH:     pPlayer->KilledMonsterCredit(NPC_SOUTH_GATE);     break;
        case TRIGGER_CENTRAL:   pPlayer->KilledMonsterCredit(NPC_CENTRAL_GATE);   break;
        case TRIGGER_NORTH:     pPlayer->KilledMonsterCredit(NPC_NORTH_GATE);     break;
        case TRIGGER_NORTHWEST: pPlayer->KilledMonsterCredit(NPC_NORTHWEST_GATE); break;
    }
    return true;
}

/*######
## at_coilfang_waterfall
######*/

enum
{
    GO_COILFANG_WATERFALL   = 184212
};

bool AreaTrigger_at_coilfang_waterfall(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (GameObject* pGo = GetClosestGameObjectWithEntry(pPlayer, GO_COILFANG_WATERFALL, 35.0f))
    {
        if (pGo->getLootState() == GO_READY)
            pGo->UseDoorOrButton();
    }
    return false;
}

/*######
## at_legion_teleporter
######*/

enum
{
    SPELL_TELE_A_TO         = 37387,
    QUEST_GAINING_ACCESS_A  = 10589,

    SPELL_TELE_H_TO         = 37389,
    QUEST_GAINING_ACCESS_H  = 10604
};

bool AreaTrigger_at_legion_teleporter(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isAlive() && !pPlayer->isInCombat())
    {
        if (pPlayer->GetTeam() == ALLIANCE && pPlayer->GetQuestRewardStatus(QUEST_GAINING_ACCESS_A))
        {
            pPlayer->CastSpell(pPlayer, SPELL_TELE_A_TO, false);
            return true;
        }

        if (pPlayer->GetTeam() == HORDE && pPlayer->GetQuestRewardStatus(QUEST_GAINING_ACCESS_H))
        {
            pPlayer->CastSpell(pPlayer, SPELL_TELE_H_TO, false);
            return true;
        }
        return false;
    }
    return false;
}

/*######
## at_ravenholdt
######*/

enum
{
    QUEST_MANOR_RAVENHOLDT  = 6681,
    NPC_RAVENHOLDT          = 13936
};

bool AreaTrigger_at_ravenholdt(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->GetQuestStatus(QUEST_MANOR_RAVENHOLDT) == QUEST_STATUS_INCOMPLETE)
        pPlayer->KilledMonsterCredit(NPC_RAVENHOLDT);

    return false;
}

/*######
## at_spearborn_encampment
######*/

enum
{
    QUEST_MISTWHISPER_TREASURE          = 12575,
    NPC_TARTEK                          = 28105,
};

// 5030
bool AreaTrigger_at_spearborn_encampment(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->GetQuestStatus(QUEST_MISTWHISPER_TREASURE) == QUEST_STATUS_INCOMPLETE &&
            pPlayer->GetReqKillOrCastCurrentCount(QUEST_MISTWHISPER_TREASURE, NPC_TARTEK) == 0)
    {
        // can only spawn one at a time, it's not a too good solution
        if (GetClosestCreatureWithEntry(pPlayer, NPC_TARTEK, 50.0f))
            return false;

        pPlayer->SummonCreature(NPC_TARTEK, pAt->x, pAt->y, pAt->z, 0.0f, TEMPSUMMON_TIMED_OOC_OR_DEAD_DESPAWN, MINUTE * IN_MILLISECONDS);
    }

    return false;
}

/*######
## at_warsong_farms
######*/

enum
{
    QUEST_THE_WARSONG_FARMS     = 11686,
    NPC_CREDIT_SLAUGHTERHOUSE   = 25672,
    NPC_CREDIT_GRAINERY         = 25669,
    NPC_CREDIT_TORP_FARM        = 25671,

    AT_SLAUGHTERHOUSE           = 4873,
    AT_GRAINERY                 = 4871,
    AT_TORP_FARM                = 4872
};

bool AreaTrigger_at_warsong_farms(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_THE_WARSONG_FARMS) == QUEST_STATUS_INCOMPLETE)
    {
        switch (pAt->id)
        {
            case AT_SLAUGHTERHOUSE: pPlayer->KilledMonsterCredit(NPC_CREDIT_SLAUGHTERHOUSE); break;
            case AT_GRAINERY:       pPlayer->KilledMonsterCredit(NPC_CREDIT_GRAINERY);       break;
            case AT_TORP_FARM:      pPlayer->KilledMonsterCredit(NPC_CREDIT_TORP_FARM);      break;
        }
    }
    return true;
}

/*######
## Quest 12548
######*/

enum
{
    SPELL_SHOLOZAR_TO_UNGORO_TELEPORT = 52056,
    SPELL_UNGORO_TO_SHOLOZAR_TELEPORT = 52057,
    AT_WAYGATE_SHOLOZAR               = 5046,
    AT_WAYGATE_UNGORO                 = 5047,
    QUEST_THE_MARKERS_OVERLOOK        = 12613,
    QUEST_THE_MARKERS_PERCH           = 12559
};

bool AreaTrigger_at_waygate(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_THE_MARKERS_OVERLOOK) == QUEST_STATUS_COMPLETE && pPlayer->GetQuestStatus(QUEST_THE_MARKERS_PERCH) == QUEST_STATUS_COMPLETE)
    {
        switch (pAt->id)
        {
            case AT_WAYGATE_SHOLOZAR: pPlayer->CastSpell(pPlayer, SPELL_SHOLOZAR_TO_UNGORO_TELEPORT, false); break;
            case AT_WAYGATE_UNGORO: pPlayer->CastSpell(pPlayer, SPELL_UNGORO_TO_SHOLOZAR_TELEPORT, false); break;
        }
    }

    return false;
}

/*######
## Quest 12741
######*/

enum
{
    QUEST_STRENGTH_OF_THE_TEMPEST            = 12741,
    SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST   = 53067
};

bool AreaTrigger_at_stormwright_shelf(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_STRENGTH_OF_THE_TEMPEST) == QUEST_STATUS_INCOMPLETE)
        pPlayer->CastSpell(pPlayer, SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST, false);

    return true;
}

/*######
## at_scent_larkorwi
######*/

enum
{
    QUEST_SCENT_OF_LARKORWI     = 4291,
    NPC_LARKORWI_MATE           = 9683
};

bool AreaTrigger_at_scent_larkorwi(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isAlive() && !pPlayer->isGameMaster() && pPlayer->GetQuestStatus(QUEST_SCENT_OF_LARKORWI) == QUEST_STATUS_INCOMPLETE)
    {
        if (!GetClosestCreatureWithEntry(pPlayer, NPC_LARKORWI_MATE, 25.0f, false, false))
            pPlayer->SummonCreature(NPC_LARKORWI_MATE, pAt->x, pAt->y, pAt->z, 3.3f, TEMPSUMMON_TIMED_OOC_DESPAWN, 2 * MINUTE * IN_MILLISECONDS);
    }

    return false;
}

/*######
## at_murkdeep
######*/

bool AreaTrigger_at_murkdeep(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    // Handle Murkdeep event start
    // The area trigger summons 3 Greymist Coastrunners; The rest of the event is handled by world map scripts
    if (pPlayer->isAlive() && !pPlayer->isGameMaster() && pPlayer->GetQuestStatus(QUEST_WANTED_MURKDEEP) == QUEST_STATUS_INCOMPLETE)
    {
        ScriptedMap* pScriptedMap = (ScriptedMap*)pPlayer->GetInstanceData();
        if (!pScriptedMap)
            return false;

        // If Murkdeep is already spawned, skip the rest
        if (pScriptedMap->GetSingleCreatureFromStorage(NPC_MURKDEEP, true))
            return true;

        // Check if there are already coastrunners (dead or alive) around the area
        if (GetClosestCreatureWithEntry(pPlayer, NPC_GREYMIST_COASTRUNNNER, 60.0f, false, false))
            return true;

        float fX, fY, fZ;
        for (uint8 i = 0; i < 3; ++i)
        {
            // Spawn locations are defined in World Maps Scripts.h
            pPlayer->GetRandomPoint(aSpawnLocations[POS_IDX_MURKDEEP_SPAWN][0], aSpawnLocations[POS_IDX_MURKDEEP_SPAWN][1], aSpawnLocations[POS_IDX_MURKDEEP_SPAWN][2], 5.0f, fX, fY, fZ);

            if (Creature* pTemp = pPlayer->SummonCreature(NPC_GREYMIST_COASTRUNNNER, fX, fY, fZ, aSpawnLocations[POS_IDX_MURKDEEP_SPAWN][3], TEMPSUMMON_DEAD_DESPAWN, 0))
            {
                pTemp->SetWalk(false);
                pTemp->GetRandomPoint(aSpawnLocations[POS_IDX_MURKDEEP_MOVE][0], aSpawnLocations[POS_IDX_MURKDEEP_MOVE][1], aSpawnLocations[POS_IDX_MURKDEEP_MOVE][2], 5.0f, fX, fY, fZ);
                pTemp->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
            }
        }
    }

    return false;
}

void AddSC_areatrigger_scripts()
{
    AutoScript s;

    s.newScript("at_childrens_week_spot");
    s->pAreaTrigger = &AreaTrigger_at_childrens_week_spot;

    s.newScript("at_aldurthar_gate");
    s->pAreaTrigger = &AreaTrigger_at_aldurthar_gate;

    s.newScript("at_coilfang_waterfall");
    s->pAreaTrigger = &AreaTrigger_at_coilfang_waterfall;

    s.newScript("at_legion_teleporter");
    s->pAreaTrigger = &AreaTrigger_at_legion_teleporter;

    s.newScript("at_ravenholdt");
    s->pAreaTrigger = &AreaTrigger_at_ravenholdt;

    s.newScript("at_spearborn_encampment");
    s->pAreaTrigger = &AreaTrigger_at_spearborn_encampment;

    s.newScript("at_warsong_farms");
    s->pAreaTrigger = &AreaTrigger_at_warsong_farms;

    s.newScript("at_waygate");
    s->pAreaTrigger = &AreaTrigger_at_waygate;

    s.newScript("at_stormwright_shelf");
    s->pAreaTrigger = &AreaTrigger_at_stormwright_shelf;

    s.newScript("at_scent_larkorwi");
    s->pAreaTrigger = &AreaTrigger_at_scent_larkorwi;

    s.newScript("at_murkdeep");
    s->pAreaTrigger = &AreaTrigger_at_murkdeep;
}
