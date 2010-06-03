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
SDName: Instance_Dark_Portal
SD%Complete: 50
SDComment: Quest support: 9836, 10297. Currently in progress.
SDCategory: Caverns of Time, The Dark Portal
EndScriptData */

#include "precompiled.h"
#include "dark_portal.h"

inline uint32 RandRiftBoss() { return (urand(0, 1) ? NPC_RKEEP : NPC_RLORD); }

float PortalLocation[4][4]=
{
    {-2041.06f, 7042.08f, 29.99f, 1.30f},
    {-1968.18f, 7042.11f, 21.93f, 2.12f},
    {-1885.82f, 7107.36f, 22.32f, 3.07f},
    {-1928.11f, 7175.95f, 22.11f, 3.44f}
};

struct Wave
{
    uint32 PortalBoss;                                      //protector of current portal
    uint32 NextPortalTime;                                  //time to next portal, or 0 if portal boss need to be killed
};

static Wave RiftWaves[]=
{
    {RIFT_BOSS,  0},
    {NPC_DEJA,   0},
    {RIFT_BOSS,  120000},
    {NPC_TEMPO,  140000},
    {RIFT_BOSS,  120000},
    {NPC_AEONUS, 0}
};

struct MANGOS_DLL_DECL instance_dark_portal : public ScriptedInstance
{
    instance_dark_portal(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint32 m_uiRiftPortalCount;
    uint32 m_uiShieldPercent;
    uint8 m_uiRiftWaveCount;
    uint8 m_uiRiftWaveId;

    uint32 m_uiNextPortal_Timer;

    uint64 m_uiMedivhGUID;
    uint8 m_uiCurrentRiftId;

    void Initialize()
    {
        m_uiMedivhGUID = 0;
        Clear();
    }

    void Clear()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiRiftPortalCount  = 0;
        m_uiShieldPercent    = 100;
        m_uiRiftWaveCount    = 0;
        m_uiRiftWaveId       = 0;

        m_uiCurrentRiftId    = 0;

        m_uiNextPortal_Timer = 0;
    }

    void InitWorldState(bool Enable = true)
    {
        DoUpdateWorldState(WORLD_STATE_BM,Enable ? 1 : 0);
        DoUpdateWorldState(WORLD_STATE_BM_SHIELD, 100);
        DoUpdateWorldState(WORLD_STATE_BM_RIFT, 0);
    }

    bool IsEncounterInProgress() const
    {
        if (m_auiEncounter[0] == IN_PROGRESS)
            return true;

        return false;
    }

    void OnPlayerEnter(Player* pPlayer)
    {
        if (m_auiEncounter[0] == IN_PROGRESS)
            return;

        pPlayer->SendUpdateWorldState(WORLD_STATE_BM, 0);
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        if (pCreature->GetEntry() == NPC_MEDIVH)
            m_uiMedivhGUID = pCreature->GetGUID();
    }

    //what other conditions to check?
    bool CanProgressEvent()
    {
        if (instance->GetPlayers().isEmpty())
            return false;

        return true;
    }

    uint8 GetRiftWaveId()
    {
        switch(m_uiRiftPortalCount)
        {
            case 6:
                m_uiRiftWaveId = 2;
                return 1;
            case 12:
                m_uiRiftWaveId = 4;
                return 3;
            case 18:
                return 5;
            default:
                return m_uiRiftWaveId;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_MEDIVH:
                if (uiData == SPECIAL && m_auiEncounter[0] == IN_PROGRESS)
                {
                    --m_uiShieldPercent;

                    DoUpdateWorldState(WORLD_STATE_BM_SHIELD, m_uiShieldPercent);

                    if (!m_uiShieldPercent)
                    {
                        if (Creature* pMedivh = instance->GetCreature(m_uiMedivhGUID))
                        {
                            if (pMedivh->isAlive())
                            {
                                pMedivh->DealDamage(pMedivh, pMedivh->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                                m_auiEncounter[0] = FAIL;
                                m_auiEncounter[1] = NOT_STARTED;
                            }
                        }
                    }
                }
                else
                {
                    if (uiData == IN_PROGRESS)
                    {
                        debug_log("SD2: Instance Dark Portal: Starting event.");
                        InitWorldState();
                        m_auiEncounter[1] = IN_PROGRESS;
                        m_uiNextPortal_Timer = 15000;
                    }

                    if (uiData == DONE)
                    {
                        //this may be completed further out in the post-event
                        debug_log("SD2: Instance Dark Portal: Event completed.");

                        Map::PlayerList const& players = instance->GetPlayers();

                        if (!players.isEmpty())
                        {
                            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* pPlayer = itr->getSource())
                                {
                                    if (pPlayer->GetQuestStatus(QUEST_OPENING_PORTAL) == QUEST_STATUS_INCOMPLETE)
                                        pPlayer->AreaExploredOrEventHappens(QUEST_OPENING_PORTAL);

                                    if (pPlayer->GetQuestStatus(QUEST_MASTER_TOUCH) == QUEST_STATUS_INCOMPLETE)
                                        pPlayer->AreaExploredOrEventHappens(QUEST_MASTER_TOUCH);
                                }
                            }
                        }
                    }

                    m_auiEncounter[0] = uiData;
                }
                break;
            case TYPE_RIFT:
                if (uiData == SPECIAL)
                {
                    if (m_uiRiftPortalCount < 7)
                        m_uiNextPortal_Timer = 5000;
                }
                else
                    m_auiEncounter[1] = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_MEDIVH:
                return m_auiEncounter[0];
            case TYPE_RIFT:
                return m_auiEncounter[1];
            case DATA_PORTAL_COUNT:
                return m_uiRiftPortalCount;
            case DATA_SHIELD:
                return m_uiShieldPercent;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        if (uiData == DATA_MEDIVH)
            return m_uiMedivhGUID;

        return 0;
    }

    Creature* SummonedPortalBoss(Creature* pSource)
    {
        uint32 uiEntry = RiftWaves[GetRiftWaveId()].PortalBoss;

        if (uiEntry == RIFT_BOSS)
            uiEntry = RandRiftBoss();

        float x,y,z;
        pSource->GetRandomPoint(pSource->GetPositionX(),pSource->GetPositionY(),pSource->GetPositionZ(),10.0f,x,y,z);
        //normalize Z-level if we can, if rift is not at ground level.
        z = std::max(instance->GetHeight(x, y, MAX_HEIGHT), instance->GetWaterLevel(x, y));

        debug_log("SD2: Instance Dark Portal: Summoning rift boss uiEntry %u.",uiEntry);

        if (Creature* pSummoned = pSource->SummonCreature(uiEntry,x,y,z,pSource->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000))
            return pSummoned;

        debug_log("SD2: Instance Dark Portal: what just happened there? No boss, no loot, no fun...");
        return NULL;
    }

    void DoSpawnPortal()
    {
        if (Creature* pMedivh = instance->GetCreature(m_uiMedivhGUID))
        {
            uint8 uiTmp = urand(0, 2);

            if (uiTmp >= m_uiCurrentRiftId)
                ++uiTmp;

            debug_log("SD2: Instance Dark Portal: Creating Time Rift at locationId %i (old locationId was %u).", uiTmp, m_uiCurrentRiftId);

            m_uiCurrentRiftId = uiTmp;

            if (Creature* pTemp = pMedivh->SummonCreature(NPC_TIME_RIFT, PortalLocation[uiTmp][0], PortalLocation[uiTmp][1], PortalLocation[uiTmp][2], PortalLocation[uiTmp][3], TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                if (Creature* pBoss = SummonedPortalBoss(pTemp))
                {
                    if (pBoss->GetEntry() == NPC_AEONUS)
                        pBoss->AddThreat(pMedivh);
                    else
                    {
                        pBoss->AddThreat(pTemp);
                        pTemp->CastSpell(pBoss, SPELL_RIFT_CHANNEL, false);
                    }
                }
            }
        }
    }

    void Update(uint32 uiDiff)
    {
        if (m_auiEncounter[1] != IN_PROGRESS)
            return;

        //add delay timer?
        if (!CanProgressEvent())
        {
            Clear();
            return;
        }

        if (m_uiNextPortal_Timer)
        {
            if (m_uiNextPortal_Timer <= uiDiff)
            {
                ++m_uiRiftPortalCount;

                DoUpdateWorldState(WORLD_STATE_BM_RIFT, m_uiRiftPortalCount);

                DoSpawnPortal();
                m_uiNextPortal_Timer = RiftWaves[GetRiftWaveId()].NextPortalTime;
            }
            else
                m_uiNextPortal_Timer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_dark_portal(Map* pMap)
{
    return new instance_dark_portal(pMap);
}

void AddSC_instance_dark_portal()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_dark_portal";
    newscript->GetInstanceData = &GetInstanceData_instance_dark_portal;
    newscript->RegisterSelf();
}
