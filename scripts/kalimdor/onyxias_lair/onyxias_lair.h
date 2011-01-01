/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ONYXIA_H
#define DEF_ONYXIA_H

enum
{
    NPC_ONYXIA_TRIGGER          = 12758
};

class MANGOS_DLL_DECL instance_onyxias_lair : public ScriptedInstance
{
    public:
        instance_onyxias_lair(Map* pMap);
        ~instance_onyxias_lair() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);

        uint64 GetOnyxiaTriggerGUID() { return m_uiOnyxTriggerGUID; }

    protected:
        uint64 m_uiOnyxTriggerGUID;
};

#endif
