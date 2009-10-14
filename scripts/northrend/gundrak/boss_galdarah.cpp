/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Galdarah
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                       = -1604019,
    SAY_TRANSFORM_1                 = -1604020,
    SAY_TRANSFORM_2                 = -1604021,
    SAY_SUMMON_1                    = -1604022,
    SAY_SUMMON_2                    = -1604023,
    SAY_SUMMON_3                    = -1604024,
    SAY_SLAY_1                      = -1604025,
    SAY_SLAY_2                      = -1604026,
    SAY_SLAY_3                      = -1604027,
    SAY_DEATH                       = -1604028
};

/*######
## boss_galdarah
######*/

struct MANGOS_DLL_DECL boss_galdarahAI : public ScriptedAI
{
    boss_galdarahAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

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
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_galdarah(Creature* pCreature)
{
    return new boss_galdarahAI(pCreature);
}

void AddSC_boss_galdarah()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_galdarah";
    newscript->GetAI = &GetAI_boss_galdarah;
    newscript->RegisterSelf();
}
