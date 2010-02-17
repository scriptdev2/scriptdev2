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
SDName: Spell_Scripts
SD%Complete: 100
SDComment: Spell scripts for dummy effects (if script need access/interaction with parts of other AI or instance, add it in related source files)
SDCategory: Spell
EndScriptData */

/* ContentData
npc_nestlewood_owlkin
npc_liquid_fire_of_elune
npc_robot_reprogrammed
npc_woodlands_walker
EndContentData */

#include "precompiled.h"
#include "../system/system.h"

/* When you make a spell effect:
- always check spell id and effect index
- always return true when the spell is handled by script
*/

/*######
## npc_nestlewood_owlkin
######*/

enum
{
    SPELL_INOCULATE_OWLKIN  = 29528,
    ENTRY_OWLKIN            = 16518,
    ENTRY_OWLKIN_INOC       = 16534,
};

bool EffectDummyCreature_npc_nestlewood_owlkin(Unit* pCaster, uint32 uiSpellId, uint32 uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_INOCULATE_OWLKIN && uiEffIndex == SPELL_EFFECT_0)
    {
        if (pCreatureTarget->GetEntry() != ENTRY_OWLKIN)
            return true;

        pCreatureTarget->UpdateEntry(ENTRY_OWLKIN_INOC);

        //set despawn timer, since we want to remove creature after a short time
        pCreatureTarget->ForcedDespawn(15000);

        return true;
    }
    return false;
}

/*######
## npc_liquid_fire_of_elune (quest 12166)
######*/

enum
{
    NPC_ELK                 = 26616,
    NPC_ELK_BUNNY           = 27111,
    NPC_GRIZZLY             = 26643,
    NPC_GRIZZLY_BUNNY       = 27112,

    SPELL_LIQUID_FIRE       = 46770,
    SPELL_LIQUID_FIRE_AURA  = 47972
};

bool EffectDummyCreature_npc_liquid_fire_of_elune(Unit* pCaster, uint32 uiSpellId, uint32 uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_LIQUID_FIRE && uiEffIndex == SPELL_EFFECT_0)
    {
        if (pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            if (pCreatureTarget->HasAura(SPELL_LIQUID_FIRE_AURA))
                return true;

            if (pCreatureTarget->GetEntry() == NPC_ELK)
            {
                pCreatureTarget->CastSpell(pCreatureTarget, SPELL_LIQUID_FIRE_AURA, true);
                ((Player*)pCaster)->KilledMonsterCredit(NPC_ELK_BUNNY, 0);
            }
            else if (pCreatureTarget->GetEntry() == NPC_GRIZZLY)
            {
                pCreatureTarget->CastSpell(pCreatureTarget, SPELL_LIQUID_FIRE_AURA, true);
                ((Player*)pCaster)->KilledMonsterCredit(NPC_GRIZZLY_BUNNY, 0);
            }
        }

        return true;
    }
    return false;
}

/*######
## npc_robot_reprogrammed
######*/

enum
{
    SPELL_ULTRASONIC_SCREWDRIVER    = 46023,
    SPELL_REPROGRAM_KILL_CREDIT     = 46027,

    NPC_COLLECT_A_TRON              = 25793,
    SPELL_SUMMON_COLLECT_A_TRON     = 46034,

    NPC_DEFENDO_TANK                = 25758,
    SPELL_SUMMON_DEFENDO_TANK       = 46058,

    NPC_SCAVENGE_A8                 = 25752,
    SPELL_SUMMON_SCAVENGE_A8        = 46063,

    NPC_SCAVENGE_B6                 = 25792,
    SPELL_SUMMON_SCAVENGE_B6        = 46066,

    NPC_SENTRY_BOT                  = 25753,
    SPELL_SUMMON_SENTRY_BOT         = 46068
};

bool EffectDummyCreature_npc_robot_reprogrammed(Unit* pCaster, uint32 uiSpellId, uint32 uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_ULTRASONIC_SCREWDRIVER && uiEffIndex == SPELL_EFFECT_0)
    {
        if (pCreatureTarget->isDead())
        {
            uiSpellId = 0;

            switch(pCreatureTarget->GetEntry())
            {
                case NPC_COLLECT_A_TRON:    uiSpellId = SPELL_SUMMON_COLLECT_A_TRON; break;
                case NPC_DEFENDO_TANK:      uiSpellId = SPELL_SUMMON_DEFENDO_TANK; break;
                case NPC_SCAVENGE_A8:       uiSpellId = SPELL_SUMMON_SCAVENGE_A8; break;
                case NPC_SCAVENGE_B6:       uiSpellId = SPELL_SUMMON_SCAVENGE_B6; break;
                case NPC_SENTRY_BOT:        uiSpellId = SPELL_SUMMON_SENTRY_BOT; break;
            }

            if (const SpellEntry* pSpell = GetSpellStore()->LookupEntry(uiSpellId))
            {
                pCaster->CastSpell(pCreatureTarget, pSpell->Id, true);

                if (Pet* pPet = pCaster->FindGuardianWithEntry(pSpell->EffectMiscValue[uiEffIndex]))
                    pPet->CastSpell(pCaster, SPELL_REPROGRAM_KILL_CREDIT, true);

                pCreatureTarget->ForcedDespawn();
            }
        }

        return true;
    }
    return false;
}

/*######
## npc_woodlands_walker
######*/

enum
{
    SPELL_STRENGTH_ANCIENTS     = 47575,
    SPELL_CREATE_BARK_WALKERS   = 47550,
    FACTION_HOSTILE             = 16,

    EMOTE_AGGRO                 = -1000551,
    EMOTE_CREATE                = -1000552
};

bool EffectDummyCreature_npc_woodlands_walker(Unit* pCaster, uint32 uiSpellId, uint32 uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_STRENGTH_ANCIENTS && uiEffIndex == SPELL_EFFECT_0)
    {
        if (pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            if (urand(0, 1))
            {
                DoScriptText(EMOTE_AGGRO, pCreatureTarget);
                pCreatureTarget->setFaction(FACTION_HOSTILE);
                pCreatureTarget->AI()->AttackStart(pCaster);
            }
            else
            {
                DoScriptText(EMOTE_CREATE, pCreatureTarget);
                pCaster->CastSpell(pCaster, SPELL_CREATE_BARK_WALKERS, true);
                pCreatureTarget->ForcedDespawn(5000);
            }
        }

        return true;
    }
    return false;
}

void AddSC_spell_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_nestlewood_owlkin";
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_nestlewood_owlkin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_liquid_fire_of_elune";
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_liquid_fire_of_elune;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_robot_reprogrammed";
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_robot_reprogrammed;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_woodlands_walker";
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_woodlands_walker;
    newscript->RegisterSelf();
}
