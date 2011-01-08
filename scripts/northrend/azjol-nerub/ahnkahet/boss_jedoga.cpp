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
SDName: Boss_Jedoga
SD%Complete: 40
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SAY_AGGRO                = -1619017,
    SAY_CALL_SACRIFICE1      = -1619018,
    SAY_CALL_SACRIFICE2      = -1619019,
    SAY_SACRIFICE1           = -1619020,
    SAY_SACRIFICE2           = -1619021,
    SAY_SLAY_1               = -1619022,
    SAY_SLAY_2               = -1619023,
    SAY_SLAY_3               = -1619024,
    SAY_DEATH                = -1619025,

    // preaching 1-5 when it is used?
    SAY_PREACHING1           = -1619026,
    SAY_PREACHING2           = -1619027,
    SAY_PREACHING3           = -1619028,
    SAY_PREACHING4           = -1619029,
    SAY_PREACHING5           = -1619030,

    SAY_VOLUNTEER_CHOOSEN    = -1619031,                    // I have been choosen!
    SAY_VOLUNTEER_SACRIFICED = -1619032,                    // I give myself to the master!

    SPELL_CYCLONE_STRIKE     = 56855,
    SPELL_CYCLONE_STRIKE_H   = 60030,
    SPELL_LIGHTNING_BOLT     = 56891,
    SPELL_LIGHTNING_BOLT_H   = 60032,
    SPELL_THUNDERSHOCK       = 56926,
    SPELL_THUNDERSHOCK_H     = 60029,
    SPELL_GIFT_OF_THE_HERALD = 56219,

    SPELL_SACRIFICE_VISUAL   = 56133,
    SPELL_SACRIFICE_BEAM     = 56150,

    NPC_JEDOGA_CONTROLLER    = 30181,
    NPC_TWILIGHT_INITIATE    = 30114,
    NPC_TWILIGHT_VOLUNTEER   = 30385
};

/*######
## boss_jedoga
######*/

struct MANGOS_DLL_DECL boss_jedogaAI : public ScriptedAI
{
    boss_jedogaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiThundershockTimer;
    uint32 m_uiCycloneStrikeTimer;
    uint32 m_uiLightningBoltTimer;
    uint8 m_uiSacrificeCount;
    bool m_bSacrifice;

    void Reset()
    {
        m_uiThundershockTimer  = 40000;
        m_uiCycloneStrikeTimer = 15000;
        m_uiLightningBoltTimer = 7000;
        m_bSacrifice           = false;
    }

    Creature* SelectRandomCreatureOfEntryInRange(uint32 uiEntry, float fRange)
    {
        std::list<Creature* > lCreatureList;
        GetCreatureListWithEntryInGrid(lCreatureList, m_creature, uiEntry, fRange);

        if (lCreatureList.empty())
            return NULL;

        std::list<Creature* >::iterator iter = lCreatureList.begin();
        advance(iter, urand(0, lCreatureList.size()-1));

        return *iter;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
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

        if (m_creature->GetHealthPercent() < 50.0f && !m_bSacrifice)
        {
            // start sacrifice here
            m_bSacrifice = true;
        }

        if (m_uiThundershockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_THUNDERSHOCK : SPELL_THUNDERSHOCK_H);

            m_uiThundershockTimer = 40000;
        }
        else
            m_uiThundershockTimer -= uiDiff;

        if (m_uiLightningBoltTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_LIGHTNING_BOLT : SPELL_LIGHTNING_BOLT_H);

            m_uiLightningBoltTimer = 7000;
        }
        else
            m_uiLightningBoltTimer -= uiDiff;

        if (m_uiCycloneStrikeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CYCLONE_STRIKE : SPELL_CYCLONE_STRIKE_H);
            m_uiCycloneStrikeTimer = 15000;
        }
        else
            m_uiCycloneStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jedoga(Creature* pCreature)
{
    return new boss_jedogaAI(pCreature);
}

void AddSC_boss_jedoga()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_jedoga";
    newscript->GetAI = &GetAI_boss_jedoga;
    newscript->RegisterSelf();
}
