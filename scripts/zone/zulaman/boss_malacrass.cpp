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
SDName: Boss_Malacrass
SD%Complete: 0
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"

enum
{
    SAY_AGGRO               = -1568045,
    SAY_ENRAGE              = -1568046,
    SAY_KILL1               = -1568047,
    SAY_KILL2               = -1568048,
    SAY_SOUL_SIPHON         = -1568049,
    SAY_DRAIN_POWER         = -1568050,
    SAY_SPIRIT_BOLTS        = -1568051,
    SAY_ADD_DIED1           = -1568052,
    SAY_ADD_DIED2           = -1568053,
    SAY_ADD_DIED3           = -1568054,
    SAY_DEATH               = -1568055,

    SPELL_SPIRIT_BOLTS      = 43383,
    SPELL_SIPHON_SOUL       = 43501,
    SPELL_DRAIN_POWER       = 44131,

    WEAPON_ID               = 33494,

    MOB_TEMP_TRIGGER        = 23920,

    //for various powers he uses after using soul drain
    //Druid
    SPELL_DR_THORNS         = 43420,
    SPELL_DR_LIFEBLOOM      = 43421,
    SPELL_DR_MOONFIRE       = 43545,

    //Hunter
    SPELL_HU_EXPLOSIVE_TRAP = 43444,
    SPELL_HU_FREEZING_TRAP  = 43447,
    SPELL_HU_SNAKE_TRAP     = 43449,

    //Mage
    SPELL_MG_FIREBALL       = 41383,
    SPELL_MG_FROST_NOVA     = 43426,
    SPELL_MG_ICE_LANCE      = 43427,
    SPELL_MG_FROSTBOLT      = 43428,

    //Paladin
    SPELL_PA_CONSECRATION   = 43429,
    SPELL_PA_AVENGING_WRATH = 43430,
    SPELL_PA_HOLY_LIGHT     = 43451,

    //Priest
    SPELL_PR_HEAL           = 41372,
    SPELL_PR_MIND_BLAST     = 41374,
    SPELL_PR_SW_DEATH       = 41375,
    SPELL_PR_PSYCHIC_SCREAM = 43432,
    SPELL_PR_MIND_CONTROL   = 43550,
    SPELL_PR_PAIN_SUPP      = 44416,

    //Rogue
    SPELL_RO_WOUND_POISON   = 39665,
    SPELL_RO_BLIND          = 43433,
    SPELL_RO_SLICE_DICE     = 43457,

    //Shaman
    SPELL_SH_CHAIN_LIGHT    = 43435,
    SPELL_SH_FIRE_NOVA      = 43436,
    SPELL_SH_HEALING_WAVE   = 43548,

    //Warlock
    SPELL_WL_CURSE_OF_DOOM  = 43439,
    SPELL_WL_RAIN_OF_FIRE   = 43440,
    SPELL_WL_UNSTABLE_AFFL  = 35183,

    //Warrior
    SPELL_WR_MORTAL_STRIKE  = 43441,
    SPELL_WR_WHIRLWIND      = 43442,
    SPELL_WR_SPELL_REFLECT  = 43443
};

//Adds positions
static float fAddPosX[4] = {128.279, 123.261, 112.084, 106.473};
const float ADD_POS_Y       = 921.279;
const float ADD_POS_Z       = 33.889;
const float ADD_ORIENT      = 1.527;

static uint32 AddEntryList[8]=
{
    24240,                                                  //Alyson Antille
    24241,                                                  //Thurg
    24242,                                                  //Slither
    24243,                                                  //Lord Raadan
    24244,                                                  //Gazakroth
    24245,                                                  //Fenstalker
    24246,                                                  //Darkheart
    24247,                                                  //Koragg
};

struct MANGOS_DLL_DECL boss_malacrassAI : public ScriptedAI
{
    boss_malacrassAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

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

        if (!pInstance)
            return;

        pInstance->SetData(TYPE_MALACRASS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_malacrass(Creature* pCreature)
{
    return new boss_malacrassAI(pCreature);
}

void AddSC_boss_malacrass()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_malacrass";
    newscript->GetAI = &GetAI_boss_malacrass;
    newscript->RegisterSelf();
}
