/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
    NPC_SJONNIR             = 27978,

    GO_DOOR_MAIDEN          = 191292,
    GO_DOOR_TRIBUNAL        = 191294,                       // possibly closed during event?
    GO_DOOR_TO_TRIBUNAL     = 191295,
    GO_DOOR_SJONNIR         = 191296,

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

    ACHIEV_START_MAIDEN_ID  = 20383,
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

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

        void ActivateFace(uint8 uiFace, bool bAfterEvent);
        void DoFaceSpeak(uint8 uiFace, int32 iTextId);

    private:
        void SortFaces();
        void ProcessFace(uint8 uiFace);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        Face m_aFaces[MAX_FACES];
        std::string m_strInstData;

        GuidList m_lKaddrakGUIDs;
        GuidList m_lAbedneumGUIDs;
        GuidList m_lMarnakGUIDs;
        GuidList m_lTribunalGUIDs;
        GuidList m_lWorldtriggerGUIDs;
};

#endif
