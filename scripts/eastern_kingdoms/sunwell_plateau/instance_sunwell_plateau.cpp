/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Instance_Sunwell_Plateau
SD%Complete: 70%
SDComment:
SDCategory: Sunwell_Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

/* Sunwell Plateau:
0 - Kalecgos and Sathrovarr
1 - Brutallus
2 - Felmyst
3 - Eredar Twins (Alythess and Sacrolash)
4 - M'uru
5 - Kil'Jaeden
*/

instance_sunwell_plateau::instance_sunwell_plateau(Map* pMap) : ScriptedInstance(pMap),
    m_uiSpectralRealmTimer(5000)
{
    Initialize();
}

void instance_sunwell_plateau::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_sunwell_plateau::IsEncounterInProgress() const
{
    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_sunwell_plateau::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_KALECGOS_DRAGON:
        case NPC_KALECGOS_HUMAN:
        case NPC_SATHROVARR:
        case NPC_ALYTHESS:
        case NPC_SACROLASH:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_sunwell_plateau::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_FORCEFIELD:
        case GO_BOSS_COLLISION_1:
        case GO_BOSS_COLLISION_2:
        case GO_ICE_BARRIER:
            break;
        case GO_FIRE_BARRIER:
            if (m_auiEncounter[TYPE_KALECGOS] == DONE && m_auiEncounter[TYPE_BRUTALLUS] == DONE && m_auiEncounter[TYPE_FELMYST] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_FIRST_GATE:
            break;
        case GO_SECOND_GATE:
            if (m_auiEncounter[TYPE_EREDAR_TWINS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MURU_ENTER_GATE:
            if (m_auiEncounter[TYPE_EREDAR_TWINS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MURU_EXIT_GATE:
            if (m_auiEncounter[TYPE_MURU] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_THIRD_GATE:
            if (m_auiEncounter[TYPE_MURU] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_sunwell_plateau::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_KALECGOS:
            m_auiEncounter[uiType] = uiData;
            // combat doors
            DoUseDoorOrButton(GO_FORCEFIELD);
            DoUseDoorOrButton(GO_BOSS_COLLISION_1);
            DoUseDoorOrButton(GO_BOSS_COLLISION_2);
            if (uiData == IN_PROGRESS)
                SpectralRealmList.clear();
            break;
        case TYPE_BRUTALLUS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == SPECIAL)
                DoUseDoorOrButton(GO_ICE_BARRIER, MINUTE);
            break;
        case TYPE_FELMYST:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_FIRE_BARRIER);
            break;
        case TYPE_EREDAR_TWINS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_SECOND_GATE);
                DoUseDoorOrButton(GO_MURU_ENTER_GATE);
            }
            break;
        case TYPE_MURU:
            m_auiEncounter[uiType] = uiData;
            // combat door
            DoUseDoorOrButton(GO_MURU_ENTER_GATE);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_MURU_EXIT_GATE);
                DoUseDoorOrButton(GO_THIRD_GATE);
            }
            break;
        case TYPE_KILJAEDEN:
            m_auiEncounter[uiType] = uiData;
            break;
        case DATA_SET_SPECTRAL_CHECK:
            m_uiSpectralRealmTimer = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_sunwell_plateau::SetData64(uint32 uiType, uint64 uiData)
{
    if (uiType == DATA_PLAYER_SPECTRAL_REALM)
        SpectralRealmList.push_back(ObjectGuid(uiData));
}

uint32 instance_sunwell_plateau::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_sunwell_plateau::EjectPlayer(Player* pPlayer)
{
    debug_log("SD2: Ejecting Player %s from Spectral Realm", pPlayer->GetName());

    // Put player back in Kalecgos(Dragon)'s threat list
    /*if (Creature* pKalecgos = GetSingleCreatureFromStorage(NPC_KALECGOS_DRAGON))
    {
        if (pKalecgos->isAlive())
        {
            debug_log("SD2: Adding %s in Kalecgos' threatlist", pPlayer->GetName());
            pKalecgos->AddThreat(pPlayer);
        }
    }

    // Remove player from Sathrovarr's threat list
    if (Creature* pSath = instance->GetCreature(NPC_SATHROVARR))
    {
        if (pSath->isAlive())
        {
            if (HostileReference* pRef = pSath->getThreatManager().getOnlineContainer().getReferenceByTarget(pPlayer))
            {
                pRef->removeReference();
                debug_log("SD2: Deleting %s from Sathrovarr's threatlist", pPlayer->GetName());
            }
        }
    }*/

    pPlayer->CastSpell(pPlayer, SPELL_SPECTRAL_EXHAUSTION, true);
    pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_NORMAL_REALM, true);
}

void instance_sunwell_plateau::EjectPlayers()
{
    if (SpectralRealmList.empty())
        return;

    Map::PlayerList const& players = instance->GetPlayers();

    for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* plr = itr->getSource();

        if (plr && !plr->HasAura(SPELL_SPECTRAL_REALM))
        {
            SpectralRealmList.remove(plr->GetObjectGuid());
            EjectPlayer(plr);
        }
    }

    //SpectralRealmList.clear();
}

void instance_sunwell_plateau::Update(uint32 uiDiff)
{
    // Only check for Spectral Realm if Kalecgos Encounter is running
    if (m_auiEncounter[TYPE_KALECGOS] == IN_PROGRESS)
    {
        if (m_uiSpectralRealmTimer <= uiDiff)
        {
            EjectPlayers();
            m_uiSpectralRealmTimer = 1000;
        }
        else
            m_uiSpectralRealmTimer -= uiDiff;
    }
}

void instance_sunwell_plateau::Load(const char* in)
{
    if (!in)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(in);

    std::istringstream loadStream(in);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >>
        m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_sunwell_plateau(Map* pMap)
{
    return new instance_sunwell_plateau(pMap);
}

void AddSC_instance_sunwell_plateau()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_sunwell_plateau";
    pNewScript->GetInstanceData = &GetInstanceData_instance_sunwell_plateau;
    pNewScript->RegisterSelf();
}
