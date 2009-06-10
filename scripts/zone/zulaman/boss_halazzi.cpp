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
SDName: Boss_Halazzi
SD%Complete: 0
SDComment: Place Holder
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"

enum
{
    SAY_AGGRO               = -1568034,
    SAY_SPLIT               = -1568035,
    SAY_MERGE               = -1568036,
    SAY_SABERLASH1          = -1568037,
    SAY_SABERLASH2          = -1568038,
    SAY_BERSERK             = -1568039,
    SAY_KILL1               = -1568040,
    SAY_KILL2               = -1568041,
    SAY_DEATH               = -1568042,
    SAY_EVENT1              = -1568043,
    SAY_EVENT2              = -1568044,

    SPELL_DUAL_WIELD        = 29651,
    SPELL_SABER_LASH        = 43267,
    SPELL_FRENZY            = 43139,
    SPELL_FLAMESHOCK        = 43303,
    SPELL_EARTHSHOCK        = 43305,
    SPELL_TRANSFORM_SPLIT   = 43142,
    SPELL_TRANSFORM_SPLIT2  = 43573,
    SPELL_TRANSFORM_MERGE   = 43271,
    SPELL_SUMMON_LYNX       = 43143,
    SPELL_SUMMON_TOTEM      = 43302,
    SPELL_BERSERK           = 45078,

    MOB_SPIRIT_LYNX         = 24143,
    SPELL_LYNX_FRENZY       = 43290,
    SPELL_SHRED_ARMOR       = 43243,

    MOB_TOTEM               = 24224,
    SPELL_LIGHTNING         = 43301
};

struct MANGOS_DLL_DECL boss_halazziAI : public ScriptedAI
{
    boss_halazziAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_HALAZZI, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halazzi(Creature* pCreature)
{
    return new boss_halazziAI(pCreature);
}

void AddSC_boss_halazzi()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_halazzi";
    newscript->GetAI = &GetAI_boss_halazzi;
    newscript->RegisterSelf();
}
