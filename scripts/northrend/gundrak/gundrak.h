/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_GUNDRAK_H
#define DEF_GUNDRAK_H
/* Encounters
 * Slad'ran          = 1
 * Drakkari Colossus = 2
 * Moorabi           = 3
 * Gal'darah         = 4
 * Eck the Ferocious = 5
*/
enum
{
    MAX_ENCOUNTER          = 5,

    TYPE_SLADRAN           = 1,
    TYPE_COLOSSUS          = 2,
    TYPE_MOORABI           = 3,
    TYPE_GALDARAH          = 4,
    TYPE_ECK               = 5,

    NPC_SLADRAN            = 29304,
    NPC_MOORABI            = 29307,
    NPC_COLOSSUS           = 29305,
    NPC_GALDARAH           = 29306,
    NPC_ECK                = 29932,

    GO_ECK_DOOR            = 192632,
    GO_ECK_UNDERWATER_DOOR = 192569,
    GO_GALDARAH_DOOR       = 192568,
    GO_EXIT_DOOR_L         = 193208,
    GO_EXIT_DOOR_R         = 193209,

    GO_ALTAR_OF_SLADRAN    = 192518,
    GO_ALTAR_OF_MOORABI    = 192519,
    GO_ALTAR_OF_COLOSSUS   = 192520,

    GO_SNAKE_KEY           = 192564,
    GO_TROLL_KEY           = 192567,
    GO_MAMMOTH_KEY         = 192565,
    GO_RHINO_KEY           = 192566,

    GO_BRIDGE              = 193188,
    GO_COLLISION           = 192633
};

class MANGOS_DLL_DECL instance_gundrak : public ScriptedInstance
{
    public:
        instance_gundrak(Map* pMap);
        ~instance_gundrak() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiEckDoorGUID;
        uint64 m_uiEckUnderwaterDoorGUID;
        uint64 m_uiGaldarahDoorGUID;
        uint64 m_uiExitDoorLeftGUID;
        uint64 m_uiExitDoorRightGUID;
        uint64 m_uiSnakeKeyGUID;
        uint64 m_uiMammothKeyGUID;
        uint64 m_uiTrollKeyGUID;
        uint64 m_uiAltarOfSladranGUID;
        uint64 m_uiAltarOfMoorabiGUID;
        uint64 m_uiAltarOfColossusGUID;
        uint64 m_uiBridgeGUID;

        uint64 m_uiSladranGUID;
};

#endif
