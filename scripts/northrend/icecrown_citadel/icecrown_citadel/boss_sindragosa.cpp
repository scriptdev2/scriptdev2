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
SDName: boss_sindragosa
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_AGGRO                   = -1631148,
    SAY_UNCHAINED_MAGIC         = -1631149,
    SAY_BLISTERING_COLD         = -1631150,
    SAY_RESPIRE                 = -1631151,
    SAY_TAKEOFF                 = -1631152,
    SAY_PHASE_3                 = -1631153,
    SAY_SLAY_1                  = -1631154,
    SAY_SLAY_2                  = -1631155,
    SAY_BERSERK                 = -1631156,
    SAY_DEATH                   = -1631157,
};

struct MANGOS_DLL_DECL boss_sindragosaAI : public ScriptedAI
{
    boss_sindragosaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SINDRAGOSA, DONE);
    }

    void MovementInform(uint32 uiMovementType, uint32 uiPointId)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sindragosa(Creature* pCreature)
{
    return new boss_sindragosaAI(pCreature);
}


struct MANGOS_DLL_DECL npc_rimefang_iccAI : public ScriptedAI
{
    npc_rimefang_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_rimefang_icc(Creature* pCreature)
{
    return new npc_rimefang_iccAI(pCreature);
}


struct MANGOS_DLL_DECL npc_spinestalker_iccAI : public ScriptedAI
{
    npc_spinestalker_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_spinestalker_icc(Creature* pCreature)
{
    return new npc_spinestalker_iccAI(pCreature);
}

void AddSC_boss_sindragosa()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_sindragosa";
    pNewScript->GetAI = &GetAI_boss_sindragosa;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_rimefang_icc";
    pNewScript->GetAI = &GetAI_npc_rimefang_icc;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_spinestalker_icc";
    pNewScript->GetAI = &GetAI_npc_spinestalker_icc;
    pNewScript->RegisterSelf();
}
