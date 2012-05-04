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
SDName: boss_deathbringer_saurfang
SD%Complete: 20%
SDComment: Basic script
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_AGGRO                   = -1631028,
    SAY_FALLENCHAMPION          = -1631029,
    SAY_BLOODBEASTS             = -1631030,
    SAY_SLAY_1                  = -1631031,
    SAY_SLAY_2                  = -1631032,
    SAY_BERSERK                 = -1631033,
    SAY_DEATH                   = -1631034,
    SAY_INTRO_ALLY_0            = -1631035,
    SAY_INTRO_ALLY_1            = -1631036,
    SAY_INTRO_ALLY_2            = -1631037,
    SAY_INTRO_ALLY_3            = -1631038,
    SAY_INTRO_ALLY_4            = -1631039,
    SAY_INTRO_ALLY_5            = -1631040,
    SAY_INTRO_HORDE_1           = -1631041,
    SAY_INTRO_HORDE_2           = -1631042,
    SAY_INTRO_HORDE_3           = -1631043,
    SAY_INTRO_HORDE_4           = -1631044,
    SAY_INTRO_HORDE_5           = -1631045,
    SAY_INTRO_HORDE_6           = -1631046,
    SAY_INTRO_HORDE_7           = -1631047,
    SAY_INTRO_HORDE_8           = -1631048,
    SAY_INTRO_HORDE_9           = -1631049,
    SAY_OUTRO_ALLY_1            = -1631050,
    SAY_OUTRO_ALLY_2            = -1631051,
    SAY_OUTRO_ALLY_3            = -1631052,
    SAY_OUTRO_ALLY_4            = -1631053,
    SAY_OUTRO_ALLY_5            = -1631054,
    SAY_OUTRO_ALLY_6            = -1631055,
    SAY_OUTRO_ALLY_7            = -1631056,
    SAY_OUTRO_ALLY_8            = -1631057,
    SAY_OUTRO_ALLY_9            = -1631058,
    SAY_OUTRO_ALLY_10           = -1631059,
    SAY_OUTRO_ALLY_11           = -1631060,
    SAY_OUTRO_ALLY_12           = -1631061,
    SAY_OUTRO_ALLY_13           = -1631062,
    SAY_OUTRO_ALLY_14           = -1631063,
    SAY_OUTRO_ALLY_15           = -1631064,
    SAY_OUTRO_ALLY_16           = -1631065,
    SAY_OUTRO_HORDE_1           = -1631066,
    SAY_OUTRO_HORDE_2           = -1631067,
    SAY_OUTRO_HORDE_3           = -1631068,
    SAY_OUTRO_HORDE_4           = -1631069,
};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public ScriptedAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, IN_PROGRESS);
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
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_deathbringer_saurfang(Creature* pCreature)
{
    return new boss_deathbringer_saurfangAI(pCreature);
}

void AddSC_boss_deathbringer_saurfang()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_deathbringer_saurfang";
    pNewScript->GetAI = &GetAI_boss_deathbringer_saurfang;
    pNewScript->RegisterSelf();
}
