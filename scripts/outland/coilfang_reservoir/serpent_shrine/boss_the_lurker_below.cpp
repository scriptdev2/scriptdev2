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
SDName: boss_the_lurker_below
SD%Complete: 5
SDComment: Timers from ACID, only placeholder quality, Spout unused, submerge phase missing
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"

enum
{
    SPELL_LURKER_SPAWN_TRIGGER      = 54587,
    SPELL_WHIRL                     = 37363,
    SPELL_GEYSER                    = 37478,
    SPELL_SPOUT                     = 37433,                // TODO should sweep the room 360degrees, related spells 37429 37430 37431
    SPELL_WATERBOLT                 = 37138,                // TODO is used when no enemy in melee range (unknown if on random or top-most aggro holder in this case
};

enum Phases
{
    PHASE_EMERGEING             = 0,                        // TODO unused for now
    PHASE_NORMAL                = 1,
    PHASE_SPOUT                 = 2,
    PHASE_SUBMERGED             = 3,
};

// TODO This boss should infact be a Scripted_NoMovementAI, but selecting only melee targets is not supported yet, change when implemented
struct MANGOS_DLL_DECL boss_the_lurker_belowAI : public ScriptedAI
{
    boss_the_lurker_belowAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Phases m_uiPhase;

    uint32 m_uiWhirlTimer;
    uint32 m_uiGeyserTimer;

    void Reset()
    {
        m_uiPhase = PHASE_NORMAL;

        m_uiWhirlTimer = 19500;
        m_uiGeyserTimer = 49700;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, NOT_STARTED);

        m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pVictim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        // Unclear if we will use this selecting for spout-alike situations
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            case PHASE_NORMAL:
                if (m_uiWhirlTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_WHIRL) == CAST_OK)
                        m_uiWhirlTimer = urand(15000, 30000);
                }
                else
                    m_uiWhirlTimer -= uiDiff;

                if (m_uiGeyserTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        if (DoCastSpellIfCan(pTarget, SPELL_GEYSER) == CAST_OK)
                            m_uiGeyserTimer = urand(49700, 60000);
                }
                else
                    m_uiGeyserTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
        }
    }
};

CreatureAI* GetAI_boss_the_lurker_below(Creature* pCreature)
{
    return new boss_the_lurker_belowAI(pCreature);
}

// Cast the spell that should summon the Lurker-Below
bool GOUse_go_strange_pool(Player* pPlayer, GameObject* pGo)
{
    // There is some chance to fish The Lurker Below, sources are from 20s to 10minutes, average 5min => 20 tries, hence 5%
    if (urand(0,99) < 5)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_THELURKER_EVENT) == NOT_STARTED)
            {
                pPlayer->CastSpell(pPlayer, SPELL_LURKER_SPAWN_TRIGGER, true);
                pInstance->SetData(TYPE_THELURKER_EVENT, IN_PROGRESS);
                return true;
            }
        }
    }
    return false;
}

void AddSC_boss_the_lurker_below()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_the_lurker_below";
    pNewScript->GetAI = &GetAI_boss_the_lurker_below;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_strange_pool";
    pNewScript->pGOUse = &GOUse_go_strange_pool;
    pNewScript->RegisterSelf();
}
