/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SHADOWFANG_H
#define DEF_SHADOWFANG_H

enum
{
    TYPE_FREE_NPC     = 1,
    TYPE_RETHILGORE   = 2,
    TYPE_FENRUS       = 3,
    TYPE_NANDOS       = 4,
    TYPE_INTRO        = 5,
    TYPE_VOIDWALKER   = 6,
    DATA_LIGHTNING    = 7
};

struct MANGOS_DLL_DECL mob_arugal_voidwalkerAI : public ScriptedAI
{
    mob_arugal_voidwalkerAI(Creature* pCreature);

    uint32 m_uiResetTimer, m_uiDarkOffering;
    uint8 m_uiCurrentPoint, m_uiPosition; //0 - leader, 1 - behind-right, 2 - behind, 3 - behind-left
    uint64 m_uiLeaderGUID;
    ScriptedInstance* m_pInstance;
    bool m_bIsLeader, m_bReverse, m_bWPDone;

    void Reset();
    void UpdateAI(const uint32 uiDiff);
    void MovementInform(uint32 uiMoveType, uint32 uiPointId);
    void JustDied(Unit* /*pKiller*/);
    void SetPosition(uint8 uiPosition, Creature* pLeader);
    uint8 GetPosition();
    void SendWaypoint();
    void ReceiveWaypoint(uint32 uiNewPoint, bool bReverse);
    void EnterEvadeMode();
};
#endif
