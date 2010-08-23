/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_SIMPLEAI_H
#define SC_SIMPLEAI_H

enum CastTarget
{
    CAST_SELF = 0,                  //Self cast
    CAST_HOSTILE_TARGET,            //Our current target (ie: highest aggro)
    CAST_HOSTILE_SECOND_AGGRO,      //Second highest aggro (generaly used for cleaves and some special attacks)
    CAST_HOSTILE_LAST_AGGRO,        //Dead last on aggro (no idea what this could be used for)
    CAST_HOSTILE_RANDOM,            //Just any random target on our threat list
    CAST_FRIENDLY_RANDOM,           //NOT YET IMPLEMENTED

    //Special cases
    CAST_KILLEDUNIT_VICTIM,         //Only works within KilledUnit function
    CAST_JUSTDIED_KILLER,           //Only works within JustDied function
};

struct MANGOS_DLL_DECL SimpleAI : public ScriptedAI
{
    SimpleAI(Creature* pCreature);

    void Reset();

    void Aggro(Unit* pWho);

    void KilledUnit(Unit* pVictim);

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage);

    void UpdateAI(const uint32 uiDiff);

    public:
        int32      m_aiAggroTextId[3];
        uint32     m_auiAggroSound[3];

        int32      m_aiDeathTextId[3];
        uint32     m_auiDeathSound[3];
        uint32     m_uiDeathSpell;
        CastTarget m_DeathTargetType;

        int32      m_aiKillTextId[3];
        uint32     m_auiKillSound[3];
        uint32     m_uiKillSpell;
        CastTarget m_KillTargetType;

        struct SimpleAI_Spell
        {
            uint32     uiSpellId;                               // Spell ID to cast
            int32      iFirstCast;                              // Delay for first cast
            uint32     uiCooldown;                              // Cooldown between casts
            uint32     uiCooldownRandomAddition;                // Random addition to cooldown (in range from 0 - CooldownRandomAddition)
            CastTarget CastTargetType;                          // Target type (note that certain spells may ignore this)
            bool       bInterruptPreviousCast;                  // Interrupt a previous cast if this spell needs to be cast
            bool       bEnabled;                                // Spell enabled or disabled (default: false)

            //3 texts to many?
            int32  aiTextId[3];
            uint32 auiTextSound[3];
        } m_Spell[10];

    protected:
        uint32 m_auiSpellTimer[10];
};

#endif
