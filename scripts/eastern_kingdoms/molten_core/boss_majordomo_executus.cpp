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
SDName: Boss_Majordomo_Executus
SD%Complete: 90
SDComment: Ragnaros summon missing
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "TemporarySummon.h"

enum
{
    SAY_AGGRO               = -1409003,
    SAY_SLAY                = -1409005,
    SAY_SPECIAL             = -1409006,                     // Use unknown
    SAY_LAST_ADD            = -1409019,                     // When only one add remaining
    SAY_DEFEAT_1            = -1409007,
    SAY_DEFEAT_2            = -1409020,
    SAY_DEFEAT_3            = -1409021,

    SAY_SUMMON_MAJ          = -1409008,
    SAY_ARRIVAL1_RAG        = -1409009,
    SAY_ARRIVAL2_MAJ        = -1409010,
    SAY_ARRIVAL3_RAG        = -1409011,
    SAY_ARRIVAL4_MAJ        = -1409022,
    SAY_ARRIVAL5_RAG        = -1409012,

    SPELL_MAGIC_REFLECTION  = 20619,
    SPELL_DAMAGE_REFLECTION = 21075,
    SPELL_BLASTWAVE         = 20229,
    SPELL_AEGIS             = 20620,
    SPELL_TELEPORT          = 20618,

    SPELL_TELEPORT_SELF     = 19484,
    SPELL_SUMMON_RAGNAROS   = 19774,
};

struct MANGOS_DLL_DECL boss_majordomoAI : public ScriptedAI
{
    boss_majordomoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_molten_core*)pCreature->GetInstanceData();
        Reset();
    }

    instance_molten_core* m_pInstance;

    uint32 m_uiMagicReflectionTimer;
    uint32 m_uiDamageReflectionTimer;
    uint32 m_uiBlastwaveTimer;
    uint32 m_uiTeleportTimer;
    uint32 m_uiAegisTimer;
    uint32 m_uiSpeechTimer;

    bool m_bHasEncounterFinished;
    uint8 m_uiAddsKilled;
    uint8 m_uiSpeech;
    GUIDList m_luiMajordomoAddsGUIDs;

    void Reset()
    {
        m_uiMagicReflectionTimer  = 30000;                  // Damage reflection first so we alternate
        m_uiDamageReflectionTimer = 15000;
        m_uiBlastwaveTimer = 10000;
        m_uiTeleportTimer = 20000;
        m_uiAegisTimer = 5000;
        m_uiSpeechTimer = 1000;

        m_bHasEncounterFinished = false;
        m_uiAddsKilled = 0;
        m_uiSpeech = 0;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_bHasEncounterFinished = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAJORDOMO, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (!m_bHasEncounterFinished)                        // Normal reached home, FAIL
        {
            if (m_pInstance)
            {
                m_pInstance->SetData(TYPE_MAJORDOMO, FAIL);
                m_pInstance->DoHandleAdds(m_luiMajordomoAddsGUIDs);
            }
        }
        else                                                 // Finished the encounter, DONE
        {
            // Exit combat
            m_creature->RemoveAllAuras();
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            m_creature->LoadCreatureAddon();
            m_creature->SetLootRecipient(NULL);

            // Set friendly
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            m_creature->setFaction(FACTION_MAJORDOMO_FRIENDLY);

            // Start his speech
            m_uiSpeechTimer = 1;                        // At next tick
            m_uiSpeech = 1;

            m_pInstance->SetData(TYPE_MAJORDOMO, DONE);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_FLAMEWAKER_HEALER || pSummoned->GetEntry() == NPC_FLAMEWAKER_ELITE)
            m_luiMajordomoAddsGUIDs.push_back(pSummoned->GetGUID());
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_FLAMEWAKER_HEALER || pSummoned->GetEntry() == NPC_FLAMEWAKER_ELITE)
            m_uiAddsKilled += 1;

        // Yell if only one Add alive
        if (m_uiAddsKilled == m_luiMajordomoAddsGUIDs.size() - 1)
            DoScriptText(SAY_LAST_ADD, m_creature);

        // All adds are killed, retreat
        else if (m_uiAddsKilled == m_luiMajordomoAddsGUIDs.size())
        {
            m_bHasEncounterFinished = true;
            m_creature->GetMotionMaster()->MoveTargetedHome();
        }
    }

    // Unsummon Majordomo adds
    void UnsummonMajordomoAdds()
    {
        for (std::list<uint64>::const_iterator itr = m_luiMajordomoAddsGUIDs.begin(); itr != m_luiMajordomoAddsGUIDs.end(); ++itr)
        {
            if (Creature* pAdd = m_creature->GetMap()->GetCreature(*itr))
                if (pAdd->IsTemporarySummon())
                    ((TemporarySummon*)pAdd)->UnSummon();
        }

        m_luiMajordomoAddsGUIDs.clear();
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            DoCastSpellIfCan(m_creature, SPELL_AEGIS, CAST_TRIGGERED);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Handling of his combat-end speech and Ragnaros summoning
        if (m_uiSpeech)
        {
            if (m_uiSpeechTimer < uiDiff)
            {
                switch (m_uiSpeech)
                {
                    case 1:
                        DoScriptText(SAY_DEFEAT_1, m_creature);
                        m_uiSpeechTimer = 7500;
                        ++m_uiSpeech;
                        break;
                    case 2:
                        DoScriptText(SAY_DEFEAT_2, m_creature);
                        m_uiSpeechTimer = 8000;
                        ++m_uiSpeech;
                        break;
                    case 3:
                        DoScriptText(SAY_DEFEAT_3, m_creature);
                        m_uiSpeechTimer = 21500;
                        ++m_uiSpeech;
                        break;
                    case 4:
                        DoCastSpellIfCan(m_creature, SPELL_TELEPORT_SELF);
                        m_uiSpeechTimer = 1100;
                        ++m_uiSpeech;
                        break;
                    case 5:
                        // Majordomo is away now, remove his adds (TODO: perhaps handle them in a different way, unclear)
                        UnsummonMajordomoAdds();
                        m_uiSpeech = 0;
                        break;
                        // TODO: teleport to ragnaros, implement ragnaros speech here, teleport might be to handled as part of the tele-self spell
                }
            }
            else
                m_uiSpeechTimer -= uiDiff;
        }

        // When encounter finished, no need to do anything anymore (important for moving home after victory)
        if (m_bHasEncounterFinished)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Cast Ageis to heal self
        if (m_uiAegisTimer <= uiDiff)
            m_uiAegisTimer = 0;
        else
            m_uiAegisTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 90.0f && !m_uiAegisTimer)
        {
            DoCastSpellIfCan(m_creature, SPELL_AEGIS);
            m_uiAegisTimer = 10000;
        }

        // Magic Reflection Timer
        if (m_uiMagicReflectionTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_MAGIC_REFLECTION) == CAST_OK)
                m_uiMagicReflectionTimer = 30000;
        }
        else
            m_uiMagicReflectionTimer -= uiDiff;

        // Damage Reflection Timer
        if (m_uiDamageReflectionTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DAMAGE_REFLECTION) == CAST_OK)
                m_uiDamageReflectionTimer = 30000;
        }
        else
            m_uiDamageReflectionTimer -= uiDiff;

        // Teleports the target to the heated rock in the center of the area
        if (m_uiTeleportTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_TELEPORT) == CAST_OK)
                    m_uiTeleportTimer = 20000;
            }
        }
        else
            m_uiTeleportTimer -= uiDiff;

        // Blastwave Timer
        if (m_uiBlastwaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BLASTWAVE) == CAST_OK)
                m_uiBlastwaveTimer = 10000;
        }
        else
            m_uiBlastwaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_majordomo(Creature* pCreature)
{
    return new boss_majordomoAI(pCreature);
}

void AddSC_boss_majordomo()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_majordomo";
    pNewScript->GetAI = &GetAI_boss_majordomo;
    pNewScript->RegisterSelf();
}
