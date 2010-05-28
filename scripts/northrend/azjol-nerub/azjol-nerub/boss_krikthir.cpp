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
SDName: Boss_Krikthir
SD%Complete: 20%
SDComment:
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SAY_AGGRO                       = -1601000,
    SAY_KILL_1                      = -1601001,
    SAY_KILL_2                      = -1601002,
    SAY_KILL_3                      = -1601003,
    SAY_SEND_GROUP_1                = -1601004,
    SAY_SEND_GROUP_2                = -1601005,
    SAY_SEND_GROUP_3                = -1601006,
    SAY_PREFIGHT_1                  = -1601007,
    SAY_PREFIGHT_2                  = -1601008,
    SAY_PREFIGHT_3                  = -1601009,
    SAY_SWARM_1                     = -1601010,
    SAY_SWARM_2                     = -1601011,
    SAY_DEATH                       = -1601012,
    EMOTE_BOSS_GENERIC_FRENZY       = -1000005
};

/*######
## boss_krikthir
######*/

struct MANGOS_DLL_DECL boss_krikthirAI : public ScriptedAI
{
    boss_krikthirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRIKTHIR, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krikthir(Creature* pCreature)
{
    return new boss_krikthirAI(pCreature);
}

void AddSC_boss_krikthir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_krikthir";
    newscript->GetAI = &GetAI_boss_krikthir;
    newscript->RegisterSelf();
}
