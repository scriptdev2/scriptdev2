/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_HALLS_OF_STONE_H
#define DEF_HALLS_OF_STONE_H

enum
{
    MAX_ENCOUNTER           = 4,

    TYPE_TRIBUNAL           = 0,
    TYPE_MAIDEN             = 1,
    TYPE_KRYSTALLUS         = 2,
    TYPE_SJONNIR            = 3,

    // NPC_BRANN            = 28070,

    NPC_KADDRAK             = 30898,
    NPC_ABEDNEUM            = 30899,
    NPC_MARNAK              = 30897,
    NPC_TRIBUNAL_OF_AGES    = 28234,
    NPC_WORLDTRIGGER        = 22515,

    GO_DOOR_SJONNIR         = 191296,
    GO_DOOR_TRIBUNAL        = 191294,                       // possibly closed during event?

    GO_TRIBUNAL_CHEST       = 190586,
    GO_TRIBUNAL_CHEST_H     = 193996,

    GO_TRIBUNAL_HEAD_RIGHT  = 191670,                       // marnak
    GO_TRIBUNAL_HEAD_CENTER = 191669,                       // abedneum
    GO_TRIBUNAL_HEAD_LEFT   = 191671,                       // kaddrak

    GO_TRIBUNAL_CONSOLE     = 193907,
    GO_TRIBUNAL_FLOOR       = 191527,

    GO_SJONNIR_CONSOLE      = 193906,

    MAX_FACES               = 3,
    FACE_MARNAK             = 0,
    FACE_ABEDNEUM           = 1,
    FACE_KADDRAK            = 2,
};

struct Face
{
    Face() : m_bIsActive(false), m_uiTimer(1000) {}

    ObjectGuid m_leftEyeGuid;
    ObjectGuid m_rightEyeGuid;
    ObjectGuid m_goFaceGuid;
    ObjectGuid m_speakerGuid;
    bool m_bIsActive;
    uint32 m_uiTimer;
};

class MANGOS_DLL_DECL instance_halls_of_stone : public ScriptedInstance
{
    public:
        instance_halls_of_stone(Map* pMap);
        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void Update(uint32 uiDiff);

        void ActivateFace(uint8 uiFace, bool bAfterEvent);
        void DoFaceSpeak(uint8 uiFace, int32 iTextId);

    private:
        void SortFaces();
        void ProcessFace(uint8 uiFace);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        Face m_aFaces[MAX_FACES];

        GUIDList m_lKaddrakGUIDs;
        GUIDList m_lAbedneumGUIDs;
        GUIDList m_lMarnakGUIDs;
        GUIDList m_lTribunalGUIDs;
        GUIDList m_lWorldtriggerGUIDs;

        uint64 m_uiBrannGUID;
        uint64 m_uiSjonnirDoorGUID;
        uint64 m_uiTribunalDoorGUID;
        uint64 m_uiTribunalChestGUID;
        uint64 m_uiTribunalConsoleGUID;
        uint64 m_uiTribunalFloorGUID;
        uint64 m_uiSjonnirConsoleGUID;
};

#endif
