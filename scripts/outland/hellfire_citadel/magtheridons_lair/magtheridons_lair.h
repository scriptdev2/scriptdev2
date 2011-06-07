/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MAGTHERIDONS_LAIR_H
#define DEF_MAGTHERIDONS_LAIR_H

enum
{
    MAX_ENCOUNTER               = 2,

    TYPE_MAGTHERIDON_EVENT      = 1,
    TYPE_CHANNELER_EVENT        = 2,
    TYPE_HALL_COLLAPSE          = 4,

    DATA_CHANNELER              = 5,

    NPC_MAGTHERIDON             = 17257,
    NPC_CHANNELER               = 17256,

    SPELL_SOUL_TRANSFER         = 30531,
    SPELL_BLAZE_TARGET          = 30541,
    SPELL_DEBRIS_DAMAGE         = 30631,
    SPELL_DEBRIS_KNOCKDOWN      = 36449,

    GO_MANTICRON_CUBE           = 181713,
    GO_DOODAD_HF_MAG_DOOR01     = 183847,
    GO_DOODAD_HF_RAID_FX01      = 184653,
    GO_MAGTHERIDON_COLUMN_003   = 184634,
    GO_MAGTHERIDON_COLUMN_002   = 184635,
    GO_MAGTHERIDON_COLUMN_004   = 184636,
    GO_MAGTHERIDON_COLUMN_005   = 184637,
    GO_MAGTHERIDON_COLUMN_000   = 184638,
    GO_MAGTHERIDON_COLUMN_001   = 184639,
};

class MANGOS_DLL_DECL instance_magtheridons_lair : public ScriptedInstance
{
    public:
        instance_magtheridons_lair(Map* pMap);

        void Initialize();

        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void AttackNearestTarget(Creature* pCreature);

        void Update(uint32 uiDiff);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        GUIDSet m_sChannelerGuid;
        GUIDSet m_sColumnGuid;

        uint32 m_uiCageTimer;
        uint32 m_uiRespawnTimer;
};

#endif
