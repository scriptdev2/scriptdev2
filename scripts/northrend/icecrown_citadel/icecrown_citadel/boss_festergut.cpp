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
SDName: boss_festergut
SD%Complete: 10%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_AGGRO                   = -1631082,
    SAY_BLIGHT                  = -1631083,
    SAY_SPORE                   = -1631084,
    SAY_PUNGUENT_BLIGHT         = -1631085,
    SAY_PUNGUENT_BLIGHT_EMOTE   = -1631086,
    SAY_SLAY_1                  = -1631087,
    SAY_SLAY_2                  = -1631088,
    SAY_BERSERK                 = -1631089,
    SAY_DEATH                   = -1631090,
    SAY_FESTERGUT_DEATH         = -1631091,
};

struct MANGOS_DLL_DECL boss_festergutAI : public ScriptedAI
{
    boss_festergutAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetMap()->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel *m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FESTERGUT, IN_PROGRESS);
    }

    void KilledUnit(Unit *pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FESTERGUT, FAIL);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FESTERGUT, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_festergut(Creature* pCreature)
{
    return new boss_festergutAI(pCreature);
}

void AddSC_boss_festergut()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_festergut";
    pNewScript->GetAI = &GetAI_boss_festergut;
    pNewScript->RegisterSelf();
}
