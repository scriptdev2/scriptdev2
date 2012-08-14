/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Tomb_Of_Seven
SD%Complete: 100
SDComment:
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "blackrock_depths.h"

enum
{
    SPELL_SHADOW_BOLT_VOLLEY    = 15245,
    SPELL_IMMOLATE              = 12742,
    SPELL_CURSE_OF_WEAKNESS     = 12493,
    SPELL_DEMON_ARMOR           = 13787,
    SPELL_SUMMON_VOIDWALKERS    = 15092,

    SAY_DOOMREL_START_EVENT     = -1230003,
    GOSSIP_ITEM_CHALLENGE       = -3230002,
    GOSSIP_TEXT_ID_CHALLENGE    = 2601,
};

struct MANGOS_DLL_DECL boss_doomrelAI : public ScriptedAI
{
    boss_doomrelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowVolleyTimer;
    uint32 m_uiImmolateTimer;
    uint32 m_uiCurseOfWeaknessTimer;
    uint32 m_uiDemonArmorTimer;

    void Reset()
    {
        m_uiShadowVolleyTimer    = 10000;
        m_uiImmolateTimer        = 18000;
        m_uiCurseOfWeaknessTimer = 5000;
        m_uiDemonArmorTimer      = 1000;
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_VOIDWALKERS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShadowVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_BOLT_VOLLEY) == CAST_OK)
                m_uiShadowVolleyTimer = 12000;
        }
        else
            m_uiShadowVolleyTimer -= uiDiff;

        if (m_uiImmolateTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_IMMOLATE) == CAST_OK)
                    m_uiImmolateTimer = 25000;
            }
        }
        else
            m_uiImmolateTimer -= uiDiff;

        if (m_uiCurseOfWeaknessTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CURSE_OF_WEAKNESS) == CAST_OK)
                    m_uiCurseOfWeaknessTimer = 45000;
            }
        }
        else
            m_uiCurseOfWeaknessTimer -= uiDiff;

        if (m_uiDemonArmorTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DEMON_ARMOR) == CAST_OK)
                m_uiDemonArmorTimer = 300000;
        }
        else
            m_uiDemonArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_doomrel(Creature* pCreature)
{
    return new boss_doomrelAI(pCreature);
}

bool GossipHello_boss_doomrel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED || pInstance->GetData(TYPE_TOMB_OF_SEVEN) == FAIL)
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHALLENGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_CHALLENGE, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_boss_doomrel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            DoScriptText(SAY_DOOMREL_START_EVENT, pCreature);
            // start event
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
                pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);

            break;
    }
    return true;
}

void AddSC_boss_tomb_of_seven()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_doomrel";
    pNewScript->GetAI = &GetAI_boss_doomrel;
    pNewScript->pGossipHello = &GossipHello_boss_doomrel;
    pNewScript->pGossipSelect = &GossipSelect_boss_doomrel;
    pNewScript->RegisterSelf();
}
