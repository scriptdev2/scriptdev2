/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


#ifndef DEF_ICCRAID_H
#define DEF_ICCRAID_H

enum
{
    MAX_ENCOUNTER               = 12,

    TYPE_MARROWGAR              = 0,
    TYPE_DEATHWHISPER           = 1,
    TYPE_GUNSHIP                = 2,
    TYPE_SAURFANG               = 3,
    TYPE_FESTERGUT              = 4,
    TYPE_ROTFACE                = 5,
    TYPE_PUTRICIDE              = 6,
    TYPE_COUNCIL                = 7,
    TYPE_BLOOD_QUEEN            = 8,
    TYPE_VALITHRIA              = 9,
    TYPE_SINDRAGOSA             = 10,
    TYPE_LICH_KING              = 11,
};

class MANGOS_DLL_DECL instance_icecrown_citadel : public ScriptedInstance
{
    public:
        instance_icecrown_citadel(Map* pMap);

        void Initialize();

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};

#endif
