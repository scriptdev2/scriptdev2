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
SDName: Boss_Zuljin
SD%Complete: 0
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_INTRO                       = -1568056,
    SAY_AGGRO                       = -1568057,
    SAY_BEAR_TRANSFORM              = -1568058,
    SAY_EAGLE_TRANSFORM             = -1568059,
    SAY_LYNX_TRANSFORM              = -1568060,
    SAY_DRAGONHAWK_TRANSFORM        = -1568061,
    SAY_FIRE_BREATH                 = -1568062,
    SAY_BERSERK                     = -1568053,
    SAY_KILL1                       = -1568064,
    SAY_KILL2                       = -1568065,
    SAY_DEATH                       = -1568066,

    // Troll Form
    SPELL_WHIRLWIND                 = 17207,
    SPELL_GRIEVOUS_THROW            = 43093,                //removes debuff after full healed

    // Bear Form
    SPELL_CREEPING_PARALYSIS        = 43095,                //should cast on the whole raid
    SPELL_OVERPOWER                 = 43456,                //use after melee attack dodged

    // Eagle Form
    SPELL_ENERGY_STORM              = 43983,                //enemy area aura, trigger 42577
    SPELL_ZAP_INFORM                = 42577,
    SPELL_ZAP_DAMAGE                = 43137,                //1250 damage
    SPELL_SUMMON_CYCLONE            = 43112,                //summon four feather vortex
    CREATURE_FEATHER_VORTEX         = 24136,
    SPELL_CYCLONE_VISUAL            = 43119,                //trigger 43147 visual
    SPELL_CYCLONE_PASSIVE           = 43120,                //trigger 43121 (4y aoe) every second

    // Lynx Form
    SPELL_CLAW_RAGE_HASTE           = 42583,
    SPELL_CLAW_RAGE_TRIGGER         = 43149,
    SPELL_CLAW_RAGE_DAMAGE          = 43150,
    SPELL_LYNX_RUSH_HASTE           = 43152,
    SPELL_LYNX_RUSH_DAMAGE          = 43153,

    // Dragonhawk Form
    SPELL_FLAME_WHIRL               = 43213,                //trigger two spells
    SPELL_FLAME_BREATH              = 43215,
    SPELL_SUMMON_PILLAR             = 43216,                //summon 24187
    CREATURE_COLUMN_OF_FIRE         = 24187,
    SPELL_PILLAR_TRIGGER            = 43218,                //trigger 43217

    // Cosmetic
    SPELL_SPIRIT_AURA               = 42466,
    SPELL_SIPHON_SOUL               = 43501,

    // Transforms
    SPELL_SHAPE_OF_THE_BEAR         = 42594,
    SPELL_SHAPE_OF_THE_EAGLE        = 42606,
    SPELL_SHAPE_OF_THE_LYNX         = 42607,
    SPELL_SHAPE_OF_THE_DRAGONHAWK   = 42608,

    SPELL_BERSERK                   = 45078,

    WEAPON_ID                       = 33975,

    PHASE_BEAR                      = 0,
    PHASE_EAGLE                     = 1,
    PHASE_LYNX                      = 2,
    PHASE_DRAGONHAWK                = 3,
    PHASE_TROLL                     = 4
};

//coords for going for changing form
const float CENTER_X = 120.148811f;
const float CENTER_Y = 703.713684f;
const float CENTER_Z = 45.111477f;

struct MANGOS_DLL_DECL boss_zuljinAI : public ScriptedAI
{
    boss_zuljinAI(Creature* pCreature) : ScriptedAI(pCreature)
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
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_ZULJIN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zuljin(Creature* pCreature)
{
    return new boss_zuljinAI(pCreature);
}

void AddSC_boss_zuljin()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_zuljin";
    newscript->GetAI = &GetAI_boss_zuljin;
    newscript->RegisterSelf();
}
