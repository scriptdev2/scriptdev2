/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_EBON_HOLD_H
#define DEF_EBON_HOLD_H

class MANGOS_DLL_DECL world_map_ebon_hold : public ScriptedInstance
{
    public:
        world_map_ebon_hold(Map* pMap);

        void Initialize() override;

        void Update(uint32 uiDiff) override;

        bool CanAndToggleGothikYell();

    protected:
        uint32 m_uiGothikYellTimer;                         // Timer to check if Gothik can yell (related q 12698)
        uint32 m_uiBattleEncounter;                         // Store state of the battle around  "The Light of Dawn"
};

#endif
