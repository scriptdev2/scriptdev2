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
SDName: mob_anubisath_sentinel
SD%Complete: 75
SDComment: Abilities selection needs further improvements. Shadow storm is not properly implemented in core it should only target ppl outside of melee range.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_PERIODIC_MANA_BURN        = 812,
    SPELL_MENDING                   = 2147,
    SPELL_PERIODIC_SHADOW_STORM     = 2148,
    SPELL_PERIODIC_THUNDERCLAP      = 2834,
    SPELL_MORTAL_STRIKE             = 9347,
    SPELL_FIRE_ARCANE_REFLECT       = 13022,
    SPELL_SHADOW_FROST_REFLECT      = 19595,
    SPELL_PERIODIC_KNOCK_AWAY       = 21737,
    SPELL_THORNS                    = 25777,

    MAX_BUDDY                       = 4
};

struct MANGOS_DLL_DECL npc_anubisath_sentinelAI : public ScriptedAI
{
    npc_anubisath_sentinelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_lAssistList.clear();
        Reset();
    }

    uint32 m_uiMyAbility;
    uint32 m_uiMyTransfer;

    std::list<uint64> m_lAssistList;

    void Reset()
    {
        m_uiMyAbility = 0;
        m_uiMyTransfer = 0;
    }

    void JustReachedHome()
    {
        for(std::list<uint64>::iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
        {
            if (Creature* pBuddy = (Creature*)Unit::GetUnit(*m_creature, *itr))
            {
                if (pBuddy->isDead())
                    pBuddy->Respawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        SetAbility();
        InitSentinelsNear(pWho);
        DoTransferAbility(false);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!m_uiMyAbility && pCaster->GetEntry() == m_creature->GetEntry())
            m_uiMyAbility = pSpell->Id;
    }

    void JustDied(Unit* pKiller)
    {
        DoTransferAbility(true);
    }

    void SetAbility()
    {
        switch(urand(0, 8))
        {
            case 0: m_uiMyTransfer = SPELL_MENDING; break;
            case 1: m_uiMyTransfer = SPELL_PERIODIC_KNOCK_AWAY; break;
            case 2: m_uiMyTransfer = SPELL_PERIODIC_MANA_BURN; break;
            case 3: m_uiMyTransfer = SPELL_FIRE_ARCANE_REFLECT; break;
            case 4: m_uiMyTransfer = SPELL_SHADOW_FROST_REFLECT; break;
            case 5: m_uiMyTransfer = SPELL_THORNS; break;
            case 6: m_uiMyTransfer = SPELL_PERIODIC_THUNDERCLAP; break;
            case 7: m_uiMyTransfer = SPELL_MORTAL_STRIKE; break;
            case 8: m_uiMyTransfer = SPELL_PERIODIC_SHADOW_STORM; break;
        }
    }

    void DoTransferAbility(bool bAtDeath)
    {
        Unit* pSource = NULL;
        Unit* pTarget = NULL;

        for(std::list<uint64>::iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
        {
            Unit* pBuddy = Unit::GetUnit(*m_creature, *itr);

            if (pTarget)
                pTarget = pSource;
            else
                pTarget = m_creature;

            pSource = pBuddy;

            if (pSource && pTarget && pTarget->isAlive())
            {
                pSource->CastSpell(pTarget, bAtDeath ? m_uiMyAbility : m_uiMyTransfer, true);

                if (bAtDeath)
                    pTarget->SetHealth(pTarget->GetMaxHealth());
                else
                    return;
            }
        }
    }

    void InitSentinelsNear(Unit* pTarget)
    {
        if (!m_lAssistList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
            {
                if (Creature* pBuddy = (Creature*)Unit::GetUnit(*m_creature, *itr))
                {
                    if (pBuddy->isAlive())
                        pBuddy->AI()->AttackStart(pTarget);
                }
            }

            return;
        }

        std::list<Creature*> lAssistList;
        GetCreatureListWithEntryInGrid(lAssistList, m_creature, m_creature->GetEntry(), 70.0f);

        if (lAssistList.empty())
            return;

        for(std::list<Creature*>::iterator iter = lAssistList.begin(); iter != lAssistList.end(); ++iter)
        {
            m_lAssistList.push_back((*iter)->GetGUID());
            (*iter)->AI()->AttackStart(pTarget);
        }

        if (m_lAssistList.size() != MAX_BUDDY)
            error_log("SD2: npc_anubisath_sentinel found too few/too many buddies, expected %u.", MAX_BUDDY);
    }
};

CreatureAI* GetAI_npc_anubisath_sentinel(Creature* pCreature)
{
    return new npc_anubisath_sentinelAI(pCreature);
}

void AddSC_mob_anubisath_sentinel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_anubisath_sentinel";
    newscript->GetAI = &GetAI_npc_anubisath_sentinel;
    newscript->RegisterSelf();
}
