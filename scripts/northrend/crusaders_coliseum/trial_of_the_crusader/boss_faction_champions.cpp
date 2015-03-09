/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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
SDName: faction_champions
SD%Complete: 30
SDComment: Basic script only
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    SAY_GARROSH_PVP_A_SLAY_1            = -1649048,
    SAY_GARROSH_PVP_A_SLAY_2            = -1649049,
    SAY_GARROSH_PVP_A_SLAY_3            = -1649050,
    SAY_GARROSH_PVP_A_SLAY_4            = -1649051,

    SAY_VARIAN_PVP_H_SLAY_1             = -1649052,
    SAY_VARIAN_PVP_H_SLAY_2             = -1649053,
    SAY_VARIAN_PVP_H_SLAY_3             = -1649054,
    SAY_VARIAN_PVP_H_SLAY_4             = -1649055,

    SPELL_PVP_TRINKET                   = 65547,
};

/*######
## trial_crusader_common
######*/

struct trial_crusader_commonAI : public ScriptedAI
{
    trial_crusader_commonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    void Reset() override
    {
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_FACTION_CHAMPIONS) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_FACTION_CHAMPIONS, IN_PROGRESS);
        }
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (!m_pInstance)
            return;

        Creature* pSpeaker = m_pInstance->GetSingleCreatureFromStorage(m_pInstance->GetPlayerTeam() == ALLIANCE ? NPC_GARROSH : NPC_VARIAN);
        if (!pSpeaker)
            return;

        switch (urand(0, 3))
        {
            case 0: DoScriptText(m_pInstance->GetPlayerTeam() == ALLIANCE ? SAY_GARROSH_PVP_A_SLAY_1 : SAY_VARIAN_PVP_H_SLAY_1, pSpeaker); break;
            case 1: DoScriptText(m_pInstance->GetPlayerTeam() == ALLIANCE ? SAY_GARROSH_PVP_A_SLAY_2 : SAY_VARIAN_PVP_H_SLAY_2, pSpeaker); break;
            case 2: DoScriptText(m_pInstance->GetPlayerTeam() == ALLIANCE ? SAY_GARROSH_PVP_A_SLAY_3 : SAY_VARIAN_PVP_H_SLAY_3, pSpeaker); break;
            case 3: DoScriptText(m_pInstance->GetPlayerTeam() == ALLIANCE ? SAY_GARROSH_PVP_A_SLAY_4 : SAY_VARIAN_PVP_H_SLAY_4, pSpeaker); break;
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_FACTION_CHAMPIONS) != FAIL)
                m_pInstance->SetData(TYPE_FACTION_CHAMPIONS, FAIL);
        }
    }

    // Return true to handle shared timers and MeleeAttack
    virtual bool UpdateCrusaderAI(const uint32 /*uiDiff*/) { return true; }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Call specific virtual function
        if (!UpdateCrusaderAI(uiDiff))
            return;

        DoMeleeAttackIfReady();
    }
};

enum
{
    // death knight spells
};

/*######
## boss_crusader_death_knight
######*/

struct boss_crusader_death_knightAI : public trial_crusader_commonAI
{
    boss_crusader_death_knightAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_death_knight(Creature* pCreature)
{
    return new boss_crusader_death_knightAI(pCreature);
}

enum
{
    // druid balance spells
};

/*######
## boss_crusader_druid_balance
######*/

struct boss_crusader_druid_balanceAI : public trial_crusader_commonAI
{
    boss_crusader_druid_balanceAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_druid_balance(Creature* pCreature)
{
    return new boss_crusader_druid_balanceAI(pCreature);
}

enum
{
    // druid restoration spells
};

/*######
## boss_crusader_druid_resto
######*/

struct boss_crusader_druid_restoAI : public trial_crusader_commonAI
{
    boss_crusader_druid_restoAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_druid_resto(Creature* pCreature)
{
    return new boss_crusader_druid_restoAI(pCreature);
}

enum
{
    // hunter spells
    SPELL_CALL_PET                  = 67777,
};

/*######
## boss_crusader_hunter
######*/

struct boss_crusader_hunterAI : public trial_crusader_commonAI
{
    boss_crusader_hunterAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* /*pInvoker*/, uint32 /*uiMiscValue*/) override
    {
        if (eventType == AI_EVENT_CUSTOM_A)
            DoCastSpellIfCan(m_creature, SPELL_CALL_PET);
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_hunter(Creature* pCreature)
{
    return new boss_crusader_hunterAI(pCreature);
}

enum
{
    // mage spells
};

/*######
## boss_crusader_mage
######*/

struct boss_crusader_mageAI : public trial_crusader_commonAI
{
    boss_crusader_mageAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_mage(Creature* pCreature)
{
    return new boss_crusader_mageAI(pCreature);
}

enum
{
    // paladin holy spells
};

/*######
## boss_crusader_paladin_holy
######*/

struct boss_crusader_paladin_holyAI : public trial_crusader_commonAI
{
    boss_crusader_paladin_holyAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_paladin_holy(Creature* pCreature)
{
    return new boss_crusader_paladin_holyAI(pCreature);
}

enum
{
    // paladin retribution spells
};

/*######
## boss_crusader_paladin_retri
######*/

struct boss_crusader_paladin_retriAI : public trial_crusader_commonAI
{
    boss_crusader_paladin_retriAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_paladin_retri(Creature* pCreature)
{
    return new boss_crusader_paladin_retriAI(pCreature);
}

enum
{
    // priest discipline spells
};

/*######
## boss_crusader_priest_disc
######*/

struct boss_crusader_priest_discAI : public trial_crusader_commonAI
{
    boss_crusader_priest_discAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_priest_disc(Creature* pCreature)
{
    return new boss_crusader_priest_discAI(pCreature);
}

enum
{
    // priest shadow spells
};

/*######
## boss_crusader_priest_shadow
######*/

struct boss_crusader_priest_shadowAI : public trial_crusader_commonAI
{
    boss_crusader_priest_shadowAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_priest_shadow(Creature* pCreature)
{
    return new boss_crusader_priest_shadowAI(pCreature);
}

enum
{
    // rogue spells
};

/*######
## boss_crusader_rogue
######*/

struct boss_crusader_rogueAI : public trial_crusader_commonAI
{
    boss_crusader_rogueAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_rogue(Creature* pCreature)
{
    return new boss_crusader_rogueAI(pCreature);
}

enum
{
    // shaman enhancement spells
};

/*######
## boss_crusader_shaman_enha
######*/

struct boss_crusader_shaman_enhaAI : public trial_crusader_commonAI
{
    boss_crusader_shaman_enhaAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_shaman_enha(Creature* pCreature)
{
    return new boss_crusader_shaman_enhaAI(pCreature);
}

enum
{
    // shaman restoration spells
};

/*######
## boss_crusader_shaman_resto
######*/

struct boss_crusader_shaman_restoAI : public trial_crusader_commonAI
{
    boss_crusader_shaman_restoAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_shaman_resto(Creature* pCreature)
{
    return new boss_crusader_shaman_restoAI(pCreature);
}

enum
{
    // warlock spells
    SPELL_SUMMON_FELHUNTER          = 67514,
};

/*######
## boss_crusader_warlock
######*/

struct boss_crusader_warlockAI : public trial_crusader_commonAI
{
    boss_crusader_warlockAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* /*pInvoker*/, uint32 /*uiMiscValue*/) override
    {
        if (eventType == AI_EVENT_CUSTOM_A)
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_FELHUNTER);
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_warlock(Creature* pCreature)
{
    return new boss_crusader_warlockAI(pCreature);
}

enum
{
    // warrior spells
};

/*######
## boss_crusader_warrior
######*/

struct boss_crusader_warriorAI : public trial_crusader_commonAI
{
    boss_crusader_warriorAI(Creature* pCreature) : trial_crusader_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_crusader_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_boss_crusader_warrior(Creature* pCreature)
{
    return new boss_crusader_warriorAI(pCreature);
}

void AddSC_boss_faction_champions()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_death_knight";
    pNewScript->GetAI = &GetAI_boss_crusader_death_knight;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_druid_balance";
    pNewScript->GetAI = &GetAI_boss_crusader_druid_balance;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_druid_resto";
    pNewScript->GetAI = &GetAI_boss_crusader_druid_resto;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_hunter";
    pNewScript->GetAI = &GetAI_boss_crusader_hunter;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_mage";
    pNewScript->GetAI = &GetAI_boss_crusader_mage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_paladin_holy";
    pNewScript->GetAI = &GetAI_boss_crusader_paladin_holy;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_paladin_retri";
    pNewScript->GetAI = &GetAI_boss_crusader_paladin_retri;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_priest_disc";
    pNewScript->GetAI = &GetAI_boss_crusader_priest_disc;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_priest_shadow";
    pNewScript->GetAI = &GetAI_boss_crusader_priest_shadow;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_rogue";
    pNewScript->GetAI = &GetAI_boss_crusader_rogue;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_shaman_enha";
    pNewScript->GetAI = &GetAI_boss_crusader_shaman_enha;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_shaman_resto";
    pNewScript->GetAI = &GetAI_boss_crusader_shaman_resto;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_warlock";
    pNewScript->GetAI = &GetAI_boss_crusader_warlock;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_crusader_warrior";
    pNewScript->GetAI = &GetAI_boss_crusader_warrior;
    pNewScript->RegisterSelf();
}
