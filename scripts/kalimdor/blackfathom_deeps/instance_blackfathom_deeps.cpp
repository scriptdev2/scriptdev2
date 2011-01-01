/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Blackfathom_Deeps
SD%Complete: 50
SDComment:
SDCategory: Blackfathom Deeps
EndScriptData */

#include "precompiled.h"
#include "blackfathom_deeps.h"

/* Encounter 0 = Twilight Lord Kelris
   Encounter 1 = Shrine event
   Must kill twilight lord for shrine event to be possible
 */

struct MANGOS_DLL_DECL instance_blackfathom_deeps : public ScriptedInstance
{
    instance_blackfathom_deeps(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint64 m_uiKelrisGUID;
    uint64 m_uiShrineOfGelihastGUID;
    uint64 m_uiAltarOfTheDeepsGUID;
    uint64 m_uiPortalGUID;
    uint32 m_uiSpawnServantTimer;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiKelrisGUID = 0;
        m_uiShrineOfGelihastGUID = 0;
        m_uiAltarOfTheDeepsGUID = 0;
        m_uiPortalGUID = 0;
        m_uiSpawnServantTimer = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_KELRIS)
            m_uiKelrisGUID = pCreature->GetGUID();
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_PORTAL_DOOR:
                if (m_auiEncounter[1] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);

                m_uiPortalGUID = pGo->GetGUID();
                break;
            case GO_SHRINE_1:
                if (m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SHRINE_2:
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SHRINE_3:
                if (m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SHRINE_4:
                if (m_auiEncounter[5] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;

            case 103015:    m_uiShrineOfGelihastGUID = pGo->GetGUID();  break;
            case 103016:    m_uiAltarOfTheDeepsGUID = pGo->GetGUID();   break;
            
        }
    }

    bool CanOpenEndDoor()
    {
        if (m_auiEncounter[0] != DONE)
            return false;

        if (m_auiEncounter[2] == DONE && m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE)
            return true;

        return false;
    }

    void SpawnServants()
    {
        if (Creature* pKelris = instance->GetCreature(m_uiKelrisGUID))
        {
            float fX_resp, fY_resp, fZ_resp;
            pKelris->GetRespawnCoord(fX_resp, fY_resp, fZ_resp);

            for(uint8 i = 0; i < 5 ; ++i)
            {
                // this part gets a random position at circumference point in a circle
                // fRadius is how far from center to calculate.
                // here we use kelris's close point coords as base and then move the summoned to the location of his respawn coords
                float fRadius = 30.0f;
                float fAngle = 2.0 * M_PI * rand_norm();

                float fX, fY, fZ;

                fRadius *= sqrt(rand_norm());

                pKelris->GetClosePoint(fX, fY, fZ, 0.0f, fRadius, fAngle);

                if (Creature* pServant = pKelris->SummonCreature(NPC_SERVANT, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 300000))
                    pServant->GetMotionMaster()->MovePoint(0, fX_resp, fY_resp, fZ_resp);
            }

        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_KELRIS:                               // eventAI must set instance data (1,3) at his death
                if (m_auiEncounter[0] != DONE && uiData == DONE)
                    m_auiEncounter[0] = uiData;
                break;
            case TYPE_SHRINE:
            {
                switch(uiData)
                {
                    case GO_SHRINE_1:
                        m_auiEncounter[2] = DONE;
                        break;
                    case GO_SHRINE_2:
                        m_auiEncounter[3] = DONE;
                        break;
                    case GO_SHRINE_3:
                        m_auiEncounter[4] = DONE;
                        break;
                    case GO_SHRINE_4:
                        m_auiEncounter[5] = DONE;
                        break;
                }

                m_uiSpawnServantTimer = 7500;

                if (CanOpenEndDoor())
                {
                    m_auiEncounter[1] = DONE;
                    DoUseDoorOrButton(m_uiPortalGUID);
                }

                break;
            }
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_KELRIS:
                return m_auiEncounter[0];
            case TYPE_SHRINE:
                return m_auiEncounter[1];
        }

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_TWILIGHT_LORD_KELRIS:
                return m_uiKelrisGUID;
            case DATA_SHRINE_OF_GELIHAST:
                return m_uiShrineOfGelihastGUID;
        }

        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if (m_uiSpawnServantTimer)
        {
            if (m_uiSpawnServantTimer <= uiDiff)
            {
                SpawnServants();
                m_uiSpawnServantTimer = 0;
            }
            else
                m_uiSpawnServantTimer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_blackfathom_deeps(Map* pMap)
{
    return new instance_blackfathom_deeps(pMap);
}

bool GOUse_go_fire_of_akumai(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_KELRIS) == DONE)
    {
        pInstance->SetData(TYPE_SHRINE, pGo->GetEntry());
        return false;
    }

    return true;
}

void AddSC_instance_blackfathom_deeps()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_blackfathom_deeps";
    newscript->GetInstanceData = &GetInstanceData_instance_blackfathom_deeps;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_fire_of_akumai";
    newscript->pGOUse = &GOUse_go_fire_of_akumai;
    newscript->RegisterSelf();
}
