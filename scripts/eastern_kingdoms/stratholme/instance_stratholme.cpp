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
SDName: Instance_Stratholme
SD%Complete: 50
SDComment: In progress. Undead side 80% implemented, wipe support for doors at slaughterhouse needed, event needs better implementation
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

instance_stratholme::instance_stratholme(Map* pMap) : ScriptedInstance(pMap),
    m_uiBaronRunTimer(0),
    m_uiSlaugtherSquareTimer(0),

    m_uiServiceEntranceGUID(0),
    m_uiGauntletGate1GUID(0),
    m_uiPortGauntletGUID(0),
    m_uiPortSlaugtherGUID(0),
    m_uiPortElderGUID(0),
    m_auiRamsteinDoorGUID(0),
    m_auiRivendareDoorGUID(0),

    m_uiBaronGUID(0),
    m_uiYsidaTriggerGUID(0),

    m_uiAcolyteAnnouncerGUID(0)
{
    Initialize();
}


void instance_stratholme::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_bIsSilverHandDead, false, sizeof(m_bIsSilverHandDead));
    memset(&m_auiZigguratGUID, 0, sizeof(m_auiZigguratGUID));
    memset(&m_auiCrystalSortedGUID, 0, sizeof(m_auiCrystalSortedGUID));

    m_luiCrystalGUIDs.clear();
    m_sAbomnationGUID.clear();
    m_luiAcolyteGUIDs.clear();
}

bool instance_stratholme::StartSlaugtherSquare()
{
    if (m_auiEncounter[1] == SPECIAL && m_auiEncounter[2] == SPECIAL && m_auiEncounter[3] == SPECIAL)
    {
        if (Creature* pBaron = instance->GetCreature(m_uiBaronGUID))
            DoScriptText(SAY_ANNOUNCE_RIVENDARE, pBaron);

        DoUseDoorOrButton(m_uiPortGauntletGUID);
        DoUseDoorOrButton(m_uiPortSlaugtherGUID);
        return true;
    }

    debug_log("SD2: Instance Stratholme: Cannot open slaugther square yet.");
    return false;
}

void instance_stratholme::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_BARON:            m_uiBaronGUID = pCreature->GetGUID();                break;
        case NPC_YSIDA_TRIGGER:    m_uiYsidaTriggerGUID = pCreature->GetGUID();         break;
        case NPC_CRYSTAL:          m_luiCrystalGUIDs.push_back(pCreature->GetGUID());   break;
        case NPC_ABOM_BILE:
        case NPC_ABOM_VENOM:       m_sAbomnationGUID.insert(pCreature->GetGUID());      break;
        case NPC_THUZADIN_ACOLYTE: m_luiAcolyteGUIDs.push_back(pCreature->GetGUID());   break;
    }
}

void instance_stratholme::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_SERVICE_ENTRANCE:
            m_uiServiceEntranceGUID = pGo->GetGUID();
            break;
        case GO_GAUNTLET_GATE1:
            // TODO
            //weird, but unless flag is set, client will not respond as expected. DB bug?
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_LOCKED);
            m_uiGauntletGate1GUID = pGo->GetGUID();
            break;
        case GO_ZIGGURAT_DOOR_1:
            m_auiZigguratGUID[0] = pGo->GetGUID();
            if (m_auiEncounter[1] == DONE || m_auiEncounter[1] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ZIGGURAT_DOOR_2:
            m_auiZigguratGUID[1] = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE || m_auiEncounter[2] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ZIGGURAT_DOOR_3:
            m_auiZigguratGUID[2] = pGo->GetGUID();
            if (m_auiEncounter[3] == DONE || m_auiEncounter[3] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ZIGGURAT_DOOR_4:
            m_auiRamsteinDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ZIGGURAT_DOOR_5:
            m_auiRivendareDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORT_GAUNTLET:
            m_uiPortGauntletGUID = pGo->GetGUID();
            if (m_auiEncounter[1] == SPECIAL && m_auiEncounter[2] == SPECIAL && m_auiEncounter[3] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORT_SLAUGTHER:
            m_uiPortSlaugtherGUID = pGo->GetGUID();
            if (m_auiEncounter[1] == SPECIAL && m_auiEncounter[2] == SPECIAL && m_auiEncounter[3] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORT_ELDERS:
            m_uiPortElderGUID = pGo->GetGUID();
            break;
    }
}

void instance_stratholme::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_BARON_RUN:
            switch(uiData)
            {
                case IN_PROGRESS:
                    if (m_auiEncounter[0] == IN_PROGRESS || m_auiEncounter[0] == FAIL)
                        break;
                    m_uiBaronRunTimer = 45*MINUTE*IN_MILLISECONDS;
                    debug_log("SD2: Instance Stratholme: Baron run in progress.");
                    break;
                case FAIL:
                    //may add code to remove aura from players, but in theory the time should be up already and removed.
                    break;
                case DONE:
                    if (Creature* pYsidaT = instance->GetCreature(m_uiYsidaTriggerGUID))
                        pYsidaT->SummonCreature(NPC_YSIDA, pYsidaT->GetPositionX(), pYsidaT->GetPositionY(), pYsidaT->GetPositionZ(), pYsidaT->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 1800000);

                    m_uiBaronRunTimer = 0;
                    break;
            }
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_BARONESS:
            m_auiEncounter[1] = uiData;
            if (uiData == DONE)
            {
                DoSortZiggurats();
                DoUseDoorOrButton(m_auiZigguratGUID[0]);
            }
            if (uiData == SPECIAL)
                StartSlaugtherSquare();
            break;
        case TYPE_NERUB:
            m_auiEncounter[2] = uiData;
            if (uiData == DONE)
            {
                DoSortZiggurats();
                DoUseDoorOrButton(m_auiZigguratGUID[1]);
            }
            if (uiData == SPECIAL)
                StartSlaugtherSquare();
            break;
        case TYPE_PALLID:
            m_auiEncounter[3] = uiData;
            if (uiData == DONE)
            {
                DoSortZiggurats();
                DoUseDoorOrButton(m_auiZigguratGUID[2]);
            }
            if (uiData == SPECIAL)
                StartSlaugtherSquare();
            break;
        case TYPE_RAMSTEIN:
            if (uiData == IN_PROGRESS)
            {
                if (m_auiEncounter[4] != IN_PROGRESS)
                    DoUseDoorOrButton(m_uiPortGauntletGUID);

                uint32 uiCount = m_sAbomnationGUID.size();
                for(std::set<uint64>::iterator i = m_sAbomnationGUID.begin(); i != m_sAbomnationGUID.end(); ++i)
                {
                    if (Creature* pAbom = instance->GetCreature(*i))
                    {
                        if (!pAbom->isAlive())
                            --uiCount;
                    }
                }

                if (!uiCount)
                {
                    //a bit itchy, it should close the door after 10 secs, but it doesn't. skipping it for now.
                    // TODO - not working correctly!
                    //DoUseDoorOrButton(m_auiRamsteinDoorGUID, 10);

                    if (Creature* pBaron = instance->GetCreature(m_uiBaronGUID))
                        pBaron->SummonCreature(NPC_RAMSTEIN, 4032.84f, -3390.24f, 119.73f, 4.71f, TEMPSUMMON_DEAD_DESPAWN, 0);

                    debug_log("SD2: Instance Stratholme: Ramstein spawned.");
                }
                else
                    debug_log("SD2: Instance Stratholme: %u Abomnation left to kill.", uiCount);
            }
            if (uiData == DONE)
            {
                m_uiSlaugtherSquareTimer = 5*MINUTE*IN_MILLISECONDS;
                debug_log("SD2: Instance Stratholme: Slaugther event will continue in 5 minutes.");
            }
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_BARON:
            if (uiData == IN_PROGRESS)
            {
                if (GetData(TYPE_BARON_RUN) == IN_PROGRESS)
                {
                    Map::PlayerList const& players = instance->GetPlayers();

                    if (!players.isEmpty())
                    {
                        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            if (Player* pPlayer = itr->getSource())
                            {
                                if (pPlayer->HasAura(SPELL_BARON_ULTIMATUM))
                                    pPlayer->RemoveAurasDueToSpell(SPELL_BARON_ULTIMATUM);

                                if (pPlayer->GetQuestStatus(QUEST_DEAD_MAN_PLEA) == QUEST_STATUS_INCOMPLETE)
                                    pPlayer->AreaExploredOrEventHappens(QUEST_DEAD_MAN_PLEA);
                            }
                        }
                    }

                    SetData(TYPE_BARON_RUN,DONE);
                }
            }
            if (uiData == DONE)
                // Open a few doors again                   // TODO: needs research, how to handle wipes in this area!
                DoUseDoorOrButton(m_uiPortGauntletGUID);

            m_auiEncounter[5] = uiData;
            break;

        case TYPE_SH_AELMAR:
            m_bIsSilverHandDead[0] = (uiData) ? true : false;
            break;
        case TYPE_SH_CATHELA:
            m_bIsSilverHandDead[1] = (uiData) ? true : false;
            break;
        case TYPE_SH_GREGOR:
            m_bIsSilverHandDead[2] = (uiData) ? true : false;
            break;
        case TYPE_SH_NEMAS:
            m_bIsSilverHandDead[3] = (uiData) ? true : false;
            break;
        case TYPE_SH_VICAR:
            m_bIsSilverHandDead[4] = (uiData) ? true : false;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_stratholme::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    // Special Treatment for the Ziggurat-Bosses, as otherwise the event couldn't reload
    if (m_auiEncounter[1] == DONE)
        m_auiEncounter[1] = SPECIAL;
    if (m_auiEncounter[2] == DONE)
        m_auiEncounter[2] = SPECIAL;
    if (m_auiEncounter[3] == DONE)
        m_auiEncounter[3] = SPECIAL;

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_stratholme::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_SH_QUEST:
            if (m_bIsSilverHandDead[0] && m_bIsSilverHandDead[1] && m_bIsSilverHandDead[2] && m_bIsSilverHandDead[3] && m_bIsSilverHandDead[4])
                return 1;
            return 0;
        case TYPE_BARON_RUN:
            return m_auiEncounter[0];
        case TYPE_BARONESS:
            return m_auiEncounter[1];
        case TYPE_NERUB:
            return m_auiEncounter[2];
        case TYPE_PALLID:
            return m_auiEncounter[3];
        case TYPE_RAMSTEIN:
            return m_auiEncounter[4];
        case TYPE_BARON:
            return m_auiEncounter[5];
        default:
            return 0;
    }
}

uint64 instance_stratholme::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case NPC_BARON:
            return m_uiBaronGUID;
        case NPC_YSIDA_TRIGGER:
            return m_uiYsidaTriggerGUID;
        default:
            return 0;
    }
}

static bool sortByHeight(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionZ() > pSecond->GetPositionZ();
}

void instance_stratholme::DoSortZiggurats()
{
    if (m_luiAcolyteGUIDs.empty())
        return;

    std::list<Creature*> lAcolytes;                         // Valid pointers, only used locally
    for (std::list<uint64>::const_iterator itr = m_luiAcolyteGUIDs.begin(); itr != m_luiAcolyteGUIDs.end(); itr++)
    {
        if (Creature* pAcolyte = instance->GetCreature(*itr))
            lAcolytes.push_back(pAcolyte);
    }
    m_luiAcolyteGUIDs.clear();

    if (lAcolytes.empty())
        return;

    if (!m_uiAcolyteAnnouncerGUID)
    {
        // Sort the acolytes by height, and the one with the biggest height is the announcer (a bit outside the map)
        lAcolytes.sort(sortByHeight);
        m_uiAcolyteAnnouncerGUID = (*lAcolytes.begin())->GetGUID();
        lAcolytes.erase(lAcolytes.begin());
    }

    // Sort Acolytes
    for (std::list<Creature*>::iterator itr = lAcolytes.begin(); itr != lAcolytes.end(); )
    {
        bool bAlreadyIterated = false;
        for (uint8 i = 0; i < MAX_ZIGGURATS; ++i)
        {
            if (GameObject* pZigguratDoor = instance->GetGameObject(m_auiZigguratGUID[i]))
            {
                if ((*itr)->isAlive() && (*itr)->IsWithinDistInMap(pZigguratDoor, 30.0f, false))
                {
                    m_alZigguratAcolyteGUID[i].push_back((*itr)->GetGUID());
                    itr = lAcolytes.erase(itr);
                    bAlreadyIterated = true;
                    break;
                }
            }
        }

        if (itr != lAcolytes.end() && !bAlreadyIterated)
            ++itr;
    }

    // In case some mobs have not been able to be sorted, store their GUIDs again
    for (std::list<Creature*>::const_iterator itr = lAcolytes.begin(); itr != lAcolytes.end(); itr++)
        m_luiAcolyteGUIDs.push_back((*itr)->GetGUID());

    // Sort Crystal
    for (std::list<uint64>::const_iterator itr = m_luiCrystalGUIDs.begin(); itr != m_luiCrystalGUIDs.end(); )
    {
        Creature* pCrystal = instance->GetCreature(*itr);
        if (!pCrystal)
        {
            itr = m_luiCrystalGUIDs.erase(itr);
            continue;
        }

        bool bAlreadyIterated = false;
        for (uint8 i = 0; i < MAX_ZIGGURATS; ++i)
        {
            if (GameObject* pZigguratDoor = instance->GetGameObject(m_auiZigguratGUID[i]))
            {
                if (pCrystal->IsWithinDistInMap(pZigguratDoor, 50.0f, false))
                {
                    m_auiCrystalSortedGUID[i] = *itr;
                    itr = m_luiCrystalGUIDs.erase(itr);
                    bAlreadyIterated = true;
                    break;
                }
            }
        }

        if (itr != m_luiCrystalGUIDs.end() && !bAlreadyIterated)
            ++itr;
    }
}

void instance_stratholme::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_THUZADIN_ACOLYTE)
    {
        for (uint8 i = 0; i < MAX_ZIGGURATS; ++i)
        {
            if (m_alZigguratAcolyteGUID[i].empty())
                continue;                               // nothing to do anymore for this ziggurat

            m_alZigguratAcolyteGUID[i].remove(pCreature->GetGUID());
            if (m_alZigguratAcolyteGUID[i].empty())
            {
                // A random zone yell after one is cleared
                int32 aAnnounceSay[MAX_ZIGGURATS] = {SAY_ANNOUNCE_ZIGGURAT_1, SAY_ANNOUNCE_ZIGGURAT_2, SAY_ANNOUNCE_ZIGGURAT_3};
                if (Creature* pAnnouncer = instance->GetCreature(m_uiAcolyteAnnouncerGUID))
                    DoScriptText(aAnnounceSay[i], pAnnouncer);

                // Kill Crystal
                if (Creature* pCrystal = instance->GetCreature(m_auiCrystalSortedGUID[i]))
                    pCrystal->DealDamage(pCrystal, pCrystal->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                switch (i)
                {
                    case 0: SetData(TYPE_BARONESS, SPECIAL); break;
                    case 1: SetData(TYPE_NERUB, SPECIAL);    break;
                    case 2: SetData(TYPE_PALLID, SPECIAL);   break;
                }
            }
        }
    }
}

void instance_stratholme::Update(uint32 uiDiff)
{
    if (m_uiBaronRunTimer)
    {
        if (m_uiBaronRunTimer <= uiDiff)
        {
            if (GetData(TYPE_BARON_RUN) != DONE)
                SetData(TYPE_BARON_RUN, FAIL);

            m_uiBaronRunTimer = 0;
            debug_log("SD2: Instance Stratholme: Baron run event reached end. Event has state %u.",GetData(TYPE_BARON_RUN));
        }
        else
            m_uiBaronRunTimer -= uiDiff;
    }

    if (m_uiSlaugtherSquareTimer)
    {
        if (m_uiSlaugtherSquareTimer <= uiDiff)
        {
            if (Creature* pBaron = instance->GetCreature(m_uiBaronGUID))
            {
                for(uint8 i = 0; i < 4; ++i)
                    pBaron->SummonCreature(NPC_BLACK_GUARD, 4032.84f, -3390.24f, 119.73f, 4.71f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000);

                DoUseDoorOrButton(m_auiRamsteinDoorGUID);
                DoUseDoorOrButton(m_auiRivendareDoorGUID);

                debug_log("SD2: Instance Stratholme: Black guard sentries spawned. Opening gates to baron.");
            }
            m_uiSlaugtherSquareTimer = 0;
        }
        else
            m_uiSlaugtherSquareTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_stratholme(Map* pMap)
{
    return new instance_stratholme(pMap);
}

void AddSC_instance_stratholme()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_stratholme";
    pNewScript->GetInstanceData = &GetInstanceData_instance_stratholme;
    pNewScript->RegisterSelf();
}
