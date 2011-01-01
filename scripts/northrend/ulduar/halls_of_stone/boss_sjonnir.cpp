/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Sjonnir
SD%Complete: 20%
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

enum
{
    SAY_AGGRO                       = -1599000,
    SAY_SLAY_1                      = -1599001,
    SAY_SLAY_2                      = -1599002,
    SAY_SLAY_3                      = -1599003,
    SAY_DEATH                       = -1599004,
    EMOTE_GENERIC_FRENZY            = -1000002,

    SPELL_FRENZY                    = 28747,

    SPELL_CHAIN_LIGHTNING           = 50830,
    SPELL_CHAIN_LIGHTNING_H         = 59844,

    SPELL_STATIC_CHARGE             = 50834,
    SPELL_STATIC_CHARGE_H           = 59846,

    SPELL_LIGHTNING_SHIELD          = 50831,
    SPELL_LIGHTNING_SHIELD_H        = 59845,

    SPELL_LIGHTNING_RING            = 50840,
    SPELL_LIGHTNING_RING_H          = 59848,

    SPELL_SUMMON_IRON_DWARF         = 50789,                // periodic dummy aura, tick each 30sec or each 20sec in heroic
    SPELL_SUMMON_IRON_DWARF_H       = 59860,                // left/right 50790,50791

    SPELL_SUMMON_IRON_TROGG         = 50792,                // periodic dummy aura, tick each 10sec or each 7sec in heroic
    SPELL_SUMMON_IRON_TROGG_H       = 59859,                // left/right 50793,50794

    SPELL_SUMMON_MALFORMED_OOZE     = 50801,                // periodic dummy aura, tick each 5sec or each 3sec in heroic
    SPELL_SUMMON_MALFORMED_OOZE_H   = 59858,                // left/right 50802,50803

    SPELL_SUMMON_IRON_SLUDGE        = 50747,                // instakill TARGET_SCRIPT
    SPELL_IRON_SLUDGE_SPAWN_VISUAL  = 50777,

    NPC_IRON_TROGG                  = 27979,
    NPC_IRON_DWARF                  = 27982,
    NPC_MALFORMED_OOZE              = 27981,
    NPC_IRON_SLUDGE                 = 28165
};

/*######
## boss_sjonnir
######*/

struct MANGOS_DLL_DECL boss_sjonnirAI : public ScriptedAI
{
    boss_sjonnirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
        if (m_creature->isAlive())
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_SHIELD : SPELL_LIGHTNING_SHIELD_H, false);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_SUMMON_IRON_DWARF : SPELL_SUMMON_IRON_DWARF_H, true);
        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_SUMMON_IRON_TROGG : SPELL_SUMMON_IRON_TROGG_H, true);
    }


    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_IRON_TROGG || pSummoned->GetEntry() == NPC_IRON_DWARF || pSummoned->GetEntry() == NPC_MALFORMED_OOZE)
        {
            float fX, fY, fZ;
            pSummoned->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 10.0f, fX, fY, fZ);

            pSummoned->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pSummoned->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sjonnir(Creature* pCreature)
{
    return new boss_sjonnirAI(pCreature);
}

void AddSC_boss_sjonnir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sjonnir";
    newscript->GetAI = &GetAI_boss_sjonnir;
    newscript->RegisterSelf();
}
