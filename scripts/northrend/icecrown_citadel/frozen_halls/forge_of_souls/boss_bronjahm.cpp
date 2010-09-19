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
SDName: boss_bronjahm
SD%Complete: 0%
SDComment: Placeholder
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    SAY_AGGRO_1                 = -1632000,                 // without sound, really correct?
    SAY_AGGRO_2                 = -1632001,
    SAY_SLAY_1                  = -1632002,
    SAY_SLAY_2                  = -1632003,
    SAY_DEATH                   = -1632004,
    SAY_SOULSTORM               = -1632005,
    SAY_CORRUPT_SOUL            = -1632006,
};

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_forge_of_souls* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(urand(0, 1) ? SAY_AGGRO_1 : SAY_AGGRO_2, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
    }
};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

void AddSC_boss_bronjahm()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_bronjahm";
    pNewScript->GetAI = &GetAI_boss_bronjahm;
    pNewScript->RegisterSelf();
}
