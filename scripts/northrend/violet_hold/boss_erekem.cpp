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
SDName: boss_erekem
SD%Complete: 0
SDComment: Placeholder
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SAY_AGGRO                   = -1608012,
    SAY_ADD_DIE_1               = -1608013,
    SAY_ADD_DIE_2               = -1608014,
    SAY_DEATH                   = -1608018,
    /* A few Sound IDs on SLAY, if there _is_ text related, fields -1608015 to -1608017 are free
    ** 14222
    ** 14223
    ** 14224
    */

    SPELL_BLOODLUST             = 54516,
    SPELL_BREAK_BONDS_H         = 59463,
    SPELL_CHAIN_HEAL            = 54481,
    SPELL_CHAIN_HEAL_H          = 59473,
    SPELL_EARTH_SHIELD          = 54479,
    SPELL_EARTH_SHIELD_H        = 59471,
    SPELL_EARTH_SHOCK           = 54511,
    SPELL_LIGHTNING_BOLT        = 53044,
    SPELL_STORMSTRIKE           = 51876,

    // Spells of adds
    SPELL_GUSHING_WOUND         = 39215,
    SPELL_HOWLING_SCREECH       = 54463,
    SPELL_STRIKE                = 14516
};

struct MANGOS_DLL_DECL boss_erekemAI : public ScriptedAI
{
    boss_erekemAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        Reset();
    }

    instance_violet_hold* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
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

CreatureAI* GetAI_boss_erekem(Creature* pCreature)
{
    return new boss_erekemAI(pCreature);
}

void AddSC_boss_erekem()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_erekem";
    pNewScript->GetAI = &GetAI_boss_erekem;
    pNewScript->RegisterSelf();
}
