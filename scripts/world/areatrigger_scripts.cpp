/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDComment: Quest support: 6681, 11686, 10589/10604, 12741, 13315/13351
SDCategory: Areatrigger
EndScriptData */

/* ContentData
at_aldurthar_gate               5284,5285,5286,5287
at_coilfang_waterfall           4591
at_legion_teleporter            4560 Teleporter TO Invasion Point: Cataclysm
at_ravenholdt
at_warsong_farms
at_stormwright_shelf            5108
at_childrens_week_spot          3546,3547,3548,3552,3549,3550
EndContentData */

#include "precompiled.h"
                                                                    
uint32 TriggerOrphanSpell[6][3] =
{
    {3546, 14305, 65056},   // The Bough of the Eternals
    {3547, 14444, 65059},   // Lordaeron Throne Room
    {3548, 14305, 65055},   // The Stonewrought Dam
    {3549, 14444, 65058},   // Gateway to the Frontier
    {3550, 14444, 65057},   // Down at the Docks
    {3552, 14305, 65054}    // Spooky Lighthouse
};

bool AreaTrigger_at_childrens_week_spot(Player* pPlayer, AreaTriggerEntry* pAt)
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

bool AreaTrigger_at_aldurthar_gate(Player* pPlayer, AreaTriggerEntry* pAt)
{
    switch(pAt->id)
    {
        case TRIGGER_SOUTH:     pPlayer->KilledMonsterCredit(NPC_SOUTH_GATE, 0);     break;
        case TRIGGER_CENTRAL:   pPlayer->KilledMonsterCredit(NPC_CENTRAL_GATE, 0);   break;
        case TRIGGER_NORTH:     pPlayer->KilledMonsterCredit(NPC_NORTH_GATE, 0);     break;
        case TRIGGER_NORTHWEST: pPlayer->KilledMonsterCredit(NPC_NORTHWEST_GATE, 0); break;
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

bool AreaTrigger_at_coilfang_waterfall(Player* pPlayer, AreaTriggerEntry* pAt)
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

bool AreaTrigger_at_legion_teleporter(Player* pPlayer, AreaTriggerEntry* pAt)
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

bool AreaTrigger_at_ravenholdt(Player* pPlayer, AreaTriggerEntry* pAt)
{
    if (pPlayer->GetQuestStatus(QUEST_MANOR_RAVENHOLDT) == QUEST_STATUS_INCOMPLETE)
        pPlayer->KilledMonsterCredit(NPC_RAVENHOLDT, 0);

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

bool AreaTrigger_at_warsong_farms(Player* pPlayer, AreaTriggerEntry* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_THE_WARSONG_FARMS) == QUEST_STATUS_INCOMPLETE)
    {
        switch(pAt->id)
        {
            case AT_SLAUGHTERHOUSE: pPlayer->KilledMonsterCredit(NPC_CREDIT_SLAUGHTERHOUSE, 0); break;
            case AT_GRAINERY:       pPlayer->KilledMonsterCredit(NPC_CREDIT_GRAINERY, 0);       break;
            case AT_TORP_FARM:      pPlayer->KilledMonsterCredit(NPC_CREDIT_TORP_FARM, 0);      break;
        }
    }
    return true;
 }

/*######
## Quest 12741
######*/

enum
{
    QUEST_STRENGTH_OF_THE_TEMPEST            = 12741,
    SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST   = 53067
};

bool AreaTrigger_at_stormwright_shelf(Player* pPlayer, AreaTriggerEntry* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_STRENGTH_OF_THE_TEMPEST) == QUEST_STATUS_INCOMPLETE)
        pPlayer->CastSpell(pPlayer, SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST, false);

    return true;
}

void AddSC_areatrigger_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "at_childrens_week_spot";
    newscript->pAreaTrigger = &AreaTrigger_at_childrens_week_spot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_aldurthar_gate";
    newscript->pAreaTrigger = &AreaTrigger_at_aldurthar_gate;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_coilfang_waterfall";
    newscript->pAreaTrigger = &AreaTrigger_at_coilfang_waterfall;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_legion_teleporter";
    newscript->pAreaTrigger = &AreaTrigger_at_legion_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_ravenholdt";
    newscript->pAreaTrigger = &AreaTrigger_at_ravenholdt;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_warsong_farms";
    newscript->pAreaTrigger = &AreaTrigger_at_warsong_farms;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_stormwright_shelf";
    newscript->pAreaTrigger = &AreaTrigger_at_stormwright_shelf;
    newscript->RegisterSelf();
}
