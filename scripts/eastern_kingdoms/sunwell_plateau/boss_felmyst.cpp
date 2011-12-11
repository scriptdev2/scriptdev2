/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_felmyst
SD%Complete: 20%
SDComment: Spawn support only
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum
{
    SAY_INTRO           = -1580036,
    SAY_KILL_1          = -1580037,
    SAY_KILL_2          = -1580038,
    SAY_DEATH           = -1580042,
    SAY_TAKEOFF         = -1580040,
    SAY_BREATH          = -1580039,
    SAY_BERSERK         = -1580041,

    SPELL_FELBLAZE_VISUAL   = 45068,            // Visual transform aura
};

struct MANGOS_DLL_DECL boss_felmystAI : public ScriptedAI
{
    boss_felmystAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_sunwell_plateau*)pCreature->GetInstanceData();
        m_bHasTransformed = false;
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    bool m_bHasTransformed;

    void Reset()
    {
        // Transform into Felmyst dragon
        DoCastSpellIfCan(m_creature, SPELL_FELBLAZE_VISUAL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTransformed)
        {
            if (pWho->GetTypeId() == TYPEID_PLAYER && pWho->IsWithinLOSInMap(m_creature) && pWho->IsWithinDistInMap(m_creature, 100.0f))
            {
                DoScriptText(SAY_INTRO, m_creature);
                m_bHasTransformed = true;
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        // Add the visual aura back when evading - workaround because there is no way to remove only the negative auras
        DoCastSpellIfCan(m_creature, SPELL_FELBLAZE_VISUAL, CAST_TRIGGERED);

        if (m_creature->isAlive())
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FELMYST, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_felmyst(Creature* pCreature)
{
    return new boss_felmystAI(pCreature);
}

void AddSC_boss_felmyst()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_felmyst";
    pNewScript->GetAI = &GetAI_boss_felmyst;
    pNewScript->RegisterSelf();
}
