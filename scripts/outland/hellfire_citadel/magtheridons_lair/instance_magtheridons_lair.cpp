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
SDName: Instance_Magtheridons_Lair
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "precompiled.h"
#include "magtheridons_lair.h"

struct MANGOS_DLL_DECL instance_magtheridons_lair : public ScriptedInstance
{
    instance_magtheridons_lair(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiMagtheridonGUID;
    std::set<uint64> ChannelerGUID;
    uint64 m_uiDoorGUID;
    std::set<uint64> ColumnGUID;

    uint32 m_uiCageTimer;
    uint32 m_uiRespawnTimer;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        ChannelerGUID.clear();
        ColumnGUID.clear();

        m_uiMagtheridonGUID = 0;
        m_uiDoorGUID = 0;

        m_uiCageTimer = 0;
        m_uiRespawnTimer = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_MAGTHERION: m_uiMagtheridonGUID = pCreature->GetGUID(); break;
            case NPC_CHANNELER: ChannelerGUID.insert(pCreature->GetGUID()); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case 181713:
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
            case 183847:                                    //event door
                m_uiDoorGUID = pGo->GetGUID();
                break;
            case 184653:                                    // hall
            case 184634:                                    // six columns
            case 184635:
            case 184636:
            case 184637:
            case 184638:
            case 184639:
                ColumnGUID.insert(pGo->GetGUID());
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_MAGTHERIDON_EVENT:
                m_auiEncounter[0] = uiData;
                if (uiData == NOT_STARTED)
                    m_uiRespawnTimer = 10000;
                if (uiData != IN_PROGRESS)
                {
                    if (GameObject* pDoor = instance->GetGameObject(m_uiDoorGUID))
                        pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                break;
            case TYPE_CHANNELER_EVENT:
                switch(uiData)
                {
                    case NOT_STARTED:                       // Reset all channelers once one is reset.
                        if (m_auiEncounter[1] != NOT_STARTED)
                        {
                            m_auiEncounter[1] = NOT_STARTED;

                            if (ChannelerGUID.empty())
                            {
                                debug_log("SD2: Instance Magtheridon: Channeler GUID list are empty.");
                                break;
                            }

                            for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                            {
                                if (Creature* pChanneler = instance->GetCreature(*i))
                                {
                                    if (pChanneler->isAlive())
                                        pChanneler->AI()->EnterEvadeMode();
                                    else
                                        pChanneler->Respawn();
                                }
                            }

                            m_uiCageTimer = 0;

                            if (GameObject* pDoor = instance->GetGameObject(m_uiDoorGUID))
                                pDoor->SetGoState(GO_STATE_ACTIVE);
                        }
                        break;
                    case IN_PROGRESS:                       // Event start.
                        if (m_auiEncounter[1] != IN_PROGRESS)
                        {
                            m_auiEncounter[1] = IN_PROGRESS;

                            // Let all five channelers aggro.
                            for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                            {
                                Creature* pChanneler = instance->GetCreature(*i);

                                if (pChanneler && pChanneler->isAlive())
                                    AttackNearestTarget(pChanneler);
                            }

                            // Magtheridon breaks free after two minutes.
                            Creature* pMagtheridon = instance->GetCreature(m_uiMagtheridonGUID);

                            if (pMagtheridon && pMagtheridon->isAlive())
                                m_uiCageTimer = 120000;

                            if (GameObject* pDoor = instance->GetGameObject(m_uiDoorGUID))
                                pDoor->SetGoState(GO_STATE_READY);
                        }
                        break;
                    case DONE:                              // Add buff and check if all channelers are dead.
                        for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                        {
                            Creature* pChanneler = instance->GetCreature(*i);

                            if (pChanneler && pChanneler->isAlive())
                            {
                                //Channeler->InterruptNonMeleeSpells(false);
                                //Channeler->CastSpell(Channeler, SPELL_SOUL_TRANSFER, false);
                                uiData = IN_PROGRESS;
                                break;
                            }
                        }
                        break;
                }
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_HALL_COLLAPSE:
                // IN_PROGRESS - collapse / NOT_STARTED - reset
                for(std::set<uint64>::iterator i = ColumnGUID.begin(); i != ColumnGUID.end(); ++i)
                {
                    DoUseDoorOrButton(*i);
                }
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        if (uiType == TYPE_MAGTHERIDON_EVENT)
            return m_auiEncounter[0];
        if (uiType == TYPE_CHANNELER_EVENT)
            return m_auiEncounter[1];

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        if (uiData == DATA_MAGTHERIDON)
            return m_uiMagtheridonGUID;

        return 0;
    }

    void AttackNearestTarget(Creature* pCreature)
    {
        float minRange = VISIBLE_RANGE;
        float range;
        Player* target = NULL;

        Map::PlayerList const& players = instance->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* i_pl = itr->getSource())
            {
                if (i_pl->isTargetableForAttack())
                {
                    range = i_pl->GetDistance(pCreature);
                    if (range < minRange)
                    {
                        minRange = range;
                        target = i_pl;
                    }
                }
            }
        }

        if (!target)
        {
            debug_log("SD2: Instance Magtheridon: AttackNearestTarget failed. No player.");
            return;
        }
        pCreature->AI()->AttackStart(target);
    }

    void Update(uint32 uiDiff)
    {
        if (m_uiCageTimer)
        {
            if (m_uiCageTimer <= uiDiff)
            {
                SetData(TYPE_MAGTHERIDON_EVENT, IN_PROGRESS);
                m_uiCageTimer = 0;
            }
            else
                m_uiCageTimer -= uiDiff;
        }

        if (m_uiRespawnTimer)
        {
            if (m_uiRespawnTimer <= uiDiff)
            {
                for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                {
                    if (Creature* pChanneler = instance->GetCreature(*i))
                    {
                        if (pChanneler->isAlive())
                            pChanneler->AI()->EnterEvadeMode();
                        else
                            pChanneler->Respawn();
                    }
                }

                m_uiRespawnTimer = 0;
            }
            else
                m_uiRespawnTimer -= uiDiff;
        }
    }
};

InstanceData* GetInstanceData_instance_magtheridons_lair(Map* pMap)
{
    return new instance_magtheridons_lair(pMap);
}

void AddSC_instance_magtheridons_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_magtheridons_lair";
    newscript->GetInstanceData = &GetInstanceData_instance_magtheridons_lair;
    newscript->RegisterSelf();
}
