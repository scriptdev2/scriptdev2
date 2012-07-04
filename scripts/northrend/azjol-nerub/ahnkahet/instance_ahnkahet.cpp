/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_ahnkahet
SD%Complete: 75
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

instance_ahnkahet::instance_ahnkahet(Map* pMap) : ScriptedInstance(pMap),
    m_bRespectElders(false),
    m_bVolunteerWork(false),
    m_uiInitiatesKilled(0),
    m_uiDevicesActivated(0)
{
    Initialize();
}

void instance_ahnkahet::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_ahnkahet::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_ELDER_NADOX:
        case NPC_TALDARAM:
        case NPC_JEDOGA_SHADOWSEEKER:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_AHNKAHAR_GUARDIAN_EGG:
            m_GuardianEggList.push_back(pCreature->GetObjectGuid());
            break;
        case NPC_AHNKAHAR_SWARM_EGG:
            m_SwarmerEggList.push_back(pCreature->GetObjectGuid());
            break;
        case NPC_JEDOGA_CONTROLLER:
            // Sort the controllers based on their purpose
            if (pCreature->GetPositionZ() > 30.0f)
                // Used for Taldaram visual
                m_lJedogaControllersGuidList.push_back(pCreature->GetObjectGuid());
            else if (pCreature->GetPositionZ() > 20.0f)
                // Used for Jedoga visual
                m_lJedogaEventControllersGuidList.push_back(pCreature->GetObjectGuid());
            else if (pCreature->GetPositionZ() < -16.0f)
                // Used for Jedoga sacrifice
                m_jedogaSacrificeController = pCreature->GetObjectGuid();
            break;
    }
}

void instance_ahnkahet::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_TALDARAM:
            if (m_auiEncounter[TYPE_TALDARAM] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_VORTEX:
            if (m_auiEncounter[TYPE_TALDARAM] == SPECIAL)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        case GO_ANCIENT_DEVICE_L:
        case GO_ANCIENT_DEVICE_R:
            if (m_auiEncounter[TYPE_NADOX] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_ahnkahet::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: Instance Ahn'Kahet: SetData received for type %u with data %u", uiType, uiData);

    switch(uiType)
    {
        case TYPE_NADOX:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
                m_bRespectElders = true;
            if (uiData == SPECIAL)
                m_bRespectElders = false;
            if (uiData == DONE)
            {
                DoToggleGameObjectFlags(GO_ANCIENT_DEVICE_L, GO_FLAG_NO_INTERACT, false);
                DoToggleGameObjectFlags(GO_ANCIENT_DEVICE_R, GO_FLAG_NO_INTERACT, false);
            }
            break;
        case TYPE_TALDARAM:
            if (uiData == SPECIAL)
            {
                ++m_uiDevicesActivated;

                if (m_uiDevicesActivated == 2)
                {
                    m_auiEncounter[uiType] = uiData;
                    DoUseDoorOrButton(GO_VORTEX);

                    // Lower Taldaram
                    if (Creature* pTaldaram = GetSingleCreatureFromStorage(NPC_TALDARAM))
                        pTaldaram->GetMotionMaster()->MovePoint(1, aTaldaramLandingLoc[0], aTaldaramLandingLoc[1], aTaldaramLandingLoc[2]);

                    // Interrupt the channeling
                    for (GuidList::const_iterator itr = m_lJedogaControllersGuidList.begin(); itr != m_lJedogaControllersGuidList.end(); ++itr)
                    {
                        if (Creature* pTemp = instance->GetCreature(*itr))
                            pTemp->InterruptNonMeleeSpells(false);
                    }
                }
            }
            if (uiData == DONE)
            {
                m_auiEncounter[uiType] = uiData;
                DoUseDoorOrButton(GO_DOOR_TALDARAM);
            }
            break;
        case TYPE_JEDOGA:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
                m_bVolunteerWork = true;
            if (uiData == SPECIAL)
                m_bVolunteerWork = false;
            if (uiData == FAIL)
                m_uiInitiatesKilled = 0;
            break;
        case TYPE_AMANITAR:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_VOLAZJ:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_VOLAZJ_ID);
            break;

        default:
            error_log("SD2: Instance Ahn'Kahet: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            break;
    }

    // For some encounters Special data needs to be saved
    if (uiData == DONE || uiData == SPECIAL)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3]
            << " " << m_auiEncounter[4];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_ahnkahet::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_TWILIGHT_INITIATE)
    {
        ++m_uiInitiatesKilled;

        // If all initiates are killed, then land Jedoga and stop the channeling
        if (m_uiInitiatesKilled == MAX_INITIATES)
        {
            if (Creature* pJedoga = GetSingleCreatureFromStorage(NPC_JEDOGA_SHADOWSEEKER))
                pJedoga->GetMotionMaster()->MovePoint(1, aJedogaLandingLoc[0], aJedogaLandingLoc[1], aJedogaLandingLoc[2]);

            for (GuidList::const_iterator itr = m_lJedogaEventControllersGuidList.begin(); itr != m_lJedogaEventControllersGuidList.end(); ++itr)
            {
                if (Creature* pTemp = instance->GetCreature(*itr))
                    pTemp->InterruptNonMeleeSpells(false);
            }
        }
    }
}

ObjectGuid instance_ahnkahet::SelectRandomGuardianEggGuid()
{
    if (m_GuardianEggList.empty())
        return ObjectGuid();

    GuidList::iterator iter = m_GuardianEggList.begin();
    advance(iter, urand(0, m_GuardianEggList.size() - 1));

    return *iter;
}

ObjectGuid instance_ahnkahet::SelectRandomSwarmerEggGuid()
{
    if (m_SwarmerEggList.empty())
        return ObjectGuid();

    GuidList::iterator iter = m_SwarmerEggList.begin();
    advance(iter, urand(0, m_SwarmerEggList.size() - 1));

    return *iter;
}

bool instance_ahnkahet::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_RESPECT_ELDERS:
            return m_bRespectElders;
        case ACHIEV_CRIT_VOLUNTEER_WORK:
            return m_bVolunteerWork;

        default:
            return false;
    }
}

void instance_ahnkahet::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_ahnkahet::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

InstanceData* GetInstanceData_instance_ahnkahet(Map* pMap)
{
    return new instance_ahnkahet(pMap);
}

void AddSC_instance_ahnkahet()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ahnkahet";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ahnkahet;
    pNewScript->RegisterSelf();
}
