/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MECHANAR_H
#define DEF_MECHANAR_H

enum
{
    MAX_ENCOUNTER       = 1,

    TYPE_SEPETHREA      = 1
};

class MANGOS_DLL_DECL instance_mechanar : public ScriptedInstance
{
    public:
        instance_mechanar(Map* pMap);

        void Initialize();

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
};

#endif
