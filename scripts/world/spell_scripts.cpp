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
spell 34665
spell 19512
spell 8913
spell 21014
spell 29528
spell 29866
spell 46770
spell 46023
spell 47575
spell 50706
spell 45109
spell 45111
EndContentData */

#include "precompiled.h"

/* When you make a spell effect:
- always check spell id and effect index
- always return true when the spell is handled by script
*/

enum
{
    // quest 9452
    SPELL_CAST_FISHING_NET      = 29866,
    GO_RED_SNAPPER              = 181616,
    NPC_ANGRY_MURLOC            = 17102,
    ITEM_RED_SNAPPER            = 23614,
    //SPELL_SUMMON_TEST           = 49214                   // ! Just wrong spell name? It summon correct creature (17102)but does not appear to be used.

    // quest 11472
    SPELL_ANUNIAQS_NET          = 21014,
    GO_TASTY_REEF_FISH          = 186949,
    NPC_REEF_SHARK              = 24637,
    ITEM_TASTY_REEF_FISH        = 34127,
};

bool EffectDummyGameObj_spell_dummy_go(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, GameObject* pGOTarget)
{
    switch(uiSpellId)
    {
        case SPELL_ANUNIAQS_NET:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pGOTarget->GetEntry() != GO_TASTY_REEF_FISH || pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (urand(0, 3))
                {
                    if (Item* pItem = ((Player*)pCaster)->StoreNewItemInInventorySlot(ITEM_TASTY_REEF_FISH, 1))
                        ((Player*)pCaster)->SendNewItem(pItem, 1, true, false);
                }
                else
                {
                    if (Creature* pShark = pCaster->SummonCreature(NPC_REEF_SHARK, pGOTarget->GetPositionX(), pGOTarget->GetPositionY(), pGOTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        pShark->AI()->AttackStart(pCaster);
                }

                pGOTarget->Delete();                        // sends despawn anim + destroy
                return true;
            }
            return true;
        }
        case SPELL_CAST_FISHING_NET:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pGOTarget->GetEntry() != GO_RED_SNAPPER || pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (urand(0, 2))
                {
                    if (Creature* pMurloc = pCaster->SummonCreature(NPC_ANGRY_MURLOC, pCaster->GetPositionX(), pCaster->GetPositionY()+20.0f, pCaster->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                        pMurloc->AI()->AttackStart(pCaster);
                }
                else
                {
                    if (Item* pItem = ((Player*)pCaster)->StoreNewItemInInventorySlot(ITEM_RED_SNAPPER, 1))
                        ((Player*)pCaster)->SendNewItem(pItem, 1, true, false);
                }

                pGOTarget->Delete();                        // sends despawn anim + destroy
                return true;
            }
            return true;
        }
    }

    return false;
}

enum
{
    // quest 9629
    SPELL_TAG_MURLOC                    = 30877,
    SPELL_TAG_MURLOC_PROC               = 30875,
    NPC_BLACKSILT_MURLOC                = 17326,
    NPC_TAGGED_MURLOC                   = 17654,

    // quest 9447
    SPELL_HEALING_SALVE                 = 29314,
    SPELL_HEALING_SALVE_DUMMY           = 29319,
    NPC_MAGHAR_GRUNT                    = 16846,

    // quest 10190
    SPELL_RECHARGING_BATTERY            = 34219,
    NPC_DRAINED_PHASE_HUNTER            = 19595,

    // target hulking helboar
    SPELL_ADMINISTER_ANTIDOTE           = 34665,
    NPC_HELBOAR                         = 16880,
    NPC_DREADTUSK                       = 16992,

    // quest 6124/6129
    SPELL_APPLY_SALVE                   = 19512,

    NPC_SICKLY_DEER                     = 12298,
    NPC_SICKLY_GAZELLE                  = 12296,

    NPC_CURED_DEER                      = 12299,
    NPC_CURED_GAZELLE                   = 12297,

    // quest 12906/13422
    SPELL_DISCIPLINING_ROD              = 56033,
    SAY_RAND_WORK1                      = -1000555,
    SAY_RAND_WORK2                      = -1000556,
    SAY_RAND_WORK3                      = -1000557,
    SAY_RAND_ATTACK1                    = -1000558,
    SAY_RAND_ATTACK2                    = -1000559,
    SAY_RAND_ATTACK3                    = -1000560,

    // target morbent fel
    SPELL_SACRED_CLEANSING              = 8913,
    NPC_MORBENT                         = 1200,
    NPC_WEAKENED_MORBENT                = 24782,

    // quest 11515
    SPELL_FEL_SIPHON_DUMMY              = 44936,
    NPC_FELBLOOD_INITIATE               = 24918,
    NPC_EMACIATED_FELBLOOD              = 24955,

    // target nestlewood owlkin
    SPELL_INOCULATE_OWLKIN              = 29528,
    NPC_OWLKIN                          = 16518,
    NPC_OWLKIN_INOC                     = 16534,

    // target for quest 12166)
    SPELL_LIQUID_FIRE                   = 46770,
    SPELL_LIQUID_FIRE_AURA              = 47972,

    NPC_ELK                             = 26616,
    NPC_GRIZZLY                         = 26643,

    NPC_ELK_BUNNY                       = 27111,
    NPC_GRIZZLY_BUNNY                   = 27112,

    // for quest 12516
    SPELL_MODIFIED_MOJO                 = 50706,

    NPC_PROPHET_OF_SSERATUS             = 28068,
    NPC_WEAK_PROPHET_OF_SSERATUS        = 28151,

    // for quest 12459
    SPELL_SEEDS_OF_NATURES_WRATH        = 49587,

    NPC_REANIMATED_FROSTWYRM            = 26841,
    NPC_TURGID                          = 27808,
    NPC_DEATHGAZE                       = 27122,

    NPC_WEAK_REANIMATED_FROSTWYRM       = 27821,
    NPC_WEAK_TURGID                     = 27809,
    NPC_WEAK_DEATHGAZE                  = 27807,

    // quest 11982
    SPELL_THROW_BOULDER                 = 47005,
    SPELL_BOULBER_IMPACT                = 47007,
    SPELL_BOULDER_TOSS_CREDIT           = 47009,

    NPC_IRON_RUNESHAPER                 = 26270,
    NPC_RUNE_REAVER                     = 26268,

    // for quest 11730
    SPELL_ULTRASONIC_SCREWDRIVER        = 46023,
    SPELL_REPROGRAM_KILL_CREDIT         = 46027,

    NPC_COLLECT_A_TRON                  = 25793,
    SPELL_SUMMON_COLLECT_A_TRON         = 46034,

    NPC_DEFENDO_TANK                    = 25758,
    SPELL_SUMMON_DEFENDO_TANK           = 46058,

    NPC_SCAVENGE_A8                     = 25752,
    SPELL_SUMMON_SCAVENGE_A8            = 46063,

    NPC_SCAVENGE_B6                     = 25792,
    SPELL_SUMMON_SCAVENGE_B6            = 46066,

    NPC_SENTRY_BOT                      = 25753,
    SPELL_SUMMON_SENTRY_BOT             = 46068,

    // target woodlands walker
    SPELL_STRENGTH_ANCIENTS             = 47575,
    SPELL_CREATE_BARK_WALKERS           = 47550,
    FACTION_HOSTILE                     = 16,

    EMOTE_AGGRO                         = -1000551,
    EMOTE_CREATE                        = -1000552,

    SAY_SPECIMEN                        = -1000581,
    NPC_NEXUS_DRAKE_HATCHLING           = 26127,
    SPELL_RAELORASZ_FIREBALL            = 46704,

    // Quest "Disrupt the Greengill Coast" (11541)
    SPELL_ORB_OF_MURLOC_CONTROL         = 45109,
    SPELL_GREENGILL_SLAVE_FREED         = 45110,
    SPELL_ENRAGE                        = 45111,
    NPC_FREED_GREENGILL_SLAVE           = 25085,
    NPC_DARKSPINE_MYRMIDON              = 25060,
    NPC_DARKSPINE_SIREN                 = 25073
};

bool EffectAuraDummy_spell_aura_dummy_npc(const Aura* pAura, bool bApply)
{
    switch(pAura->GetId())
    {
        case SPELL_HEALING_SALVE:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (bApply)
            {
                if (Unit* pCaster = pAura->GetCaster())
                    pCaster->CastSpell(pAura->GetTarget(), SPELL_HEALING_SALVE_DUMMY, true);
            }

            return true;
        }
        case SPELL_HEALING_SALVE_DUMMY:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (!bApply)
            {
                Creature* pCreature = (Creature*)pAura->GetTarget();

                pCreature->UpdateEntry(NPC_MAGHAR_GRUNT);

                if (pCreature->getStandState() == UNIT_STAND_STATE_KNEEL)
                    pCreature->SetStandState(UNIT_STAND_STATE_STAND);

                pCreature->ForcedDespawn(60*IN_MILLISECONDS);
            }

            return true;
        }
        case SPELL_RECHARGING_BATTERY:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (!bApply)
            {
                if (pAura->GetTarget()->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
                    ((Creature*)pAura->GetTarget())->UpdateEntry(NPC_DRAINED_PHASE_HUNTER);
            }

            return true;
        }
        case SPELL_TAG_MURLOC:
        {
            Creature* pCreature = (Creature*)pAura->GetTarget();

            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;

            if (bApply)
            {
                if (pCreature->GetEntry() == NPC_BLACKSILT_MURLOC)
                {
                    if (Unit* pCaster = pAura->GetCaster())
                        pCaster->CastSpell(pCreature, SPELL_TAG_MURLOC_PROC, true);
                }
            }
            else
            {
                if (pCreature->GetEntry() == NPC_TAGGED_MURLOC)
                    pCreature->ForcedDespawn();
            }

            return true;
        }
        case SPELL_RAELORASZ_FIREBALL:
        {
            if (pAura->GetEffIndex() != EFFECT_INDEX_0)
                return true;
            
            if (Unit* pCaster = pAura->GetCaster())
                DoScriptText(SAY_SPECIMEN, pCaster);

            Unit* pTarget = pAura->GetTarget();
            if (pTarget->GetTypeId() == TYPEID_UNIT)
            {
                Creature* pCreature = (Creature*)pTarget;

                if (pCreature->GetEntry() == NPC_NEXUS_DRAKE_HATCHLING)
                {
                    pCreature->SetStandState(UNIT_STAND_STATE_SLEEP);
                    pCreature->ForcedDespawn(3000);
                }
            }
            return true;
        }
        case SPELL_ENRAGE:
        {
            if (pAura->GetTarget()->GetTypeId() != TYPEID_UNIT || !bApply)
                return false;

            if (Creature* pCreature = GetClosestCreatureWithEntry(pAura->GetTarget(), NPC_DARKSPINE_MYRMIDON, 25.0f))
            {
                dynamic_cast<Creature*>(pAura->GetTarget())->AI()->AttackStart(pCreature);
                return true;
            }

            if (Creature* pCreature = GetClosestCreatureWithEntry(pAura->GetTarget(), NPC_DARKSPINE_SIREN, 25.0f))
            {
                dynamic_cast<Creature*>(pAura->GetTarget())->AI()->AttackStart(pCreature);
                return true;
            }

            return false;
        }
    }

    return false;
}

bool EffectDummyCreature_spell_dummy_npc(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    switch(uiSpellId)
    {
        case SPELL_ADMINISTER_ANTIDOTE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_HELBOAR)
                    return true;

                // possible needs check for quest state, to not have any effect when quest really complete

                pCreatureTarget->UpdateEntry(NPC_DREADTUSK);
                return true;
            }
            return true;
        }
        case SPELL_APPLY_SALVE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (pCreatureTarget->GetEntry() == NPC_SICKLY_DEER && ((Player*)pCaster)->GetTeam() == ALLIANCE)
                    pCreatureTarget->UpdateEntry(NPC_CURED_DEER);

                if (pCreatureTarget->GetEntry() == NPC_SICKLY_GAZELLE && ((Player*)pCaster)->GetTeam() == HORDE)
                    pCreatureTarget->UpdateEntry(NPC_CURED_GAZELLE);

                return true;
            }
            return true;
        }
        case SPELL_DISCIPLINING_ROD:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->getStandState() == UNIT_STAND_STATE_STAND)
                    return true;

                switch(urand(1,2))
                {
                    case 1:
                    {
                        switch(urand(1,3))
                        {
                            case 1: DoScriptText(SAY_RAND_ATTACK1, pCreatureTarget); break;
                            case 2: DoScriptText(SAY_RAND_ATTACK2, pCreatureTarget); break;
                            case 3: DoScriptText(SAY_RAND_ATTACK3, pCreatureTarget); break;
                        }

                        pCreatureTarget->SetStandState(UNIT_STAND_STATE_STAND);
                        pCreatureTarget->AI()->AttackStart(pCaster);
                        break;
                    }
                    case 2:
                    {
                        switch(urand(1,3))
                        {
                            case 1: DoScriptText(SAY_RAND_WORK1, pCreatureTarget); break;
                            case 2: DoScriptText(SAY_RAND_WORK2, pCreatureTarget); break;
                            case 3: DoScriptText(SAY_RAND_WORK3, pCreatureTarget); break;
                        }

                        pCreatureTarget->SetStandState(UNIT_STAND_STATE_STAND);
                        pCreatureTarget->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_WORK);
                        break;
                    }
                }

                return true;
            }
            return true;
        }
        case SPELL_INOCULATE_OWLKIN:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_OWLKIN)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_OWLKIN_INOC);

                //set despawn timer, since we want to remove creature after a short time
                pCreatureTarget->ForcedDespawn(15000);

                return true;
            }
            return true;
        }
        case SPELL_LIQUID_FIRE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() == TYPEID_PLAYER)
                {
                    if (pCreatureTarget->HasAura(SPELL_LIQUID_FIRE_AURA))
                        return true;

                    if (pCreatureTarget->GetEntry() == NPC_ELK)
                    {
                        pCreatureTarget->CastSpell(pCreatureTarget, SPELL_LIQUID_FIRE_AURA, true);
                        ((Player*)pCaster)->KilledMonsterCredit(NPC_ELK_BUNNY);
                    }
                    else if (pCreatureTarget->GetEntry() == NPC_GRIZZLY)
                    {
                        pCreatureTarget->CastSpell(pCreatureTarget, SPELL_LIQUID_FIRE_AURA, true);
                        ((Player*)pCaster)->KilledMonsterCredit(NPC_GRIZZLY_BUNNY);
                    }
                }
                return true;
            }
            return true;
        }
        case SPELL_MODIFIED_MOJO:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_PROPHET_OF_SSERATUS)
                    return true;

                // Apparently done before updateEntry, so need to make a way to handle that
                // "Mmm, more mojo"
                // "%s drinks the Mojo"
                // "NOOOOOOOOOOOOooooooo...............!"

                pCreatureTarget->UpdateEntry(NPC_WEAK_PROPHET_OF_SSERATUS);
                return true;
            }
            return true;
        }
        case SPELL_FEL_SIPHON_DUMMY:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() != NPC_FELBLOOD_INITIATE)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_EMACIATED_FELBLOOD);
                return true;
            }
            return true;
        }
        case SPELL_SACRED_CLEANSING:
        {
            if (uiEffIndex == EFFECT_INDEX_1)
            {
                if (pCreatureTarget->GetEntry() != NPC_MORBENT)
                    return true;

                pCreatureTarget->UpdateEntry(NPC_WEAKENED_MORBENT);
                return true;
            }
            return true;
        }
        case SPELL_SEEDS_OF_NATURES_WRATH:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                uint32 uiNewEntry = 0;

                switch(pCreatureTarget->GetEntry())
                {
                    case NPC_REANIMATED_FROSTWYRM:  uiNewEntry = NPC_WEAK_REANIMATED_FROSTWYRM; break;
                    case NPC_TURGID:                uiNewEntry = NPC_WEAK_TURGID; break;
                    case NPC_DEATHGAZE:             uiNewEntry = NPC_WEAK_DEATHGAZE; break;
                }

                if (uiNewEntry)
                    pCreatureTarget->UpdateEntry(uiNewEntry);

                return true;
            }
            return true;
        }
        case SPELL_STRENGTH_ANCIENTS:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
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
            return true;
        }
        case SPELL_TAG_MURLOC_PROC:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->GetEntry() == NPC_BLACKSILT_MURLOC)
                    pCreatureTarget->UpdateEntry(NPC_TAGGED_MURLOC);
            }
            return true;
        }
        case SPELL_THROW_BOULDER:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (pCreatureTarget->GetEntry() != NPC_IRON_RUNESHAPER && pCreatureTarget->GetEntry() != NPC_RUNE_REAVER)
                    return true;

                pCreatureTarget->CastSpell(pCreatureTarget, SPELL_BOULBER_IMPACT, true);
                pCaster->CastSpell(pCaster, SPELL_BOULDER_TOSS_CREDIT, true);

                return true;
            }
            return true;
        }
        case SPELL_ULTRASONIC_SCREWDRIVER:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCreatureTarget->isDead())
                {
                    uint32 newSpellId = 0;

                    switch(pCreatureTarget->GetEntry())
                    {
                        case NPC_COLLECT_A_TRON:    newSpellId = SPELL_SUMMON_COLLECT_A_TRON; break;
                        case NPC_DEFENDO_TANK:      newSpellId = SPELL_SUMMON_DEFENDO_TANK; break;
                        case NPC_SCAVENGE_A8:       newSpellId = SPELL_SUMMON_SCAVENGE_A8; break;
                        case NPC_SCAVENGE_B6:       newSpellId = SPELL_SUMMON_SCAVENGE_B6; break;
                        case NPC_SENTRY_BOT:        newSpellId = SPELL_SUMMON_SENTRY_BOT; break;
                    }

                    if (const SpellEntry* pSpell = GetSpellStore()->LookupEntry(newSpellId))
                    {
                        pCaster->CastSpell(pCreatureTarget, pSpell->Id, true);

                        if (Pet* pPet = pCaster->FindGuardianWithEntry(pSpell->EffectMiscValue[uiEffIndex]))
                            pPet->CastSpell(pCaster, SPELL_REPROGRAM_KILL_CREDIT, true);

                        pCreatureTarget->ForcedDespawn();
                    }
                }
                return true;
            }
            return true;
        }
        case SPELL_ORB_OF_MURLOC_CONTROL:
        {
            pCreatureTarget->CastSpell(pCaster, SPELL_GREENGILL_SLAVE_FREED, true);

            if (pCreatureTarget->GetTypeId() == TYPEID_UNIT)
                dynamic_cast<Creature*>(pCreatureTarget)->UpdateEntry(NPC_FREED_GREENGILL_SLAVE); // Freed Greengill Slave

            pCreatureTarget->CastSpell(pCreatureTarget, SPELL_ENRAGE, true);

            return true;
        }
    }

    return false;
}

void AddSC_spell_scripts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "spell_dummy_go";
    newscript->pEffectDummyGameObj = &EffectDummyGameObj_spell_dummy_go;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_dummy_npc";
    newscript->pEffectDummyCreature = &EffectDummyCreature_spell_dummy_npc;
    newscript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_npc;
    newscript->RegisterSelf();
}
