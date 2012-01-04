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

static const DialogueEntry aFelmystOutroDialogue[] =
{
    {NPC_KALECGOS,          0,              10000},
    {SAY_KALECGOS_OUTRO,    NPC_KALECGOS,   10000},
    {SPELL_OPEN_BACK_DOOR,  0,              0},
    {0, 0, 0},
};

instance_sunwell_plateau::instance_sunwell_plateau(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aFelmystOutroDialogue),
    m_uiMuruBerserkTimer(0),
    m_uiDeceiversKilled(0),
    m_uiKalecRespawnTimer(0),
    m_uiSpectralRealmTimer(5000)
{
    Initialize();
}

void instance_sunwell_plateau::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    InitializeDialogueHelper(this);
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

void instance_sunwell_plateau::OnPlayerEnter(Player* pPlayer)
{
    // Return if Felmyst already dead, or Brutallus alive
    if (m_auiEncounter[TYPE_BRUTALLUS] != DONE || m_auiEncounter[TYPE_FELMYST] == DONE)
        return;

    // Return if already summoned
    if (GetSingleCreatureFromStorage(NPC_FELMYST, true))
        return;

    // Summon Felmyst in reload case
    pPlayer->SummonCreature(NPC_FELMYST, aMadrigosaGroundLoc[0], aMadrigosaGroundLoc[1], aMadrigosaGroundLoc[2], aMadrigosaGroundLoc[3], TEMPSUMMON_DEAD_DESPAWN, 0);
}

void instance_sunwell_plateau::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_KALECGOS_DRAGON:
        case NPC_KALECGOS_HUMAN:
        case NPC_SATHROVARR:
        case NPC_FLIGHT_TRIGGER_LEFT:
        case NPC_MADRIGOSA:
        case NPC_BRUTALLUS:
        case NPC_FELMYST:
        case NPC_ALYTHESS:
        case NPC_SACROLASH:
        case NPC_MURU:
        case NPC_ENTROPIUS:
        case NPC_KILJAEDEN_CONTROLLER:
        case NPC_KILJAEDEN:
        case NPC_KALECGOS:
        case NPC_ANVEENA:
        case NPC_VELEN:
        case NPC_LIADRIN:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_sunwell_plateau::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_DECEIVER)
    {
        ++m_uiDeceiversKilled;
        // Spawn Kiljaeden when all deceivers are killed
        if (m_uiDeceiversKilled == MAX_DECEIVERS)
        {
            if (Creature* pController = GetSingleCreatureFromStorage(NPC_KILJAEDEN_CONTROLLER))
            {
                if (Creature* pKiljaeden = pController->SummonCreature(NPC_KILJAEDEN, pController->GetPositionX(), pController->GetPositionY(), pController->GetPositionZ(), pController->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 0))
                    pKiljaeden->SetInCombatWithZone();

                pController->RemoveAurasDueToSpell(SPELL_ANVEENA_DRAIN);
            }
        }
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
            if (uiData == FAIL)
            {
                m_uiKalecRespawnTimer = 20000;

                if (Creature* pKalecDragon = GetSingleCreatureFromStorage(NPC_KALECGOS_DRAGON))
                    pKalecDragon->ForcedDespawn();
                if (Creature* pKalecHuman = GetSingleCreatureFromStorage(NPC_KALECGOS_HUMAN))
                    pKalecHuman->ForcedDespawn();
                if (Creature* pSathrovarr = GetSingleCreatureFromStorage(NPC_SATHROVARR))
                    pSathrovarr->AI()->EnterEvadeMode();
            }
            break;
        case TYPE_BRUTALLUS:
            m_auiEncounter[uiType] = uiData;
            // Temporary - until spells 46609 and 46637 are properly fixed
            if (uiData == SPECIAL)
                DoUseDoorOrButton(GO_ICE_BARRIER, MINUTE);
            break;
        case TYPE_FELMYST:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                StartNextDialogueText(NPC_KALECGOS);
                // Temporary - until spell 46650 is properly fixed
                DoUseDoorOrButton(GO_FIRE_BARRIER);
            }
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
            else if (uiData == IN_PROGRESS)
                m_uiMuruBerserkTimer = 10*MINUTE*IN_MILLISECONDS;
            break;
        case TYPE_KILJAEDEN:
            m_auiEncounter[uiType] = uiData;
            // When event fails the deceivers are respawned so restart the counter
            if (uiData == FAIL)
                m_uiDeceiversKilled = 0;
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

uint32 instance_sunwell_plateau::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_sunwell_plateau::Update(uint32 uiDiff)
{
    DialogueUpdate(uiDiff);

    if (m_uiKalecRespawnTimer)
    {
        if (m_uiKalecRespawnTimer <= uiDiff)
        {
            if (Creature* pKalecDragon = GetSingleCreatureFromStorage(NPC_KALECGOS_DRAGON))
                pKalecDragon->Respawn();
            if (Creature* pKalecHuman = GetSingleCreatureFromStorage(NPC_KALECGOS_HUMAN))
                pKalecHuman->Respawn();
            m_uiKalecRespawnTimer = 0;
        }
        else
            m_uiKalecRespawnTimer -= uiDiff;
    }

    // Muru berserk timer; needs to be done here because it involves two distinct creatures
    if (m_auiEncounter[TYPE_MURU] == IN_PROGRESS)
    {
        if (m_uiMuruBerserkTimer <= uiDiff)
        {
            if (Creature* pEntrpius = GetSingleCreatureFromStorage(NPC_ENTROPIUS, true))
                pEntrpius->CastSpell(pEntrpius, SPELL_MURU_BERSERK, true);
            else if (Creature* pMuru = GetSingleCreatureFromStorage(NPC_MURU))
                pMuru->CastSpell(pMuru, SPELL_MURU_BERSERK, true);

            m_uiMuruBerserkTimer = 10*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiMuruBerserkTimer -= uiDiff;
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

void instance_sunwell_plateau::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case NPC_KALECGOS:
            if (Creature* pTrigger = GetSingleCreatureFromStorage(NPC_FLIGHT_TRIGGER_LEFT))
            {
                if (Creature* pKalec = pTrigger->SummonCreature(NPC_KALECGOS, pTrigger->GetPositionX(), pTrigger->GetPositionY(), pTrigger->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 1*MINUTE*IN_MILLISECONDS))
                {
                    pKalec->SetWalk(false);
                    pKalec->SetLevitate(true);
                    pKalec->GetMotionMaster()->MovePoint(0, aMadrigosaFlyLoc[0], aMadrigosaFlyLoc[1], aMadrigosaFlyLoc[2]);
                }
            }
            break;
        case SPELL_OPEN_BACK_DOOR:
            if (Creature* pKalec = GetSingleCreatureFromStorage(NPC_KALECGOS))
                pKalec->CastSpell(pKalec, SPELL_OPEN_BACK_DOOR, true);
            break;
    }
}

InstanceData* GetInstanceData_instance_sunwell_plateau(Map* pMap)
{
    return new instance_sunwell_plateau(pMap);
}

bool AreaTrigger_at_sunwell_plateau(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_TWINS)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        instance_sunwell_plateau* pInstance = (instance_sunwell_plateau*)pPlayer->GetInstanceData();

        if (pInstance && pInstance->GetData(TYPE_EREDAR_TWINS) == NOT_STARTED)
            pInstance->SetData(TYPE_EREDAR_TWINS, SPECIAL);
    }

    return false;
}

void AddSC_instance_sunwell_plateau()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_sunwell_plateau";
    pNewScript->GetInstanceData = &GetInstanceData_instance_sunwell_plateau;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_sunwell_plateau";
    pNewScript->pAreaTrigger = &AreaTrigger_at_sunwell_plateau;
    pNewScript->RegisterSelf();
}
