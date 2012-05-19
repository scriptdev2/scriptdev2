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
SDName: Instance_Arcatraz
SD%Complete: 80
SDComment: Mainly Harbringer Skyriss event
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "precompiled.h"
#include "arcatraz.h"

/* Arcatraz encounters:
1 - Zereketh the Unbound event
2 - Dalliah the Doomsayer event
3 - Wrath-Scryer Soccothrates event
4 - Harbinger Skyriss event, 5 sub-events
*/

enum
{
    YELL_MELLICHAR_INTRO1           = -1552023,
    YELL_MELLICHAR_INTRO2           = -1552024,
    YELL_MELLICHAR_RELEASE1         = -1552025,
    YELL_MELLICHAR_RELEASE2         = -1552026,
    YELL_MELLICHAR_RELEASE3         = -1552027,
    YELL_MELLICHAR_RELEASE4         = -1552028,
    YELL_MELLICHAR_RELEASE5         = -1552029,
    YELL_MELLICAR_WELCOME           = -1552030,
    SAY_SKYRISS_INTRO               = -1552000,
    SAY_SKYRISS_AGGRO               = -1552001,
    SAY_MILLHOUSE_COMPLETE          = -1552022,

    // Spells used by Mellichar during the dialogue
    SPELL_TARGET_BETA               = 36854,
    SPELL_TARGET_ALPHA              = 36856,
    SPELL_TARGET_DELTA              = 36857,
    SPELL_TARGET_GAMMA              = 36858,
};

static const DialogueEntry aArcatrazDialogue[] =
{
    {YELL_MELLICHAR_INTRO1,   NPC_MELLICHAR, 22000},
    {YELL_MELLICHAR_INTRO2,   NPC_MELLICHAR, 7000},
    {SPELL_TARGET_ALPHA,      0,             7000},
    {TYPE_WARDEN_1,           0,             0},
    {YELL_MELLICHAR_RELEASE2, NPC_MELLICHAR, 7000},
    {SPELL_TARGET_BETA,       0,             7000},
    {TYPE_WARDEN_2,           0,             0},
    {YELL_MELLICHAR_RELEASE3, NPC_MELLICHAR, 7000},
    {SPELL_TARGET_DELTA,      0,             7000},
    {TYPE_WARDEN_3,           0,             0},
    {YELL_MELLICHAR_RELEASE4, NPC_MELLICHAR, 7000},
    {SPELL_TARGET_GAMMA,      0,             7000},
    {TYPE_WARDEN_4,           0,             0},
    {YELL_MELLICHAR_RELEASE5, NPC_MELLICHAR, 8000},
    {TYPE_WARDEN_5,           0,             5000},
    {SAY_SKYRISS_INTRO,       NPC_SKYRISS,   25000},
    {YELL_MELLICAR_WELCOME,   NPC_MELLICHAR, 5000},
    {SAY_SKYRISS_AGGRO,       NPC_SKYRISS,   0},
    {0, 0, 0},
};

instance_arcatraz::instance_arcatraz(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aArcatrazDialogue),
    m_uiResetDelayTimer(0)
{
    Initialize();
}

void instance_arcatraz::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    InitializeDialogueHelper(this);
}

void instance_arcatraz::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CORE_SECURITY_FIELD_ALPHA:
            if (m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CORE_SECURITY_FIELD_BETA:
            if (m_auiEncounter[1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SEAL_SPHERE:
        case GO_POD_ALPHA:
        case GO_POD_BETA:
        case GO_POD_DELTA:
        case GO_POD_GAMMA:
        case GO_POD_OMEGA:
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_arcatraz::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_SKYRISS:
        case NPC_MILLHOUSE:
            m_lSkyrissEventMobsGuidList.push_back(pCreature->GetObjectGuid());
            // no break here because we want them in both lists
        case NPC_PRISON_APHPA_POD:
        case NPC_PRISON_BETA_POD:
        case NPC_PRISON_DELTA_POD:
        case NPC_PRISON_GAMMA_POD:
        case NPC_PRISON_BOSS_POD:
        case NPC_MELLICHAR:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_BLAZING_TRICKSTER:
        case NPC_PHASE_HUNTER:
        case NPC_AKKIRIS:
        case NPC_SULFURON:
        case NPC_TW_DRAKONAAR:
        case NPC_BL_DRAKONAAR:
            m_lSkyrissEventMobsGuidList.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_arcatraz::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ZEREKETH:
            m_auiEncounter[0] = uiData;
            break;

        case TYPE_DALLIAH:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CORE_SECURITY_FIELD_BETA);
            m_auiEncounter[1] = uiData;
            break;

        case TYPE_SOCCOTHRATES:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CORE_SECURITY_FIELD_ALPHA);
            m_auiEncounter[2] = uiData;
            break;

        case TYPE_HARBINGERSKYRISS:
            if (uiData == FAIL)
            {
                m_auiEncounter[4] = NOT_STARTED;
                m_auiEncounter[5] = NOT_STARTED;
                m_auiEncounter[6] = NOT_STARTED;
                m_auiEncounter[7] = NOT_STARTED;
                m_auiEncounter[8] = NOT_STARTED;

                // Reset event in 1 min
                if (Creature* pMellichar = GetSingleCreatureFromStorage(NPC_MELLICHAR))
                    pMellichar->ForcedDespawn();
                m_uiResetDelayTimer = 60000;

                // Despawn all the summons manually
                for (GUIDList::const_iterator itr = m_lSkyrissEventMobsGuidList.begin(); itr != m_lSkyrissEventMobsGuidList.end(); ++itr)
                {
                    if (Creature* pTemp = instance->GetCreature(*itr))
                        pTemp->ForcedDespawn();
                }

                // Reset these objects, because they doesn't reset automatically
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_POD_BETA))
                    pGo->ResetDoorOrButton();
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_POD_OMEGA))
                    pGo->ResetDoorOrButton();
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_SEAL_SPHERE))
                    pGo->ResetDoorOrButton();
            }
            if (uiData == IN_PROGRESS)
            {
                StartNextDialogueText(YELL_MELLICHAR_INTRO1);
                DoUseDoorOrButton(GO_SEAL_SPHERE);
            }
            if (uiData == DONE)
            {
                if (Creature* pMillhouse = GetSingleCreatureFromStorage(NPC_MILLHOUSE))
                    DoScriptText(SAY_MILLHOUSE_COMPLETE, pMillhouse);
            }
            m_auiEncounter[3] = uiData;
            break;

        case TYPE_WARDEN_1:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_ALPHA);
            if (uiData == DONE)
                StartNextDialogueText(YELL_MELLICHAR_RELEASE2);
            m_auiEncounter[4] = uiData;
            break;

        case TYPE_WARDEN_2:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_BETA);
            if (uiData == DONE)
                StartNextDialogueText(YELL_MELLICHAR_RELEASE3);
            m_auiEncounter[5] = uiData;
            break;

        case TYPE_WARDEN_3:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_DELTA);
            if (uiData == DONE)
                StartNextDialogueText(YELL_MELLICHAR_RELEASE4);
            m_auiEncounter[6] = uiData;
            break;

        case TYPE_WARDEN_4:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_GAMMA);
            if (uiData == DONE)
                StartNextDialogueText(YELL_MELLICHAR_RELEASE5);
            m_auiEncounter[7] = uiData;
            break;

        case TYPE_WARDEN_5:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(GO_POD_OMEGA);
            m_auiEncounter[8] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_arcatraz::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_HARBINGERSKYRISS: return m_auiEncounter[3];
        case TYPE_WARDEN_1:         return m_auiEncounter[4];
        case TYPE_WARDEN_2:         return m_auiEncounter[5];
        case TYPE_WARDEN_3:         return m_auiEncounter[6];
        case TYPE_WARDEN_4:         return m_auiEncounter[7];
        case TYPE_WARDEN_5:         return m_auiEncounter[8];

        default:
            return 0;
    }
}

void instance_arcatraz::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_arcatraz::JustDidDialogueStep(int32 iEntry)
{
    Creature* pMellichar = GetSingleCreatureFromStorage(NPC_MELLICHAR);
    if (!pMellichar)
        return;

    switch (iEntry)
    {
        case SPELL_TARGET_ALPHA:
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_ALPHA, false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_APHPA_POD))
                pMellichar->SetFacingToObject(pTarget);
            SetData(TYPE_WARDEN_1, IN_PROGRESS);
            break;
        case TYPE_WARDEN_1:
            pMellichar->SummonCreature(urand(0, 1) ? NPC_BLAZING_TRICKSTER : NPC_PHASE_HUNTER, aSummonPosition[0].m_fX, aSummonPosition[0].m_fY, aSummonPosition[0].m_fZ, aSummonPosition[0].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            break;
        case YELL_MELLICHAR_RELEASE2:
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_BETA_POD))
                pMellichar->SetFacingToObject(pTarget);
            break;
        case SPELL_TARGET_BETA:
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_BETA, false);
            SetData(TYPE_WARDEN_2, IN_PROGRESS);
            break;
        case TYPE_WARDEN_2:
            pMellichar->SummonCreature(NPC_MILLHOUSE, aSummonPosition[1].m_fX, aSummonPosition[1].m_fY, aSummonPosition[1].m_fZ, aSummonPosition[1].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            break;
        case SPELL_TARGET_DELTA:
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_DELTA, false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_DELTA_POD))
                pMellichar->SetFacingToObject(pTarget);
            SetData(TYPE_WARDEN_3, IN_PROGRESS);
            break;
        case TYPE_WARDEN_3:
            pMellichar->SummonCreature(urand(0, 1) ? NPC_AKKIRIS : NPC_SULFURON, aSummonPosition[2].m_fX, aSummonPosition[2].m_fY, aSummonPosition[2].m_fZ, aSummonPosition[2].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_OMEGA, false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_BOSS_POD))
                pMellichar->SetFacingToObject(pTarget);
            break;
        case YELL_MELLICHAR_RELEASE4:
            pMellichar->InterruptNonMeleeSpells(false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_GAMMA_POD))
                pMellichar->SetFacingToObject(pTarget);
            break;
        case SPELL_TARGET_GAMMA:
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_GAMMA, false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_GAMMA_POD))
                pMellichar->SetFacingToObject(pTarget);
            SetData(TYPE_WARDEN_4, IN_PROGRESS);
            break;
        case TYPE_WARDEN_4:
            pMellichar->SummonCreature(urand(0, 1) ? NPC_TW_DRAKONAAR : NPC_BL_DRAKONAAR, aSummonPosition[3].m_fX, aSummonPosition[3].m_fY, aSummonPosition[3].m_fZ, aSummonPosition[3].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            pMellichar->CastSpell(pMellichar, SPELL_TARGET_OMEGA, false);
            if (Creature* pTarget = GetSingleCreatureFromStorage(NPC_PRISON_BOSS_POD))
                pMellichar->SetFacingToObject(pTarget);
            break;
        case YELL_MELLICHAR_RELEASE5:
            pMellichar->InterruptNonMeleeSpells(false);
            SetData(TYPE_WARDEN_5, IN_PROGRESS);
            break;
        case TYPE_WARDEN_5:
            pMellichar->SummonCreature(NPC_SKYRISS, aSummonPosition[4].m_fX, aSummonPosition[4].m_fY, aSummonPosition[4].m_fZ, aSummonPosition[4].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
            break;
        case SAY_SKYRISS_AGGRO:
            // Kill Mellichar and start combat
            if (Creature* pSkyriss = GetSingleCreatureFromStorage(NPC_SKYRISS))
            {
                pSkyriss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                pMellichar->DealDamage(pMellichar, pMellichar->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            DoUseDoorOrButton(GO_SEAL_SPHERE);
            break;
    }
}

void instance_arcatraz::Update(uint32 uiDiff)
{
    DialogueUpdate(uiDiff);

    if (m_uiResetDelayTimer)
    {
        if (m_uiResetDelayTimer <= uiDiff)
        {
            if (Creature* pMellichar = GetSingleCreatureFromStorage(NPC_MELLICHAR))
                pMellichar->Respawn();
            m_uiResetDelayTimer = 0;
        }
        else
            m_uiResetDelayTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_arcatraz(Map* pMap)
{
    return new instance_arcatraz(pMap);
}

void AddSC_instance_arcatraz()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_arcatraz";
    pNewScript->GetInstanceData = &GetInstanceData_instance_arcatraz;
    pNewScript->RegisterSelf();
}
