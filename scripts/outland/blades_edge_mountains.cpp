/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Blades_Edge_Mountains
SD%Complete: 90
SDComment: Quest support: 10503, 10504, 10556, 10609. (npc_daranelle needs bit more work before consider complete)
SDCategory: Blade's Edge Mountains
EndScriptData */

/* ContentData
mobs_bladespire_ogre
mobs_nether_drake
npc_daranelle
EndContentData */

#include "precompiled.h"

/*######
## mobs_bladespire_ogre
######*/

// TODO: add support for quest 10512 + creature abilities
struct MANGOS_DLL_DECL mobs_bladespire_ogreAI : public ScriptedAI
{
    mobs_bladespire_ogreAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() override { }

    void UpdateAI(const uint32 /*uiDiff*/) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mobs_bladespire_ogre(Creature* pCreature)
{
    return new mobs_bladespire_ogreAI(pCreature);
}

/*######
## mobs_nether_drake
######*/

enum
{
    SAY_NIHIL_1                 = -1000169,
    SAY_NIHIL_2                 = -1000170,
    SAY_NIHIL_3                 = -1000171,
    SAY_NIHIL_4                 = -1000172,
    SAY_NIHIL_INTERRUPT         = -1000173,

    MAX_ENTRIES                 = 4,

    NPC_PROTO                   = 21821,
    NPC_ADOLESCENT              = 21817,
    NPC_MATURE                  = 21820,
    NPC_NIHIL                   = 21823,

    SPELL_T_PHASE_MODULATOR     = 37573,

    SPELL_ARCANE_BLAST          = 38881,
    SPELL_MANA_BURN             = 38884,
    SPELL_INTANGIBLE_PRESENCE   = 36513,
};

static const uint32 aNetherDrakeEntries[MAX_ENTRIES] = {NPC_PROTO, NPC_ADOLESCENT, NPC_MATURE, NPC_NIHIL};

struct MANGOS_DLL_DECL mobs_nether_drakeAI : public ScriptedAI
{
    mobs_nether_drakeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bIsNihil;
    uint32 m_uiNihilSpeechTimer;
    uint32 m_uiNihilSpeechPhase;

    uint32 m_uiArcaneBlastTimer;
    uint32 m_uiManaBurnTimer;
    uint32 m_uiIntangiblePresenceTimer;

    void Reset() override
    {
        m_bIsNihil                  = false;
        m_uiNihilSpeechTimer        = 3000;
        m_uiNihilSpeechPhase        = 0;

        m_uiArcaneBlastTimer        = 7500;
        m_uiManaBurnTimer           = 10000;
        m_uiIntangiblePresenceTimer = 15000;
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    // in case creature was not summoned (not expected)
    void MovementInform(uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE)
            return;

        if (uiPointId)
            m_creature->ForcedDespawn();
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell) override
    {
        if (pSpell->Id == SPELL_T_PHASE_MODULATOR && pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            // we are nihil, so say before transform
            if (m_creature->GetEntry() == NPC_NIHIL)
            {
                DoScriptText(SAY_NIHIL_INTERRUPT, m_creature);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bIsNihil = false;
            }

            // choose a new entry
            uint8 uiIndex = urand(0, MAX_ENTRIES - 1);

            // If we choose the same entry, try again
            while (aNetherDrakeEntries[uiIndex] == m_creature->GetEntry())
                uiIndex = urand(0, MAX_ENTRIES - 1);

            if (m_creature->UpdateEntry(aNetherDrakeEntries[uiIndex]))
            {
                // Nihil does only dialogue
                if (aNetherDrakeEntries[uiIndex] == NPC_NIHIL)
                {
                    EnterEvadeMode();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_bIsNihil = true;
                }
                else
                    AttackStart(pCaster);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_bIsNihil)
        {
            if (m_uiNihilSpeechTimer < uiDiff)
            {
                switch (m_uiNihilSpeechPhase)
                {
                    case 0:
                        DoScriptText(SAY_NIHIL_1, m_creature);
                        break;
                    case 1:
                        DoScriptText(SAY_NIHIL_2, m_creature);
                        break;
                    case 2:
                        DoScriptText(SAY_NIHIL_3, m_creature);
                        break;
                    case 3:
                        DoScriptText(SAY_NIHIL_4, m_creature);
                        break;
                    case 4:
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        // take off to location above
                        m_creature->SetLevitate(true);
                        m_creature->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
                        m_creature->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX() + 50.0f, m_creature->GetPositionY(), m_creature->GetPositionZ() + 50.0f);
                        break;
                }
                ++m_uiNihilSpeechPhase;
                m_uiNihilSpeechTimer = 5000;
            }
            else
                m_uiNihilSpeechTimer -= uiDiff;

            // anything below here is not interesting for Nihil, so skip it
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiIntangiblePresenceTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_INTANGIBLE_PRESENCE) == CAST_OK)
                m_uiIntangiblePresenceTimer = urand(15000, 30000);
        }
        else
            m_uiIntangiblePresenceTimer -= uiDiff;

        if (m_uiManaBurnTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_MANA_BURN, SELECT_FLAG_POWER_MANA))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_MANA_BURN) == CAST_OK)
                    m_uiManaBurnTimer = urand(8000, 16000);
            }
        }
        else
            m_uiManaBurnTimer -= uiDiff;

        if (m_uiArcaneBlastTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BLAST) == CAST_OK)
                m_uiArcaneBlastTimer = urand(2500, 7500);
        }
        else
            m_uiArcaneBlastTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mobs_nether_drake(Creature* pCreature)
{
    return new mobs_nether_drakeAI(pCreature);
}

/*######
## npc_daranelle
######*/

enum
{
    SAY_SPELL_INFLUENCE     = -1000174,
    NPC_KALIRI_AURA_DISPEL  = 21511,
    SPELL_LASHHAN_CHANNEL   = 36904
};

struct MANGOS_DLL_DECL npc_daranelleAI : public ScriptedAI
{
    npc_daranelleAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() override { }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            if (pWho->HasAura(SPELL_LASHHAN_CHANNEL, EFFECT_INDEX_0) && m_creature->IsWithinDistInMap(pWho, 10.0f))
            {
                DoScriptText(SAY_SPELL_INFLUENCE, m_creature, pWho);

                // TODO: Move the below to updateAI and run if this statement == true
                ((Player*)pWho)->KilledMonsterCredit(NPC_KALIRI_AURA_DISPEL, m_creature->GetObjectGuid());
                pWho->RemoveAurasDueToSpell(SPELL_LASHHAN_CHANNEL);
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }
};

CreatureAI* GetAI_npc_daranelle(Creature* pCreature)
{
    return new npc_daranelleAI(pCreature);
}

/*######
## AddSC
######*/

void AddSC_blades_edge_mountains()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mobs_bladespire_ogre";
    pNewScript->GetAI = &GetAI_mobs_bladespire_ogre;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mobs_nether_drake";
    pNewScript->GetAI = &GetAI_mobs_nether_drake;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_daranelle";
    pNewScript->GetAI = &GetAI_npc_daranelle;
    pNewScript->RegisterSelf();
}
