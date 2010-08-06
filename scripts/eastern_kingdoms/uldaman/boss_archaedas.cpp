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
SDName: Boss_Archaedas
SD%Complete: 60
SDComment: 
SDCategory: Uldaman
EndScriptData */

#include "precompiled.h"
#include "uldaman.h"

enum
{
    SPELL_GROUND_TREMMOR            = 6524,
    SPELL_AWAKEN_EARTHEN_GUARDIAN   = 10252,
    SPELL_STONE_DWARF_AWAKEN_VISUAL = 10254,
    SPELL_AWAKEN_VAULT_WARDER       = 10258,
    SPELL_AWAKEN_EARTHEN_DWARF      = 10259,
    SPELL_ARCHAEDAS_AWAKEN_VISUAL   = 10347,

    SAY_AGGRO                       = -1000000,
    SAY_AWAKE_GUARDIANS             = -1000000,
    SAY_AWAKE_WARDERS               = -1000000,
    SAY_UNIT_SLAIN                  = -1000000
};

struct MANGOS_DLL_DECL boss_archaedasAI : public ScriptedAI
{
    boss_archaedasAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_uldaman*)pCreature->GetInstanceData();
        Reset();
    }

    instance_uldaman* m_pInstance;

    uint32 m_uiAwakeningTimer;
    uint32 m_uiAwakeDwarfTimer;
    uint8 m_uiSubevent;
    bool m_bGuardiansAwaken;
    bool m_bWardersAwaken;

    void Reset()
    {
        m_uiAwakeningTimer  = 1000;
        m_uiSubevent        = 0;
        m_uiAwakeDwarfTimer = 10000;
        m_bGuardiansAwaken  = false;
        m_bWardersAwaken    = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->setFaction(FACTION_TITAN_NEUTRAL);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAEDAS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_UNIT_SLAIN, m_creature, NULL);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        // open door to vault (handled by instance script)
        m_pInstance->SetData(TYPE_ARCHAEDAS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAEDAS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // so many things are based in this script on instance data
        // so if we don't have access to it better do nothing
        if (!m_pInstance)
            return;

        // OOC Intro part triggered by Altar activation
        if (m_pInstance->GetData(TYPE_ARCHAEDAS) == SPECIAL)
        {
            if (m_uiAwakeningTimer <= uiDiff)
            {
                switch(m_uiSubevent)
                {
                    case 0:
                        m_creature->RemoveAurasDueToSpell(SPELL_STONED);
                        DoCastSpellIfCan(m_creature, SPELL_ARCHAEDAS_AWAKEN_VISUAL);
                        break;
                    case 1:
                        DoScriptText(SAY_AGGRO,m_creature,NULL);
                        break;
                    case 2:
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->setFaction(FACTION_TITAN_HOSTILE);
                        if (Unit* pUnit = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_EVENT_STARTER)))
                            AttackStart(pUnit);
                        else
                            EnterEvadeMode();
                        break;
                    default:
                        break;
                }

                ++m_uiSubevent;
                m_uiAwakeningTimer = 5000;
            }
            else
                m_uiAwakeningTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Awake random Dwarf
        if (m_creature->GetHealthPercent() >= 33.0f)
        {
            if (m_uiAwakeDwarfTimer <= uiDiff)
            {
                if (Creature* pDwarf = m_pInstance->GetDwarf())
                {
                    DoCastSpellIfCan(pDwarf, SPELL_AWAKEN_EARTHEN_DWARF, CAST_TRIGGERED);
                    // hack to remove
                    pDwarf->AI()->SpellHit(m_creature, GetSpellStore()->LookupEntry(SPELL_AWAKEN_EARTHEN_DWARF));
                }

                m_uiAwakeDwarfTimer = urand(9000, 12000);
            }
            else
                m_uiAwakeDwarfTimer -= uiDiff;
        }

        //Awake Earthen Guardians
        if (!m_bGuardiansAwaken && m_creature->GetHealthPercent() <= 66.0f)
        {
            if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_pInstance->SetData64(DATA_EVENT_STARTER, pUnit->GetGUID());

            DoScriptText(SAY_AWAKE_GUARDIANS, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_AWAKEN_EARTHEN_GUARDIAN, (CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS));

            // hack to remove
            m_pInstance->SimulateSpellHit(MOB_GUARDIAN, SPELL_AWAKEN_EARTHEN_GUARDIAN, m_creature);
            m_bGuardiansAwaken = true;
        }

        // Awake Warders
        if (!m_bWardersAwaken && m_creature->GetHealthPercent() <= 33.0f)
        {
            if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_pInstance->SetData64(DATA_EVENT_STARTER, pUnit->GetGUID());

            DoScriptText(SAY_AWAKE_WARDERS, m_creature, NULL);
            DoCastSpellIfCan(m_creature, SPELL_AWAKEN_VAULT_WARDER, (CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS));

            // hack to remove
            m_pInstance->SimulateSpellHit(MOB_VAULT_WARDER, SPELL_AWAKEN_VAULT_WARDER, m_creature);
            m_bWardersAwaken = true;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_archaeras_addAI : public ScriptedAI
{
    mob_archaeras_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_uldaman*)pCreature->GetInstanceData();
        Reset();
    }

    instance_uldaman* m_pInstance;
    uint32 m_uiAwakeningTimer;
    uint8 m_uiSubevent;
    bool m_bAwakening;

    void Reset()
    {
        m_uiAwakeningTimer = 0;
        m_uiSubevent = 0;
        m_bAwakening = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->setFaction(FACTION_TITAN_NEUTRAL);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        switch(m_creature->GetEntry())
        {
            case MOB_CUSTODIAN:
            case MOB_HALLSHAPER:
                if (pSpell->Id == SPELL_AWAKEN_EARTHEN_DWARF)
                    m_bAwakening = true;
                break;
            case MOB_VAULT_WARDER:
                if (pSpell->Id == SPELL_AWAKEN_VAULT_WARDER)
                    m_bAwakening = true;
                break;
            case MOB_GUARDIAN:
                if (pSpell->Id == SPELL_AWAKEN_EARTHEN_GUARDIAN)
                    m_bAwakening = true;
                break;
            default:
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // OOC part triggered by awakening spell hit
        if (m_bAwakening)
        {
            if (m_uiAwakeningTimer <= uiDiff)
            {
                switch(m_uiSubevent)
                {
                    case 0:
                        m_creature->RemoveAurasDueToSpell(SPELL_STONED);
                        DoCastSpellIfCan(m_creature, SPELL_STONE_DWARF_AWAKEN_VISUAL);
                        break;
                    case 1:
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->setFaction(FACTION_TITAN_HOSTILE);
                        if (Unit* pUnit = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_EVENT_STARTER)))
                        {
                            m_bAwakening = false;
                            AttackStart(pUnit);
                        }
                        else 
                            EnterEvadeMode();
                        break;
                    default:
                        break;
                }
                ++m_uiSubevent;
                m_uiAwakeningTimer = 2000;
            }
            else
                m_uiAwakeningTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archaedas(Creature* pCreature)
{
    return new boss_archaedasAI(pCreature);
}

CreatureAI* GetAI_mob_archaeras_add(Creature* pCreature)
{
    return new mob_archaeras_addAI(pCreature);
}

void AddSC_boss_archaedas()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_archaedas";
    pNewScript->GetAI = &GetAI_boss_archaedas;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_archaeras_add";
    pNewScript->GetAI = &GetAI_mob_archaeras_add;
    pNewScript->RegisterSelf();
}
