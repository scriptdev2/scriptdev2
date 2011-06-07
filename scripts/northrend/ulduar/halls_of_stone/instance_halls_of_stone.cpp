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
SDName: Instance_Halls_of_Stone
SD%Complete: 10%
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

enum
{
    // KADDRAK
    SPELL_GLARE_OF_THE_TRIBUNAL         = 50988,
    SPELL_GLARE_OF_THE_TRIBUNAL_H       = 59870,

    // MARNAK
    SPELL_SUMMON_DARK_MATTER_TARGET     = 51003,
    SPELL_DARK_MATTER                   = 51012,            // Exact mechanics unknown
    SPELL_DARK_MATTER_H                 = 59868,
    // NPC_DARK_MATTER_TARGET           = 28237,

    // ABEDNEUM
    SPELL_SUMMON_SEARING_GAZE_TARGET    = 51146,
    SPELL_SEARING_GAZE                  = 51136,
    SPELL_SEARING_GAZE_H                = 59867,
    // NPC_SEARING_GAZE_TARGET          = 28265,
};

instance_halls_of_stone::instance_halls_of_stone(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_halls_of_stone::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_halls_of_stone::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_KADDRAK:          m_lKaddrakGUIDs.push_back(pCreature->GetObjectGuid());      break;
        case NPC_ABEDNEUM:         m_lAbedneumGUIDs.push_back(pCreature->GetObjectGuid());     break;
        case NPC_MARNAK:           m_lMarnakGUIDs.push_back(pCreature->GetObjectGuid());       break;
        case NPC_TRIBUNAL_OF_AGES: m_lTribunalGUIDs.push_back(pCreature->GetObjectGuid());     break;
        case NPC_WORLDTRIGGER:     m_lWorldtriggerGUIDs.push_back(pCreature->GetObjectGuid()); break;
    }
}

void instance_halls_of_stone::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_SJONNIR:
            break;
        case GO_DOOR_TRIBUNAL:
        case GO_TRIBUNAL_CHEST:
        case GO_TRIBUNAL_CHEST_H:
            break;
        case GO_TRIBUNAL_HEAD_RIGHT:
            m_aFaces[FACE_MARNAK].m_goFaceGuid = pGo->GetObjectGuid();
            return;
        case GO_TRIBUNAL_HEAD_CENTER:
            m_aFaces[FACE_ABEDNEUM].m_goFaceGuid = pGo->GetObjectGuid();
            return;
        case GO_TRIBUNAL_HEAD_LEFT:
            m_aFaces[FACE_KADDRAK].m_goFaceGuid = pGo->GetObjectGuid();
            return;
        case GO_TRIBUNAL_CONSOLE:
        case GO_TRIBUNAL_FLOOR:
        case GO_SJONNIR_CONSOLE:
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_halls_of_stone::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_TRIBUNAL:
            m_auiEncounter[0] = uiData;
            switch (uiData)
            {
                case IN_PROGRESS:
                    SortFaces();
                    break;
                case DONE:
                    // DoRespawnGameObject(instance->isRegularDifficulty ? GO_TRIBUNAL_CHEST : GO_TRIBUNAL_CHEST_H);
                    // Actually, this one need to be changed faction or similar..
                    break;
                case FAIL:
                    for (uint8 i = 0; i < MAX_FACES; ++i)
                    {
                        // Shut down the faces
                        if (m_aFaces[i].m_bIsActive)
                            DoUseDoorOrButton(m_aFaces[i].m_goFaceGuid);
                        m_aFaces[i].m_bIsActive = false;
                        m_aFaces[i].m_uiTimer = 1000;
                    }
                    break;
                case SPECIAL:
                    for (uint8 i = 0; i < MAX_FACES; ++i)
                    {
                        m_aFaces[i].m_bIsActive = false;
                        m_aFaces[i].m_uiTimer = 1000;
                        // TODO - Kill NPCs
                        // TODO - Check which stay red and how long (also find out how they get red..)
                    }
                    break;
            }
            break;
        case TYPE_MAIDEN:
            m_auiEncounter[1] = uiData;
            if (uiData == IN_PROGRESS)
                DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_START_MAIDEN_ID);
            break;
        case TYPE_KRYSTALLUS:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_SJONNIR:
            m_auiEncounter[3] = uiData;
            break;
    }
}

uint32 instance_halls_of_stone::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_TRIBUNAL:   return m_auiEncounter[0];
        case TYPE_MAIDEN:     return m_auiEncounter[1];
        case TYPE_KRYSTALLUS: return m_auiEncounter[2];
        case TYPE_SJONNIR:    return m_auiEncounter[3];
        default:
            return 0;
    }
}

struct SortHelper
{
    SortHelper(WorldObject const* pRef): m_pRef(pRef) {}
    bool operator()(WorldObject* pLeft, WorldObject* pRight)
    {
        return m_pRef->GetDistanceOrder(pLeft, pRight);
    }
    WorldObject const* m_pRef;
};

// Small Helper-function
static void GetValidNPCsOfList(Map* pMap, GUIDList& lGUIDs, std::list<Creature*>& lNPCs)
{
    lNPCs.clear();
    for (GUIDList::const_iterator itr = lGUIDs.begin(); itr != lGUIDs.end(); ++itr)
    {
        if (Creature* pMob = pMap->GetCreature(*itr))
            lNPCs.push_back(pMob);
    }
}

void instance_halls_of_stone::SortFaces()
{
    std::list<Creature*> lPossibleEyes;
    GameObject* pFace = NULL;

    // FACE_MARNAK
    if (pFace = instance->GetGameObject(m_aFaces[FACE_MARNAK].m_goFaceGuid))
    {
        // Find Marnak NPCs
        GetValidNPCsOfList(instance, m_lMarnakGUIDs, lPossibleEyes);
        if (lPossibleEyes.size() > 1)
        {
            lPossibleEyes.sort(SortHelper(pFace));
            std::list<Creature*>::const_iterator itr = lPossibleEyes.begin();
            m_aFaces[FACE_MARNAK].m_leftEyeGuid = (*itr)->GetObjectGuid();
            ++itr;
            m_aFaces[FACE_MARNAK].m_speakerGuid = (*itr)->GetObjectGuid();
        }
        // Find Worldtrigger NPC
        GetValidNPCsOfList(instance, m_lWorldtriggerGUIDs, lPossibleEyes);
        if (!lPossibleEyes.empty())
        {
            lPossibleEyes.sort(SortHelper(pFace));
            m_aFaces[FACE_MARNAK].m_rightEyeGuid = (*lPossibleEyes.begin())->GetObjectGuid();
        }
    }

    // FACE_ABEDNEUM
    if (pFace = instance->GetGameObject(m_aFaces[FACE_ABEDNEUM].m_goFaceGuid))
    {
        // Find Abedneum NPCs
        GetValidNPCsOfList(instance, m_lAbedneumGUIDs, lPossibleEyes);
        if (lPossibleEyes.size() > 1)
        {
            lPossibleEyes.sort(SortHelper(pFace));
            std::list<Creature*>::const_iterator itr = lPossibleEyes.begin();
            m_aFaces[FACE_ABEDNEUM].m_leftEyeGuid = (*itr)->GetObjectGuid();
            ++itr;
            m_aFaces[FACE_ABEDNEUM].m_speakerGuid = (*itr)->GetObjectGuid();
        }
        // Find Worldtrigger NPC
        GetValidNPCsOfList(instance, m_lWorldtriggerGUIDs, lPossibleEyes);
        if (!lPossibleEyes.empty())
        {
            lPossibleEyes.sort(SortHelper(pFace));
            m_aFaces[FACE_ABEDNEUM].m_rightEyeGuid = (*lPossibleEyes.begin())->GetObjectGuid();
        }
    }

    // FACE_KADDRAK
    if (pFace = instance->GetGameObject(m_aFaces[FACE_KADDRAK].m_goFaceGuid))
    {
        // Find Marnak NPCs
        GetValidNPCsOfList(instance, m_lKaddrakGUIDs, lPossibleEyes);
        if (lPossibleEyes.size() > 1)
        {
            lPossibleEyes.sort(SortHelper(pFace));
            std::list<Creature*>::const_iterator itr = lPossibleEyes.begin();
            m_aFaces[FACE_KADDRAK].m_leftEyeGuid = (*itr)->GetObjectGuid();
            ++itr;
            m_aFaces[FACE_KADDRAK].m_speakerGuid = (*itr)->GetObjectGuid();
        }
        // Find Tribunal NPC
        GetValidNPCsOfList(instance, m_lTribunalGUIDs, lPossibleEyes);
        if (!lPossibleEyes.empty())
        {
            lPossibleEyes.sort(SortHelper(pFace));
            m_aFaces[FACE_KADDRAK].m_rightEyeGuid = (*lPossibleEyes.begin())->GetObjectGuid();
        }
    }

    // Clear GUIDs
    m_lKaddrakGUIDs.clear();
    m_lAbedneumGUIDs.clear();
    m_lMarnakGUIDs.clear();
    m_lTribunalGUIDs.clear();
    m_lWorldtriggerGUIDs.clear();
}

void instance_halls_of_stone::ActivateFace(uint8 uiFace, bool bAfterEvent)
{
    if (uiFace >= MAX_FACES)
        return;

    if (bAfterEvent)
        DoUseDoorOrButton(m_aFaces[uiFace].m_goFaceGuid);
    else
    {
        DoUseDoorOrButton(m_aFaces[uiFace].m_goFaceGuid, 0, true);
        m_aFaces[uiFace].m_bIsActive = true;
    }
}

void instance_halls_of_stone::DoFaceSpeak(uint8 uiFace, int32 iTextId)
{
    if (uiFace >= MAX_FACES)
        return;

    if (Creature* pSpeaker = instance->GetCreature(m_aFaces[uiFace].m_speakerGuid))
        DoScriptText(iTextId, pSpeaker);
}

void instance_halls_of_stone::Update(uint32 uiDiff)
{
    if (m_auiEncounter[TYPE_TRIBUNAL] == IN_PROGRESS)
    {
        for (uint8 i = 0; i < MAX_FACES; ++i)
        {
            if (!m_aFaces[i].m_bIsActive)
                continue;

            if (m_aFaces[i].m_uiTimer < uiDiff)
                ProcessFace(i);
            else
                m_aFaces[i].m_uiTimer -= uiDiff;
        }
    }
}

void instance_halls_of_stone::ProcessFace(uint8 uiFace)
{
    // Cast dmg spell from face eyes, and reset timer for face
    switch (uiFace)
    {
        case FACE_KADDRAK:
            if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_leftEyeGuid))
                pEye->CastSpell(pEye, instance->IsRegularDifficulty() ? SPELL_GLARE_OF_THE_TRIBUNAL : SPELL_GLARE_OF_THE_TRIBUNAL_H, true);
            if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_rightEyeGuid))
                pEye->CastSpell(pEye, instance->IsRegularDifficulty() ? SPELL_GLARE_OF_THE_TRIBUNAL : SPELL_GLARE_OF_THE_TRIBUNAL_H, true);
            m_aFaces[uiFace].m_uiTimer = 1500;              // TODO, verify
            break;
        case FACE_MARNAK:
            if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_leftEyeGuid))
                pEye->CastSpell(pEye, SPELL_SUMMON_DARK_MATTER_TARGET, true);
            // One should be enough..
            //if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_rightEyeGuid))
            //    pEye->CastSpell(pEye, SPELL_SUMMON_DARK_MATTER_TARGET, true);
            m_aFaces[uiFace].m_uiTimer = 6000;              // TODO, verify, was 3s+0..1s
            break;
        case FACE_ABEDNEUM:
            if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_leftEyeGuid))
                pEye->CastSpell(pEye, SPELL_SUMMON_SEARING_GAZE_TARGET, true);
            // One should be enough..
            //if (Creature* pEye = instance->GetCreature(m_aFaces[uiFace].m_rightEyeGuid))
            //    pEye->CastSpell(pEye, SPELL_SUMMON_SEARING_GAZE_TARGET, true);
            m_aFaces[uiFace].m_uiTimer = 6000;              // TODO, verify, was 3s+0..2s
            break;
        default:
            return;
    }
}

InstanceData* GetInstanceData_instance_halls_of_stone(Map* pMap)
{
    return new instance_halls_of_stone(pMap);
}

void AddSC_instance_halls_of_stone()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_halls_of_stone";
    pNewScript->GetInstanceData = &GetInstanceData_instance_halls_of_stone;
    pNewScript->RegisterSelf();
}
