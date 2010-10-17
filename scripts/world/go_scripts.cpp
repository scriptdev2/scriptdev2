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
SDName: GO_Scripts
SD%Complete: 100
SDComment: Quest support: 4285,4287,4288(crystal pylons), 4296, 5088, 5097, 5098, 6481, 10990, 10991, 10992, 14092/14076. Field_Repair_Bot->Teaches spell 22704. Barov_journal->Teaches spell 26089
SDCategory: Game Objects
EndScriptData */

/* ContentData
go_cat_figurine (the "trap" version of GO, two different exist)
go_northern_crystal_pylon
go_eastern_crystal_pylon
go_western_crystal_pylon
go_barov_journal
go_ethereum_prison
go_ethereum_stasis
go_field_repair_bot_74A
go_mysterious_snow_mound
go_orb_of_command
go_resonite_cask
go_sacred_fire_of_life
go_shrine_of_the_birds
go_tablet_of_madness
go_tablet_of_the_seven
go_tele_to_dalaran_crystal
go_tele_to_violet_stand
go_andorhal_tower
go_scourge_enclosure
EndContentData */

#include "precompiled.h"

/*######
## go_cat_figurine
######*/

enum
{
    SPELL_SUMMON_GHOST_SABER    = 5968,
};

bool GOHello_go_cat_figurine(Player* pPlayer, GameObject* pGo)
{
    pPlayer->CastSpell(pPlayer, SPELL_SUMMON_GHOST_SABER, true);
    return false;
}

/*######
## go_crystal_pylons (3x)
######*/

enum
{
    QUEST_THE_NORTHERN_PYLON = 4285,
    QUEST_THE_EASTERN_PYLON  = 4287,
    QUEST_THE_WESTERN_PYLON  = 4288
};

bool GOHello_go_northern_crystal_pylon(Player* pPlayer, GameObject* pGo)
{
    if (pGo->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        pPlayer->PrepareQuestMenu(pGo->GetGUID());
        pPlayer->SendPreparedQuest(pGo->GetGUID());
    }

    if (pPlayer->GetQuestStatus(QUEST_THE_NORTHERN_PYLON) == QUEST_STATUS_INCOMPLETE)
        pPlayer->AreaExploredOrEventHappens(QUEST_THE_NORTHERN_PYLON);

    return true;
}

bool GOHello_go_eastern_crystal_pylon(Player* pPlayer, GameObject* pGo)
{
    if (pGo->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        pPlayer->PrepareQuestMenu(pGo->GetGUID());
        pPlayer->SendPreparedQuest(pGo->GetGUID());
    }

    if (pPlayer->GetQuestStatus(QUEST_THE_EASTERN_PYLON) == QUEST_STATUS_INCOMPLETE)
        pPlayer->AreaExploredOrEventHappens(QUEST_THE_EASTERN_PYLON);

    return true;
}

bool GOHello_go_western_crystal_pylon(Player* pPlayer, GameObject* pGo)
{
    if (pGo->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        pPlayer->PrepareQuestMenu(pGo->GetGUID());
        pPlayer->SendPreparedQuest(pGo->GetGUID());
    }

    if (pPlayer->GetQuestStatus(QUEST_THE_WESTERN_PYLON) == QUEST_STATUS_INCOMPLETE)
        pPlayer->AreaExploredOrEventHappens(QUEST_THE_WESTERN_PYLON);

    return true;
}

/*######
## go_barov_journal
######*/

enum
{
    SPELL_TAILOR_FELCLOTH_BAG = 26086,
    SPELL_LEARN_FELCLOTH_BAG  = 26095
};

bool GOHello_go_barov_journal(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->HasSkill(SKILL_TAILORING) && pPlayer->GetBaseSkillValue(SKILL_TAILORING) >= 280 && !pPlayer->HasSpell(SPELL_TAILOR_FELCLOTH_BAG))
    {
        pPlayer->CastSpell(pPlayer, SPELL_LEARN_FELCLOTH_BAG, false);
    }
    return true;
}

/*######
## go_ethereum_prison
######*/

enum
{
    FACTION_LC     = 1011,
    FACTION_SHAT   = 935,
    FACTION_CE     = 942,
    FACTION_CON    = 933,
    FACTION_KT     = 989,
    FACTION_SPOR   = 970,

    SPELL_REP_LC   = 39456,
    SPELL_REP_SHAT = 39457,
    SPELL_REP_CE   = 39460,
    SPELL_REP_CON  = 39474,
    SPELL_REP_KT   = 39475,
    SPELL_REP_SPOR = 39476
};

const uint32 uiNpcPrisonEntry[] =
{
    22810, 22811, 22812, 22813, 22814, 22815,               //good guys
    20783, 20784, 20785, 20786, 20788, 20789, 20790         //bad guys
};

bool GOHello_go_ethereum_prison(Player* pPlayer, GameObject* pGo)
{
    uint8 uiRandom = urand(0, (sizeof(uiNpcPrisonEntry) / sizeof(uint32)) -1);

    if (Creature* pCreature = pPlayer->SummonCreature(uiNpcPrisonEntry[uiRandom],
        pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), pGo->GetAngle(pPlayer),
        TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
    {
        if (!pCreature->IsHostileTo(pPlayer))
        {
            uint32 uiSpell = 0;

            if (FactionTemplateEntry const* pFaction = pCreature->getFactionTemplateEntry())
            {
                switch(pFaction->faction)
                {
                    case FACTION_LC:   uiSpell = SPELL_REP_LC;   break;
                    case FACTION_SHAT: uiSpell = SPELL_REP_SHAT; break;
                    case FACTION_CE:   uiSpell = SPELL_REP_CE;   break;
                    case FACTION_CON:  uiSpell = SPELL_REP_CON;  break;
                    case FACTION_KT:   uiSpell = SPELL_REP_KT;   break;
                    case FACTION_SPOR: uiSpell = SPELL_REP_SPOR; break;
                }

                if (uiSpell)
                    pCreature->CastSpell(pPlayer,uiSpell,false);
                else
                    error_log("SD2: go_ethereum_prison summoned creature (entry %u) but faction (%u) are not expected by script.",pCreature->GetEntry(),pCreature->getFaction());
            }
        }
    }

    return false;
}

/*######
## go_ethereum_stasis
######*/

const uint32 uiNpcStasisEntry[] =
{
    22825, 20888, 22827, 22826, 22828
};

bool GOHello_go_ethereum_stasis(Player* pPlayer, GameObject* pGo)
{
    uint8 uiRandom = urand(0, (sizeof(uiNpcStasisEntry) / sizeof(uint32)) -1);

    pPlayer->SummonCreature(uiNpcStasisEntry[uiRandom],
        pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), pGo->GetAngle(pPlayer),
        TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

    return false;
}

/*######
## go_field_repair_bot_74A
######*/

enum
{
    SPELL_ENGINEER_FIELD_REPAIR_BOT_74A = 22704,
    SPELL_LEARN_FIELD_REPAIR_BOT_74A    = 22864
};

bool GOHello_go_field_repair_bot_74A(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->HasSkill(SKILL_ENGINEERING) && pPlayer->GetBaseSkillValue(SKILL_ENGINEERING) >= 300 && !pPlayer->HasSpell(SPELL_ENGINEER_FIELD_REPAIR_BOT_74A))
        pPlayer->CastSpell(pPlayer, SPELL_LEARN_FIELD_REPAIR_BOT_74A, false);

    return true;
}

/*######
## go_gilded_brazier
######*/

enum
{
    NPC_STILLBLADE = 17716,
};

bool GOHello_go_gilded_brazier(Player* pPlayer, GameObject* pGO)
{
    if (pGO->GetGoType() == GAMEOBJECT_TYPE_GOOBER)
    {
        if (Creature* pCreature = pPlayer->SummonCreature(NPC_STILLBLADE, 8087.632f, -7542.740f, 151.568f, 0.122f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
            pCreature->AI()->AttackStart(pPlayer);
    }

    return true;
}

/*######
## go_jump_a_tron
######*/

enum
{
    SPELL_JUMP_A_TRON = 33382,
    NPC_JUMP_A_TRON   = 19041
};

bool GOHello_go_jump_a_tron(Player* pPlayer, GameObject* pGo)
{
    if (Creature* pCreature = GetClosestCreatureWithEntry(pGo, NPC_JUMP_A_TRON, INTERACTION_DISTANCE))
        pCreature->CastSpell(pPlayer, SPELL_JUMP_A_TRON, false);

    return false;
}

/*######
## go_mysterious_snow_mound
######*/

enum
{
    SPELL_SUMMON_DEEP_JORMUNGAR     = 66510,
    SPELL_SUMMON_MOLE_MACHINE       = 66492,
    SPELL_SUMMON_MARAUDER           = 66491,
};

bool GOHello_go_mysterious_snow_mound(Player* pPlayer, GameObject* pGo)
{
    if (urand(0,1))
    {
        pPlayer->CastSpell(pPlayer, SPELL_SUMMON_DEEP_JORMUNGAR, true);
    }
    else
    {
        // This is basically wrong, but added for support.
        // Mole machine would summon, along with unkonwn GO (a GO trap?) and then
        // the npc would summon with base of that location.
        pPlayer->CastSpell(pPlayer, SPELL_SUMMON_MOLE_MACHINE, true);
        pPlayer->CastSpell(pPlayer, SPELL_SUMMON_MARAUDER, true);
    }

    pGo->SetLootState(GO_JUST_DEACTIVATED);
    return true;
}

/*######
## go_orb_of_command
######*/

enum
{
    QUEST_BLACKHANDS_COMMAND = 7761,
    SPELL_TELEPORT_TO_BWL    = 23460
};

bool GOHello_go_orb_of_command(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestRewardStatus(QUEST_BLACKHANDS_COMMAND))
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_TO_BWL, true);

    return true;
}

/*######
## go_resonite_cask
######*/

enum
{
    NPC_GOGGEROC = 11920
};

bool GOHello_go_resonite_cask(Player* pPlayer, GameObject* pGO)
{
    if (pGO->GetGoType() == GAMEOBJECT_TYPE_GOOBER)
        pGO->SummonCreature(NPC_GOGGEROC, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 300000);

    return false;
}

/*######
## go_sacred_fire_of_life
######*/

enum
{
    NPC_ARIKARA = 10882,
};

bool GOHello_go_sacred_fire_of_life(Player* pPlayer, GameObject* pGO)
{
    if (pGO->GetGoType() == GAMEOBJECT_TYPE_GOOBER)
        pPlayer->SummonCreature(NPC_ARIKARA, -5008.338f, -2118.894f, 83.657f, 0.874f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

    return true;
}

/*######
## go_shrine_of_the_birds
######*/

enum
{
    NPC_HAWK_GUARD   = 22992,
    NPC_EAGLE_GUARD  = 22993,
    NPC_FALCON_GUARD = 22994,
    GO_SHRINE_HAWK   = 185551,
    GO_SHRINE_EAGLE  = 185547,
    GO_SHRINE_FALCON = 185553
};

bool GOHello_go_shrine_of_the_birds(Player* pPlayer, GameObject* pGo)
{
    uint32 uiBirdEntry = 0;

    float fX,fY,fZ;
    pGo->GetClosePoint(fX, fY, fZ, pGo->GetObjectBoundingRadius(), INTERACTION_DISTANCE);

    switch(pGo->GetEntry())
    {
        case GO_SHRINE_HAWK:
            uiBirdEntry = NPC_HAWK_GUARD;
            break;
        case GO_SHRINE_EAGLE:
            uiBirdEntry = NPC_EAGLE_GUARD;
            break;
        case GO_SHRINE_FALCON:
            uiBirdEntry = NPC_FALCON_GUARD;
            break;
    }

    if (uiBirdEntry)
        pPlayer->SummonCreature(uiBirdEntry, fX, fY, fZ, pGo->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);

    return false;
}

/*######
## go_tablet_of_madness
######*/

enum
{
    SPELL_ALCHEMY_GURUBASHI_MOJO_MADNESS = 24266,
    SPELL_LEARN_GURUBASHI_MOJO_MADNESS   = 24267
};

bool GOHello_go_tablet_of_madness(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->HasSkill(SKILL_ALCHEMY) && pPlayer->GetSkillValue(SKILL_ALCHEMY) >= 300 && !pPlayer->HasSpell(SPELL_ALCHEMY_GURUBASHI_MOJO_MADNESS))
        pPlayer->CastSpell(pPlayer, SPELL_LEARN_GURUBASHI_MOJO_MADNESS, false);

    return true;
}

/*######
## go_tablet_of_the_seven - OBSOLETE
######*/

//TODO: use gossip option ("Transcript the Tablet") instead, if Mangos adds support.
bool GOHello_go_tablet_of_the_seven(Player* pPlayer, GameObject* pGo)
{
    if (pGo->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
        return true;

    if (pPlayer->GetQuestStatus(4296) == QUEST_STATUS_INCOMPLETE)
        pPlayer->CastSpell(pPlayer, 15065, false);

    return true;
}

/*######
## go_tele_to_dalaran_crystal
######*/

enum
{
    QUEST_LEARN_LEAVE_RETURN = 12790,
    QUEST_TELE_CRYSTAL_FLAG  = 12845
};

bool GOHello_go_tele_to_dalaran_crystal(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestRewardStatus(QUEST_TELE_CRYSTAL_FLAG))
        return false;

    //TODO: must send error message (what kind of message? On-screen?)
    return true;
}

/*######
## go_tele_to_violet_stand
######*/

bool GOHello_go_tele_to_violet_stand(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestRewardStatus(QUEST_LEARN_LEAVE_RETURN) || pPlayer->GetQuestStatus(QUEST_LEARN_LEAVE_RETURN) == QUEST_STATUS_INCOMPLETE)
        return false;

    return true;
}

enum
{
    NPC_ZELEMAR_THE_WRATHFULL = 17830,
    SAY_AGGRO                 = -1000579
};

float Position[4] = {-327.99f, 221.74f, -20.31f, 3.87f};

bool GOHello_go_blood_filled_orb(Player* pPlayer, GameObject* pGo)
{
    if (Creature* pZelemar = pGo->SummonCreature(NPC_ZELEMAR_THE_WRATHFULL, Position[0], Position[1], Position[2], Position[3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
    {
        DoScriptText(SAY_AGGRO, pZelemar);
        pZelemar->AI()->AttackStart(pPlayer);
    }
    return false;
}

/*######
## go_andorhal_tower
######*/

enum
{
    QUEST_ALL_ALONG_THE_WATCHTOWERS_ALLIANCE = 5097,
    QUEST_ALL_ALONG_THE_WATCHTOWERS_HORDE    = 5098,
    NPC_ANDORHAL_TOWER_1                     = 10902,
    NPC_ANDORHAL_TOWER_2                     = 10903,
    NPC_ANDORHAL_TOWER_3                     = 10904,
    NPC_ANDORHAL_TOWER_4                     = 10905,
    GO_ANDORHAL_TOWER_1                      = 176094,
    GO_ANDORHAL_TOWER_2                      = 176095,
    GO_ANDORHAL_TOWER_3                      = 176096,
    GO_ANDORHAL_TOWER_4                      = 176097
};

bool GOHello_go_andorhal_tower(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_ALL_ALONG_THE_WATCHTOWERS_ALLIANCE) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_ALL_ALONG_THE_WATCHTOWERS_HORDE) == QUEST_STATUS_INCOMPLETE)
    {
        uint32 uiKillCredit = 0;
        switch(pGo->GetEntry())
        {
            case GO_ANDORHAL_TOWER_1:   uiKillCredit = NPC_ANDORHAL_TOWER_1;   break;
            case GO_ANDORHAL_TOWER_2:   uiKillCredit = NPC_ANDORHAL_TOWER_2;   break;
            case GO_ANDORHAL_TOWER_3:   uiKillCredit = NPC_ANDORHAL_TOWER_3;   break;
            case GO_ANDORHAL_TOWER_4:   uiKillCredit = NPC_ANDORHAL_TOWER_4;   break;
        }
        if (uiKillCredit)
            pPlayer->KilledMonsterCredit(uiKillCredit);
    }
    return true;
}

/*######
## go_scourge_enclosure
######*/

enum
{
    SPELL_GYMER_LOCK_EXPLOSION      = 55529,
    NPC_GYMER_LOCK_DUMMY            = 29928

};

bool GOHello_go_scourge_enclosure(Player* pPlayer, GameObject* pGo)
{
    std::list<Creature*> m_lResearchersList;
    GetCreatureListWithEntryInGrid(m_lResearchersList, pGo, NPC_GYMER_LOCK_DUMMY, 15.0f);
    if (!m_lResearchersList.empty())
    {
        for(std::list<Creature*>::iterator itr = m_lResearchersList.begin(); itr != m_lResearchersList.end(); ++itr)
        {
            (*itr)->CastSpell((*itr),SPELL_GYMER_LOCK_EXPLOSION,true);
        }
    }
    pPlayer->KilledMonsterCredit(NPC_GYMER_LOCK_DUMMY);
    return true;
}

void AddSC_go_scripts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_cat_figurine";
    pNewScript->pGOHello =          &GOHello_go_cat_figurine;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_northern_crystal_pylon";
    pNewScript->pGOHello =          &GOHello_go_northern_crystal_pylon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_eastern_crystal_pylon";
    pNewScript->pGOHello =          &GOHello_go_eastern_crystal_pylon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_western_crystal_pylon";
    pNewScript->pGOHello =          &GOHello_go_western_crystal_pylon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_barov_journal";
    pNewScript->pGOHello =          &GOHello_go_barov_journal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_ethereum_prison";
    pNewScript->pGOHello =          &GOHello_go_ethereum_prison;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_ethereum_stasis";
    pNewScript->pGOHello =          &GOHello_go_ethereum_stasis;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_field_repair_bot_74A";
    pNewScript->pGOHello =          &GOHello_go_field_repair_bot_74A;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_gilded_brazier";
    pNewScript->pGOHello =          &GOHello_go_gilded_brazier;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_jump_a_tron";
    pNewScript->pGOHello =          &GOHello_go_jump_a_tron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_mysterious_snow_mound";
    pNewScript->pGOHello =          &GOHello_go_mysterious_snow_mound;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_orb_of_command";
    pNewScript->pGOHello =          &GOHello_go_orb_of_command;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_resonite_cask";
    pNewScript->pGOHello =          &GOHello_go_resonite_cask;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_sacred_fire_of_life";
    pNewScript->pGOHello =          &GOHello_go_sacred_fire_of_life;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_shrine_of_the_birds";
    pNewScript->pGOHello =          &GOHello_go_shrine_of_the_birds;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_tablet_of_madness";
    pNewScript->pGOHello =          &GOHello_go_tablet_of_madness;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_tablet_of_the_seven";
    pNewScript->pGOHello =          &GOHello_go_tablet_of_the_seven;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_tele_to_dalaran_crystal";
    pNewScript->pGOHello =          &GOHello_go_tele_to_dalaran_crystal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_tele_to_violet_stand";
    pNewScript->pGOHello =          &GOHello_go_tele_to_violet_stand;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_blood_filled_orb";
    pNewScript->pGOHello =          &GOHello_go_blood_filled_orb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_andorhal_tower";
    pNewScript->pGOHello =          &GOHello_go_andorhal_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_scourge_enclosure";
    pNewScript->pGOHello =          &GOHello_go_scourge_enclosure;
    pNewScript->RegisterSelf();
}
