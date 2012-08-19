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
SDName: boss_rotface
SD%Complete: 10%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_AGGRO                   = -1631071,
    SAY_SLIME_SPRAY             = -1631072,
    SAY_OOZE_EXPLODE            = -1631073,
    SAY_SLIME_FLOW_1            = -1631074,
    SAY_SLIME_FLOW_2            = -1631075,
    SAY_SLAY_1                  = -1631076,
    SAY_SLAY_2                  = -1631077,
    SAY_BERSERK                 = -1631078,
    SAY_DEATH                   = -1631079,
    SAY_ROTFACE_DEATH           = -1631080,
};

struct MANGOS_DLL_DECL boss_rotfaceAI : public ScriptedAI
{
    boss_rotfaceAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ROTFACE, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ROTFACE, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(urand(0,1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature, pVictim);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ROTFACE, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rotface(Creature* pCreature)
{
    return new boss_rotfaceAI(pCreature);
}

void AddSC_boss_rotface()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_rotface";
    newscript->GetAI = &GetAI_boss_rotface;
    newscript->RegisterSelf();
}
