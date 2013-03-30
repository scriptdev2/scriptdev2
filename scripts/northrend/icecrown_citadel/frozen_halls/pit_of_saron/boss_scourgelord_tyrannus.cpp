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
SDName: boss_scourgelord_tyrannus
SD%Complete: 20
SDComment: Basic script
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_AGGRO                           = -1658053,
    SAY_SLAY_1                          = -1658054,
    SAY_SLAY_2                          = -1658055,
    SAY_DEATH                           = -1658056,
    SAY_MARK                            = -1658057,
    SAY_SMASH                           = -1658058,

    EMOTE_RIMEFANG_ICEBOLT              = -1658059,
    EMOTE_SMASH                         = -1658060,
};

static const float afRimefangExitPos[3] = {1248.29f, 145.924f, 733.914f};

/*######
## boss_tyrannus
######*/

struct MANGOS_DLL_DECL boss_tyrannusAI : public ScriptedAI
{
    boss_tyrannusAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;

    void Reset() override
    {
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_TYRANNUS, IN_PROGRESS);

            // Set Rimefang in combat
            if (Creature* pRimefang = m_pInstance->GetSingleCreatureFromStorage(NPC_RIMEFANG))
                pRimefang->AI()->AttackStart(pWho);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller) override
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_TYRANNUS, DONE);

            // Move Rimefang out of the area
            if (Creature* pRimefang = m_pInstance->GetSingleCreatureFromStorage(NPC_RIMEFANG))
            {
                pRimefang->AI()->EnterEvadeMode();
                pRimefang->SetWalk(false);
                pRimefang->ForcedDespawn(25000);
                pRimefang->GetMotionMaster()->MovePoint(0, afRimefangExitPos[0], afRimefangExitPos[1], afRimefangExitPos[2]);
            }

            // Move the general near the boss - ToDo: move the other freed slaves as well
            if (Creature* pGeneral = m_pInstance->GetSingleCreatureFromStorage(m_pInstance->GetPlayerTeam() == HORDE ? NPC_IRONSKULL_PART2 : NPC_VICTUS_PART2))
            {
                float fX, fY, fZ;
                pGeneral->SetWalk(false);
                m_creature->GetContactPoint(pGeneral, fX, fY, fZ, INTERACTION_DISTANCE);
                pGeneral->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
            }
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_tyrannus(Creature* pCreature)
{
    return new boss_tyrannusAI (pCreature);
}

/*######
## boss_rimefang_pos
######*/

struct MANGOS_DLL_DECL boss_rimefang_posAI : public ScriptedAI
{
    boss_rimefang_posAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        m_uiMountTimer = 1000;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiMountTimer;

    void Reset() override { }

    void EnterEvadeMode() override
    {
        m_creature->RemoveAllAurasOnEvade();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreatureAddon(true);

        m_creature->SetLootRecipient(NULL);

        Reset();

        // Don't handle movement.
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_pInstance)
            return;

        // He needs to be mounted manually, not by vehicle_accessories
        /*if (m_uiMountTimer)
        {
            if (m_uiMountTimer <= uiDiff)
            {
                if (Creature* pTyrannus = m_pInstance->GetSingleCreatureFromStorage(NPC_TYRANNUS))
                    pTyrannus->CastSpell(m_creature, SPELL_RIDE_VEHICLE_HARDCODED, true);

                m_uiMountTimer = 0;
            }
            else
                m_uiMountTimer -= uiDiff;
        }*/

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_boss_rimefang_pos(Creature* pCreature)
{
    return new boss_rimefang_posAI(pCreature);
}

void AddSC_boss_tyrannus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_tyrannus";
    pNewScript->GetAI = &GetAI_boss_tyrannus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_rimefang_pos";
    pNewScript->GetAI = &GetAI_boss_rimefang_pos;
    pNewScript->RegisterSelf();
}
