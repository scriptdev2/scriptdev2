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
SDName: boss_shirrak
SD%Complete: 20
SDComment: Basic script only
SDCategory: Auchindoun, Auchenai Crypts
EndScriptData */

#include "precompiled.h"

enum
{
    EMOTE_FOCUS                     = -1558010,

    SPELL_CARNIVOROUS_BITE          = 36383,
    SPELL_CARNIVOROUS_BITE_H        = 39382,
    SPELL_INHIBIT_MAGIC             = 32264,
    SPELL_ATTRACT_MAGIC             = 32265,

    SPELL_FOCUS_TARGET_VISUAL       = 32286,
    NPC_FOCUS_FIRE                  = 18374
};

struct MANGOS_DLL_DECL boss_shirrakAI : public ScriptedAI
{
    boss_shirrakAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

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

CreatureAI* GetAI_boss_shirrak(Creature* pCreature)
{
    return new boss_shirrakAI(pCreature);
}

void AddSC_boss_shirrak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_shirrak";
    pNewScript->GetAI = &GetAI_boss_shirrak;
    pNewScript->RegisterSelf();
}
