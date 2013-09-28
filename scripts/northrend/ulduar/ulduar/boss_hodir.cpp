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
SDName: boss_hodir
SD%Complete: 20%
SDComment: Basic script
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603086,
    SAY_SLAY_1                          = -1603087,
    SAY_SLAY_2                          = -1603088,
    SAY_FLASH_FREEZE                    = -1603089,
    SAY_FROZEN_BLOWS                    = -1603090,
    SAY_EPILOGUE                        = -1603091,
    SAY_BERSERK                         = -1603092,

    EMOTE_FLASH_FREEZE                  = -1603094,
    EMOTE_FROZEN_BLOWS                  = -1603095,

    // spells
    SPELL_BERSERK                       = 26662,
    SPELL_HODIR_CREDIT                  = 64899,                // kill credit spell; added in spell_template
    SPELL_SHATTER_CHEST                 = 65272,                // hard mode timer until chest is shattered; triggers 62501
    SPELL_FROZEN_BLOWS                  = 62478,
    SPELL_FROZEN_BLOWS_H                = 63512,
    SPELL_FREEZE                        = 62469,
    SPELL_BITTING_COLD                  = 62038,                // triggers 62039 and 62188
    SPELL_ICICLE_AURA                   = 62227,                // periodic targeting aura; triggers the spell which summons npc 33169
    SPELL_ICICLE_SNOWPACK               = 62476,                // cast right before Flash Freeze; triggers the spell which summons npc 33173
    SPELL_ICICLE_SNOWPACK_H             = 62477,
    SPELL_FLASH_FREEZE                  = 61968,                // main spell; sends event 20896

    // icicle spells
    SPELL_ICICLE                        = 62236,
    SPELL_ICICLE_DUMMY                  = 62453,

    // snowpacked icicle spells
    SPELL_ICICLE_ICE_SHARDS             = 62460,                // triggers the spell which summons npc 33174 and go 194173

    // snowpacked icicle target spells
    SPELL_SAFE_AREA                     = 65705,                // grant immunity from flash freeze

    // flash freeze related spells
    SPELL_FLASH_FREEZE_VISUAL           = 62148,                // cast by npc 30298
    SPELL_FLASH_FREEZE_SUMMON           = 61970,                // cast by all Flash Freeze targets; summons 32926
    SPELL_FLASH_FREEZE_SUMMON_NPC       = 61989,                // used to flash freeze all npc targets before the encounter; summons 32938
    // SPELL_FLASH_FREEZE_STUN          = 64175,                // use and purpose unk
    // SPELL_FLASH_FREEZE_FRIENDLY      = 64176,                // use and purpose unk

    // flash freeze spells
    SPELL_FLASH_FREEZE_AURA             = 61969,                // stuns the summoner
    SPELL_FLASH_FREEZE_KILL             = 62226,                // kill frozen targets

    // flash freeze npc spells
    SPELL_FLASH_FREEZE_AURA_NPC         = 61990,                // stuns the summoner (npc)
    SPELL_FLASH_FREEZE_INITIAL          = 62878,                // trigger aggro on Hodir if damaged

    // npcs
    NPC_ICICLE                          = 33169,
    NPC_SNOWPACKED_ICICLE               = 33173,
    NPC_SNOWPACKED_ICICLE_TARGET        = 33174,                // entry used to handle safe area aura from Flash Freeze
    NPC_FLASH_FREEZE                    = 32926,                // entry used during the encounter
    NPC_FLASH_FREEZE_NPC                = 32938,                // entry which stuns the friendly npcs before the actual fight

    GO_SNOWDRIFT                        = 194173,
    FACTION_ID_FRIENDLY                 = 35,
};

/*######
## boss_hodir
######*/

struct MANGOS_DLL_DECL boss_hodirAI : public ScriptedAI
{
    boss_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bEventFinished = false;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bEventFinished;

    uint32 m_uiBerserkTimer;

    void Reset() override
    {
        m_uiBerserkTimer = 8 * MINUTE * IN_MILLISECONDS;
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HODIR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_BITTING_COLD, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_ICICLE_AURA, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
        DoCastSpellIfCan(m_creature, SPELL_SHATTER_CHEST, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HODIR, FAIL);
    }

    void EnterEvadeMode() override
    {
        m_creature->RemoveAllAurasOnEvade();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive() && !m_bEventFinished)
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage) override
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            uiDamage = 0;

            if (!m_bEventFinished)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_HODIR, DONE);

                DoScriptText(SAY_EPILOGUE, m_creature);
                m_creature->CastSpell(m_creature, SPELL_HODIR_CREDIT, true);

                m_creature->ForcedDespawn(10000);
                m_creature->setFaction(FACTION_ID_FRIENDLY);
                m_bEventFinished = true;
                EnterEvadeMode();
            }
        }
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hodir";
    pNewScript->GetAI = GetAI_boss_hodir;
    pNewScript->RegisterSelf();
}
