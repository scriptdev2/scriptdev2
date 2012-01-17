/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


#ifndef DEF_ICCRAID_H
#define DEF_ICCRAID_H

enum
{
    MAX_ENCOUNTER               = 1,
};

class MANGOS_DLL_DECL instance_icecrown_citadel : public ScriptedInstance
{
    public:
        instance_icecrown_citadel(Map* pMap);

        void Initialize();

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
};

#endif
