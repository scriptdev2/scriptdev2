/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Gothik
SD%Complete: 10
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_SPEECH                  = -1533040,                 // todo: split my text in 4 parts
    SAY_KILL                    = -1533041,
    SAY_DEATH                   = -1533042,
    SAY_TELEPORT                = -1533043,

    EMOTE_TO_FRAY               = -1533138,
    EMOTE_GATE                  = -1533139,

    PHASE_BALCONY               = 0,
    PHASE_GROUND                = 1,

    SPELL_TELEPORT_LEFT         = 28025,                    // guesswork
    SPELL_TELEPORT_RIGHT        = 28026,                    // could be defined as dead or live side, left or right facing north

    SPELL_HARVESTSOUL           = 28679,
    SPELL_SHADOWBOLT            = 29317,
    SPELL_SHADOWBOLT_H          = 56405,

    //Unrelenting Trainee
    SPELL_EAGLECLAW           = 30285,
    SPELL_KNOCKDOWN_PASSIVE   = 6961,

    //Unrelenting Deathknight
    SPELL_CHARGE              = 22120,
    SPELL_SHADOW_MARK         = 27825,

    //Unrelenting Rider
    SPELL_UNHOLY_AURA         = 55606,
    H_SPELL_UNHOLY_AURA       = 55608,
    SPELL_SHADOWBOLT_VOLLEY   = 27831,                      //Search thru targets and find those who have the SHADOW_MARK to cast this on
    H_SPELL_SHADOWBOLT_VOLLEY = 55638,

    //Spectral Trainee
    SPELL_ARCANE_EXPLOSION    = 27989,

    //Spectral Deathknight
    SPELL_WHIRLWIND           = 28334,
    SPELL_SUNDER_ARMOR        = 25051,                      //cannot find sunder that reduces armor by 2950
    SPELL_CLEAVE              = 20677,
    SPELL_MANA_BURN           = 17631,

    //Spectral Rider
    SPELL_LIFEDRAIN           = 24300,
    //USES SAME UNHOLY AURA AS UNRELENTING RIDER

    //Spectral Horse
    SPELL_STOMP               = 27993
};

enum eSpellDummy
{
    SPELL_A_TO_ANCHOR_1     = 27892,
    SPELL_B_TO_ANCHOR_1     = 27928,
    SPELL_C_TO_ANCHOR_1     = 27935,

    SPELL_A_TO_ANCHOR_2     = 27893,
    SPELL_B_TO_ANCHOR_2     = 27929,
    SPELL_C_TO_ANCHOR_2     = 27936,

    SPELL_A_TO_SKULL        = 27915,
    SPELL_B_TO_SKULL        = 27931,
    SPELL_C_TO_SKULL        = 27937
};

struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        //SetCombatMovement(false);                         // uncomment when more parts implemented.
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiPhase;
    uint32 m_uiSummonTraineeTimer;
    uint32 m_uiSummonDKTimer;
    uint32 m_uiSummonRiderTimer;

    void Reset()
    {
        m_uiPhase = PHASE_BALCONY;
        m_uiSummonTraineeTimer = 20000;
        m_uiSummonDKTimer = 45000;
        m_uiSummonRiderTimer = 55000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();

        DoScriptText(SAY_SPEECH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_GOTHIK, IN_PROGRESS);

        m_pInstance->SetGothTriggers();
    }

    bool HasPlayersInLeftSide()
    {
        Map::PlayerList const& lPlayers = m_pInstance->instance->GetPlayers();

        if (lPlayers.isEmpty())
            return false;

        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                if (!m_pInstance->IsInRightSideGothArea(pPlayer))
                    return true;
            }
        }

        return false;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, FAIL);
    }

    void SummonAdds(bool bRightSide, uint32 uiSummonEntry)
    {
        std::list<Creature*> lSummonList;
        m_pInstance->GetGothSummonPointCreatures(lSummonList, bRightSide);

        if (lSummonList.empty())
            return;

        uint8 uiCount = 2;

        switch(uiSummonEntry)
        {
            case NPC_UNREL_TRAINEE:
                lSummonList.sort(ObjectDistanceOrder(m_creature));
                break;
            case NPC_UNREL_DEATH_KNIGHT:
            case NPC_UNREL_RIDER:
                uiCount = 1;
                lSummonList.sort(ObjectDistanceOrderReversed(m_creature));
                break;
        }

        for(std::list<Creature*>::iterator itr = lSummonList.begin(); itr != lSummonList.end(); ++itr)
        {
            if (uiCount == 0)
                break;

            m_creature->SummonCreature(uiSummonEntry, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), (*itr)->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            --uiCount;
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (!m_pInstance)
            return;

        if (Creature* pAnchor = m_pInstance->GetClosestAnchorForGoth(pSummoned, true))
        {
            switch(pSummoned->GetEntry())
            {
                // Wrong caster, it expected to be pSummoned.
                // Mangos deletes the spell event at caster death, so for delayed spell like this
                // it's just a workaround. Does not affect other than the visual though (+ spell takes longer to "travel")
                case NPC_UNREL_TRAINEE:         m_creature->CastSpell(pAnchor, SPELL_A_TO_ANCHOR_1, true, NULL, NULL, pSummoned->GetGUID()); break;
                case NPC_UNREL_DEATH_KNIGHT:    m_creature->CastSpell(pAnchor, SPELL_B_TO_ANCHOR_1, true, NULL, NULL, pSummoned->GetGUID()); break;
                case NPC_UNREL_RIDER:           m_creature->CastSpell(pAnchor, SPELL_C_TO_ANCHOR_1, true, NULL, NULL, pSummoned->GetGUID()); break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gothik(Creature* pCreature)
{
    return new boss_gothikAI(pCreature);
}

bool EffectDummyCreature_spell_anchor(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiEffIndex != EFFECT_INDEX_0 || pCreatureTarget->GetEntry() != NPC_SUB_BOSS_TRIGGER)
        return true;

    instance_naxxramas* pInstance = (instance_naxxramas*)pCreatureTarget->GetInstanceData();

    if (!pInstance)
        return true;

    switch(uiSpellId)
    {
        case SPELL_A_TO_ANCHOR_1:                           // trigger mobs at high right side
        case SPELL_B_TO_ANCHOR_1:
        case SPELL_C_TO_ANCHOR_1:
        {
            if (Creature* pAnchor2 = pInstance->GetClosestAnchorForGoth(pCreatureTarget, false))
            {
                uint32 uiTriggered = SPELL_A_TO_ANCHOR_2;

                if (uiSpellId == SPELL_B_TO_ANCHOR_1)
                    uiTriggered = SPELL_B_TO_ANCHOR_2;
                else if (uiSpellId == SPELL_C_TO_ANCHOR_1)
                    uiTriggered = SPELL_C_TO_ANCHOR_2;

                pCreatureTarget->CastSpell(pAnchor2, uiTriggered, true);
            }

            return true;
        }
        case SPELL_A_TO_ANCHOR_2:                           // trigger mobs at high left side
        case SPELL_B_TO_ANCHOR_2:
        case SPELL_C_TO_ANCHOR_2:
        {
            std::list<Creature*> lTargets;
            pInstance->GetGothSummonPointCreatures(lTargets, false);

            if (!lTargets.empty())
            {
                std::list<Creature*>::iterator itr = lTargets.begin();
                uint32 uiPosition = urand(0, lTargets.size()-1);
                advance(itr, uiPosition);

                if (Creature* pTarget = (*itr))
                {
                    uint32 uiTriggered = SPELL_A_TO_SKULL;

                    if (uiSpellId == SPELL_B_TO_ANCHOR_2)
                        uiTriggered = SPELL_B_TO_SKULL;
                    else if (uiSpellId == SPELL_C_TO_ANCHOR_2)
                        uiTriggered = SPELL_C_TO_SKULL;

                    pCreatureTarget->CastSpell(pTarget, uiTriggered, true);
                }
            }
            return true;
        }
        case SPELL_A_TO_SKULL:                              // final destination trigger mob
        {
            pCreatureTarget->SummonCreature(NPC_SPECT_TRAINEE, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            return true;
        }
        case SPELL_B_TO_SKULL:
        {
            pCreatureTarget->SummonCreature(NPC_SPECT_DEATH_KNIGTH, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            return true;
        }
        case SPELL_C_TO_SKULL:
        {
            pCreatureTarget->SummonCreature(NPC_SPECT_RIDER, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            pCreatureTarget->SummonCreature(NPC_SPECT_HORSE, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            return true;
        }
    }

    return true;
};

void AddSC_boss_gothik()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_gothik";
    newscript->GetAI = &GetAI_boss_gothik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_anchor";
    newscript->pEffectDummyCreature = &EffectDummyCreature_spell_anchor;
    newscript->RegisterSelf();
}
