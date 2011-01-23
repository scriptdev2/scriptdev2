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
SDName: Boss_Vaelastrasz
SD%Complete: 75
SDComment: Burning Adrenaline not correctly implemented in core
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"

enum
{
    SAY_LINE_1                  = -1469026,
    SAY_LINE_2                  = -1469027,
    SAY_LINE_3                  = -1469028,
    SAY_HALFLIFE                = -1469029,
    SAY_KILLTARGET              = -1469030,

    SPELL_ESSENCE_OF_THE_RED    = 23513,
    SPELL_FLAME_BREATH          = 23461,
    SPELL_FIRE_NOVA             = 23462,
    SPELL_TAIL_SWEEP            = 15847,
    SPELL_BURNING_ADRENALINE    = 23620,
    SPELL_CLEAVE                = 20684                   // Chain cleave is most likely named something different and contains a dummy effect
};

#define GOSSIP_ITEM         "Start Event <Needs Gossip Text>"

struct MANGOS_DLL_DECL boss_vaelastraszAI : public ScriptedAI
{
    boss_vaelastraszAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();

        // TODO Research what actually is supposed to happen here
        pCreature->setFaction(35);
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiPlayerGUID;
    uint32 m_uiSpeachTimer;
    uint32 m_uiSpeachNum;
    uint32 m_uiCleaveTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiFireNovaTimer;
    uint32 m_uiBurningAdrenalineCasterTimer;
    uint32 m_uiBurningAdrenalineTankTimer;
    uint32 m_uiTailSweepTimer;
    bool m_bHasYelled;
    bool mbIsDoingSpeach;

    void Reset()
    {
        m_uiPlayerGUID                   = 0;
        m_uiSpeachTimer                  = 0;
        m_uiSpeachNum                    = 0;
        m_uiCleaveTimer                  = 8000;            // These times are probably wrong
        m_uiFlameBreathTimer             = 11000;
        m_uiBurningAdrenalineCasterTimer = 15000;
        m_uiBurningAdrenalineTankTimer   = 45000;
        m_uiFireNovaTimer                = 5000;
        m_uiTailSweepTimer               = 20000;
        m_bHasYelled = false;
        mbIsDoingSpeach = false;
    }

    void BeginSpeach(Unit* target)
    {
        // Stand up and begin speach
        m_uiPlayerGUID = target->GetGUID();

        // 10 seconds
        DoScriptText(SAY_LINE_1, m_creature);

        m_uiSpeachTimer = 10000;
        m_uiSpeachNum = 0;
        mbIsDoingSpeach = true;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4))
            return;

        DoScriptText(SAY_KILLTARGET, m_creature, pVictim);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAELASTRASZ, IN_PROGRESS);

        DoCastSpellIfCan(m_creature, SPELL_ESSENCE_OF_THE_RED);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAELASTRASZ, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAELASTRASZ, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Speach
        if (mbIsDoingSpeach)
        {
            if (m_uiSpeachTimer < uiDiff)
            {
                switch (m_uiSpeachNum)
                {
                    case 0:
                        // 16 seconds till next line
                        DoScriptText(SAY_LINE_2, m_creature);
                        m_uiSpeachTimer = 16000;
                        ++m_uiSpeachNum;
                        break;
                    case 1:
                        // This one is actually 16 seconds but we only go to 10 seconds because he starts attacking after he says "I must fight this!"
                        DoScriptText(SAY_LINE_3, m_creature);
                        m_uiSpeachTimer = 10000;
                        ++m_uiSpeachNum;
                        break;
                    case 2:
                        m_creature->setFaction(103);
                        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.3));

                        if (m_uiPlayerGUID)
                        {
                            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                                AttackStart(pPlayer);

                            DoCastSpellIfCan(m_creature, SPELL_ESSENCE_OF_THE_RED);
                        }
                        m_uiSpeachTimer = 0;
                        mbIsDoingSpeach = false;
                        break;
                }
            }
            else
                m_uiSpeachTimer -= uiDiff;
        }

        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Yell if hp lower than 15%
        if (m_creature->GetHealthPercent() < 15.0f && !m_bHasYelled)
        {
            DoScriptText(SAY_HALFLIFE, m_creature);
            m_bHasYelled = true;
        }

        // Cleave Timer
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = 15000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Flame Breath Timer
        if (m_uiFlameBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_BREATH) == CAST_OK)
                m_uiFlameBreathTimer = urand(4000, 8000);
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Burning Adrenaline Caster Timer
        if (m_uiBurningAdrenalineCasterTimer < uiDiff)
        {
            Unit* pTarget = NULL;

            // TODO Target Selection must be improved!
            int i = 0 ;
            while (i < 3)                                   // max 3 tries to get a random target with power_mana
            {
                ++i;
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
                if (pTarget)
                    if (pTarget->getPowerType() == POWER_MANA)
                        i=3;
            }
            if (pTarget)                                    // cast on self (see below)
                pTarget->CastSpell(pTarget, SPELL_BURNING_ADRENALINE, true);

            m_uiBurningAdrenalineCasterTimer = 15000;
        }
        else
            m_uiBurningAdrenalineCasterTimer -= uiDiff;

        // Burning Adrenaline Tank Timer
        if (m_uiBurningAdrenalineTankTimer < uiDiff)
        {
            // have the victim cast the spell on himself otherwise the third effect aura will be applied
            // to Vael instead of the player
            m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_BURNING_ADRENALINE, true);

            m_uiBurningAdrenalineTankTimer = 45000;
        }
        else
            m_uiBurningAdrenalineTankTimer -= uiDiff;

        // Fire Nova Timer
        if (m_uiFireNovaTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FIRE_NOVA) == CAST_OK)
                m_uiFireNovaTimer = 5000;
        }
        else
            m_uiFireNovaTimer -= uiDiff;

        // Tail Sweep Timer
        if (m_uiTailSweepTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TAIL_SWEEP) == CAST_OK)
                m_uiTailSweepTimer = 20000;
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool GossipSelect_boss_vaelastrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)               // Fight time
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (boss_vaelastraszAI* pVaelAI = dynamic_cast<boss_vaelastraszAI*>(pCreature->AI()))
            pVaelAI->BeginSpeach((Unit*)pPlayer);
    }

    return true;
}

bool GossipHello_boss_vaelastrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());

    return true;
}

CreatureAI* GetAI_boss_vaelastrasz(Creature* pCreature)
{
    return new boss_vaelastraszAI(pCreature);
}

void AddSC_boss_vaelastrasz()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_vaelastrasz";
    pNewScript->GetAI = &GetAI_boss_vaelastrasz;
    pNewScript->pGossipHello = &GossipHello_boss_vaelastrasz;
    pNewScript->pGossipSelect = &GossipSelect_boss_vaelastrasz;
    pNewScript->RegisterSelf();
}
