/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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


#include "config.h"
#include "ScriptMgr.h"
#include "../../game/TargetedMovementGenerator.h"

uint8 loglevel = 0;
int nrscripts;
Script *m_scripts[MAX_SCRIPTS];

// Spell summary for ScriptedAI::SelectSpell
struct TSpellSummary {
    uint8 Targets;    // set of enum SelectTarget
    uint8 Effects;    // set of enum SelectEffect 
} *SpellSummary;

void FillSpellSummary();

// -- Scripts to be added --

// -- Area --

// -- Boss --

// -- Creature --
extern void AddSC_kobold();
extern void AddSC_generic_creature();
extern void AddSC_defias();
extern void AddSC_lumberjack();
extern void AddSC_miner();

// -- Custom --
extern void AddSC_custom_example();
extern void AddSC_test();

// -- GO --

// -- Guard --
extern void AddSC_guard_bluffwatcher();
extern void AddSC_guard_contested();
extern void AddSC_guard_darnassus();
extern void AddSC_guard_dunmorogh();
extern void AddSC_guard_durotar();
extern void AddSC_guard_elwynnforest();
extern void AddSC_guard_exodar();
extern void AddSC_guard_ironforge();
extern void AddSC_guard_mulgore();
extern void AddSC_guard_orgrimmar();
extern void AddSC_guard_silvermoon();
extern void AddSC_guard_stormwind();
extern void AddSC_guard_teldrassil();
extern void AddSC_guard_tirisfal();
extern void AddSC_guard_undercity();


// -- Honor --
extern void AddSC_Honor_Vendor();

// -- Item --
extern void AddSC_item_test();

// -- NPC --

// -- Servers --
extern void AddSC_battlemaster();
extern void AddSC_guildmaster();
extern void AddSC_travelmaster();

//--------------------
//------ ZONE --------

//Alterac Mountains
//Arathi Highlands
//Ashenvale Forest
//Azshara
//Badlands
//Barrens
//Blackfathom Depths
//Blackwing lair
extern void AddSC_boss_razorgore();
extern void AddSC_boss_vael();
extern void AddSC_boss_broodlord();
extern void AddSC_boss_firemaw();
extern void AddSC_boss_ebonroc();
extern void AddSC_boss_flamegor();
extern void AddSC_boss_chromaggus();
extern void AddSC_boss_nefarian();

//Blasted lands
//Burning steppes
//Darkshore
//Deadmines
//Deadwind pass
//Desolace
//Dire Maul
//Dun Morogh
//Durotar
//Duskwood
//Duskwallow marsh
//Eastern Plaguelands
//Elwynn Forest
extern void AddSC_marshal_mcbride();
extern void AddSC_henze_faulk();

//Felwood
//Feralas
//Gnomeregan
//Hillsbrad Foothills
//Hinterlands
//Loch Modan
//Lower Blackrock Spire
//Maradon
//Molten core
extern void AddSC_boss_lucifron();
extern void AddSC_boss_magmadar();
extern void AddSC_boss_gehennas();
extern void AddSC_boss_garr();
extern void AddSC_boss_baron_geddon();
extern void AddSC_boss_shazzrah();
extern void AddSC_boss_golemagg();
extern void AddSC_boss_sulfuron();
extern void AddSC_boss_majordomo();
extern void AddSC_boss_ragnaros();

//Moonglade
extern void AddSC_silva_filnaveth();
extern void AddSC_bunthen_plainswind();

//Mulgore
extern void AddSC_skorn_whitecloud();

//Naxxramas
extern void AddSC_boss_anubrekhan();
extern void AddSC_boss_maexxna();
extern void AddSC_boss_patchwerk();
extern void AddSC_boss_razuvious();
extern void AddSC_boss_kelthuzad();

//Onyxia's Lair
extern void AddSC_boss_onyxia();

//Ragefire Chasm
//Razorfen Downs
//Redridge Mountains
//Ruins of Ahn'Qiraj
//Scarlet Monastery
extern void AddSC_boss_arcanist_doan();
extern void AddSC_boss_azshir_the_sleepless();
extern void AddSC_boss_bloodmage_thalnos();
extern void AddSC_boss_herod();
extern void AddSC_boss_high_inquisitor_fairbanks();
extern void AddSC_boss_high_inquisitor_whitemane();
extern void AddSC_boss_houndmaster_loksey();
extern void AddSC_boss_interrogator_vishas();
extern void AddSC_boss_scarlet_commander_mograine();
extern void AddSC_boss_scorn();
extern void AddSC_scarlet_torturer();

//Scholomance
//Searing gorge
//Shadowfang keep
//Silithus
//Silverpine forest
//Stockade
//Stonetalon mountains
//Stratholme
//Sunken Temple
//Tanaris
//Teldrassil
//Temple of ahn'qiraj
//Thousand Needles
//Tirisfal Glades
//Uldaman
//Un'Goro Crater
//Upper blackrock spire
//Wailing caverns
//Western plaguelands
//Westfall
//Wetlands
//Winterspring
//Zul'Farrak
//Zul'Gurub

// -------------------

MANGOS_DLL_EXPORT
void ScriptsFree()
{   
    // Free Spell Summary
    delete []SpellSummary;

    // Free resources before library unload
    for(int i=0;i<nrscripts;i++)
        delete m_scripts[i];
}

MANGOS_DLL_EXPORT
void ScriptsInit()
{
    nrscripts = 0;
    for(int i=0;i<MAX_SCRIPTS;i++)
        m_scripts[i]=NULL;

    FillSpellSummary();
    // -- Scripts to be added --

    // -- Area --

    // -- Boss --

    // -- Creature --
    AddSC_kobold();
    AddSC_generic_creature();
    AddSC_defias();
    AddSC_lumberjack();
    AddSC_miner();

    // -- Custom --
    AddSC_custom_example();
    AddSC_test();

    // -- GO --

    // -- Guard --
    AddSC_guard_bluffwatcher();
    AddSC_guard_contested();
    AddSC_guard_darnassus();
    AddSC_guard_dunmorogh();
    AddSC_guard_durotar();
    AddSC_guard_elwynnforest();
    AddSC_guard_exodar();
    AddSC_guard_ironforge();
    AddSC_guard_mulgore();
    AddSC_guard_orgrimmar();
    AddSC_guard_silvermoon();
    AddSC_guard_stormwind();
    AddSC_guard_teldrassil();
    AddSC_guard_tirisfal();
    AddSC_guard_undercity();

    // -- Honor --
    AddSC_Honor_Vendor();

    // -- Item --
    AddSC_item_test();

    // -- NPC --

    // -- Servers --
    AddSC_battlemaster();
    AddSC_guildmaster();
    AddSC_travelmaster();

    //--------------------
    //------ ZONE --------

    //Alterac Mountains
    //Arathi Highlands
    //Ashenvale Forest
    //Azshara
    //Badlands
    //Barrens
    //Blackfathom Depths
    //Blackwing lair
    AddSC_boss_razorgore();
    AddSC_boss_vael();
    AddSC_boss_broodlord();
    AddSC_boss_firemaw();
    AddSC_boss_ebonroc();
    AddSC_boss_flamegor();
    AddSC_boss_chromaggus();
    AddSC_boss_nefarian();

    //Blasted lands
    //Burning steppes
    //Darkshore
    //Deadmines
    //Deadwind pass
    //Desolace
    //Dire Maul
    //Dun Morogh
    //Durotar
    //Duskwood
    //Duskwallow marsh
    //Eastern Plaguelands
    //Elwynn Forest
    AddSC_marshal_mcbride();
    AddSC_henze_faulk();

    //Felwood
    //Feralas
    //Gnomeregan
    //Hillsbrad Foothills
    //Hinterlands
    //Loch Modan
    //Lower Blackrock Spire
    //Maradon
    //Molten core
    AddSC_boss_lucifron();
    AddSC_boss_magmadar();
    AddSC_boss_gehennas();
    AddSC_boss_garr();
    AddSC_boss_baron_geddon();
    AddSC_boss_shazzrah();
    AddSC_boss_golemagg();
    AddSC_boss_sulfuron();
    AddSC_boss_majordomo();
    AddSC_boss_ragnaros();

    //Moonglade
    AddSC_silva_filnaveth();
    AddSC_bunthen_plainswind();

    //Mulgore
    AddSC_skorn_whitecloud();

    //Naxxramas
    AddSC_boss_anubrekhan();
    AddSC_boss_maexxna();
    AddSC_boss_patchwerk();
    AddSC_boss_razuvious();
    AddSC_boss_kelthuzad();

    //Onyxia's Lair
    AddSC_boss_onyxia();

    //Ragefire Chasm
    //Razorfen Downs
    //Redridge Mountains
    //Ruins of Ahn'Qiraj
    //Scarlet Monastery
    AddSC_boss_arcanist_doan();
    AddSC_boss_azshir_the_sleepless();
    AddSC_boss_bloodmage_thalnos();
    AddSC_boss_herod();
    AddSC_boss_high_inquisitor_fairbanks();
    AddSC_boss_high_inquisitor_whitemane();
    AddSC_boss_houndmaster_loksey();
    AddSC_boss_interrogator_vishas();
    AddSC_boss_scarlet_commander_mograine();
    AddSC_boss_scorn();
    AddSC_scarlet_torturer();

    //Scholomance
    //Searing gorge
    //Shadowfang keep
    //Silithus
    //Silverpine forest
    //Stockade
    //Stonetalon mountains
    //Stratholme
    //Sunken Temple
    //Tanaris
    //Teldrassil
    //Temple of ahn'qiraj
    //Thousand Needles
    //Tirisfal Glades
    //Uldaman
    //Un'Goro Crater
    //Upper blackrock spire
    //Wailing caverns
    //Western plaguelands
    //Westfall
    //Wetlands
    //Winterspring
    //Zul'Farrak
    //Zul'Gurub

    // ----------------------------------------
}

//*********************************
//*** Functions used internally ***

Script* GetScriptByName(std::string Name)
{
    for(int i=0;i<MAX_SCRIPTS;i++)
    {
        if( m_scripts[i] && m_scripts[i]->Name == Name )
            return m_scripts[i];
    }
    return NULL;
}

//********************************
//*** Functions to be Exported ***

MANGOS_DLL_EXPORT
bool GossipHello ( Player * player, Creature *_Creature )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipHello(player,_Creature);
}

MANGOS_DLL_EXPORT
bool GossipSelect( Player *player, Creature *_Creature,uint32 sender, uint32 action )
{
    Script *tmpscript = NULL;

    printf("action: %d\n",action);

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelect(player,_Creature,sender,action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode( Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelectWithCode) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelectWithCode(player,_Creature,sender,action,sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestSelect( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestComplete( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestComplete) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool ChooseReward( Player *player, Creature *_Creature, Quest *_Quest, uint32 opt )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pChooseReward(player,_Creature,_Quest,opt);
}

MANGOS_DLL_EXPORT
uint32 NPCDialogStatus( Player *player, Creature *_Creature )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pNPCDialogStatus) return 100;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pNPCDialogStatus(player,_Creature);
}

MANGOS_DLL_EXPORT
bool ItemHello( Player *player, Item *_Item, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept( Player *player, Item *_Item, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemQuestAccept(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool GOHello( Player *player, GameObject *_GO )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOHello(player,_GO);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept( Player *player, GameObject *_GO, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(player,_GO,_Quest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward( Player *player, GameObject *_GO, Quest *_Quest, uint32 opt )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(player,_GO,_Quest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger      ( Player *player, Quest *_Quest, uint32 triggerID )
{
    Script *tmpscript = NULL;

    // TODO: load a script name for the areatriggers
    //tmpscript = GetScriptByName();
    if(!tmpscript || !tmpscript->pAreaTrigger) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pAreaTrigger(player,_Quest,triggerID);
}

MANGOS_DLL_EXPORT
CreatureAI* GetAI(Creature *_Creature)
{
    Script *tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);

    if(!tmpscript || !tmpscript->GetAI) return NULL;
    return tmpscript->GetAI(_Creature);
}

MANGOS_DLL_EXPORT
bool ItemUse( Player *player, Item* _Item)
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemUse) return false;

    return tmpscript->pItemUse(player,_Item);
}

MANGOS_DLL_EXPORT
bool ReceiveEmote( Player *player, Creature *_Creature, uint32 emote )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pReceiveEmote) return false;

    return tmpscript->pReceiveEmote(player, _Creature, emote);
}

void ScriptedAI::AttackStart(Unit* who)
{
    if (!who)
        return;

    if (who->isTargetableForAttack())
    {
        //Begin attack
        DoStartMeleeAttack(who);
    }
}

void ScriptedAI::UpdateAI(const uint32 diff)
{
    //Check if we have a current target
    if( m_creature->isAlive() && m_creature->SelectHostilTarget())
    {
        //Check if we should stop attacking because our victim is no longer in range
        if (CheckTether())
        {
            EnterEvadeMode();
            return;
        }
            
        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
        {
            if( m_creature->isAttackReady() )
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
}

void ScriptedAI::EnterEvadeMode()
{
    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void ScriptedAI::DoStartMeleeAttack(Unit* victim)
{
    if (!victim)
        return;

    if ( m_creature->Attack(victim) )
    {
        (*m_creature)->Mutate(new TargetedMovementGenerator(*victim));
        m_creature->AddThreat(victim, 0.0f);
        m_creature->resetAttackTimer();

        if (victim->GetTypeId() == TYPEID_PLAYER)
            m_creature->SetLootRecipient((Player*)victim);
    }
}

void ScriptedAI::DoStartRangedAttack(Unit* victim)
{
    if (!victim)
        return;

    if (m_creature->Attack(victim))
    {
        m_creature->AddThreat(victim, 0.0f);
        m_creature->resetAttackTimer();

        if (victim->GetTypeId() == TYPEID_PLAYER)
            m_creature->SetLootRecipient((Player*)victim);
    }
}


void ScriptedAI::DoStopAttack()
{
    if( m_creature->getVictim() != NULL )
    {
        m_creature->AttackStop();
    }
}

void ScriptedAI::DoCast(Unit* victim, uint32 spelId)
{
    m_creature->CastSpell(victim, spelId, false);
}

void ScriptedAI::DoCastSpell(Unit* who,SpellEntry const *spellInfo)
{
    m_creature->StopMoving();
    m_creature->CastSpell(who, spellInfo, false);
}

void ScriptedAI::DoSay(const char* text, uint32 language, Unit* target)
{
    if (target)m_creature->Say(text, language, target->GetGUID());
    else m_creature->Say(text, language, 0);
}

void ScriptedAI::DoYell(const char* text, uint32 language, Unit* target)
{
    if (target)m_creature->Yell(text, language, target->GetGUID());
    else m_creature->Yell(text, language, 0);
}

void ScriptedAI::DoTextEmote(const char* text, Unit* target)
{
    if (target)m_creature->TextEmote(text, target->GetGUID());
    else m_creature->TextEmote(text, 0);
}

void ScriptedAI::DoGoHome()
{
    if( !m_creature->getVictim() && m_creature->isAlive() )
    {
        if( (*m_creature)->top()->GetMovementGeneratorType() == TARGETED_MOTION_TYPE )
            (*m_creature)->TargetedHome();
    }
}

bool ScriptedAI::needToStop() const
{
    if (!m_creature->getVictim() || !m_creature->isAlive())
        return true;

    return false;
}

void ScriptedAI::DoPlaySoundToSet(Unit* unit, uint32 sound)
{
    if (!unit)
        return;

    WorldPacket data(4);
    data.SetOpcode(SMSG_PLAY_SOUND);
    data << uint32(sound);
    unit->SendMessageToSet(&data,false);
}

void ScriptedAI::DoFaceTarget(Unit *unit)
{
    if (!unit)
        return;

    //Face target
    m_creature->SetInFront(unit);
}

Creature* ScriptedAI::DoSpawnCreature(uint32 id, float x, float y, float z, float angle, TempSummonType t, uint32 despawntime)
{
    return m_creature->SummonCreature(id,m_creature->GetMapId(),m_creature->GetPositionX() + x,m_creature->GetPositionY() + y,m_creature->GetPositionZ() + z, angle,t,despawntime);
}

Unit* ScriptedAI::SelectUnit(SelectAggroTarget target, uint32 position)
{
    Unit::AttackerSet m_attackers;
    m_attackers = m_creature->getAttackers();

    Unit::AttackerSet::iterator  i = m_attackers.begin();

    if (position > m_attackers.size() || !m_attackers.size())
        return NULL;

    switch (target)
    {
        case SELECT_TARGET_RANDOM:
            advance ( i , rand()%m_attackers.size());
            return (*i);
        break;

        case SELECT_TARGET_TOPAGGRO:
            advance ( i , position);
            return (*i);
        break;

        case SELECT_TARGET_BOTTOMAGGRO:
            advance ( i , position);
            return (*i);
        break;
    }

    return NULL;
}

SpellEntry const* ScriptedAI::SelectSpell(Unit* Target, uint32 School, uint32 Mechanic, SelectTarget Targets, uint32 PowerCostMin, uint32 PowerCostMax, float RangeMin, float RangeMax, SelectEffect Effects)
{
    //No target so we can't cast
    if (!Target)
        return false;

    //Silenced so we can't cast
    if (m_creature->m_silenced)
        return false;

    //Using the extended script system we first create a list of viable spells
    SpellEntry const* Spell[4];
    Spell[0] = 0;
    Spell[1] = 0;
    Spell[2] = 0;
    Spell[3] = 0;

    uint32 SpellCount = 0;

    SpellEntry const* TempSpell;
    SpellRangeEntry const* TempRange;

    //Check if each spell is viable(set it to null if not)
    for (uint32 i = 0; i < 4; i++)
    {
        TempSpell = GetSpellStore()->LookupEntry(m_creature->m_spells[i]);

        //This spell doesn't exist
        if (!TempSpell)
            continue;

        // Targets and Effects checked first as most used restrictions
        //Check the spell targets if specified
        if ( Targets && !(SpellSummary[m_creature->m_spells[i]].Targets & (1 << (Targets-1))) )
            continue;

        //Check the type of spell if we are looking for a specific spell type
        if ( Effects && !(SpellSummary[m_creature->m_spells[i]].Effects & (1 << (Effects-1))) )
            continue;

        //Check for school if specified
        if (School && TempSpell->School != School)
            continue;

        //Check for spell mechanic if specified
        if (Mechanic && TempSpell->Mechanic != Mechanic)
            continue;

        //Make sure that the spell uses the requested amount of power
        if (PowerCostMin &&  TempSpell->manaCost < PowerCostMin)
            continue;

        if (PowerCostMax && TempSpell->manaCost > PowerCostMax)
            continue;

        //Continue if we don't have the mana to actually cast this spell
        if (TempSpell->manaCost > m_creature->GetPower((Powers)TempSpell->powerType))
            continue;

        //Get the Range
        TempRange = GetSpellRangeStore()->LookupEntry(TempSpell->rangeIndex);

        //Spell has invalid range store so we can't use it
        if (!TempRange)
            continue;

        //Check if the spell meets our range requirements
        if (RangeMin && TempRange->maxRange < RangeMin)
            continue;
        if (RangeMax && TempRange->maxRange > RangeMax)
            continue;

        //Check if our target is in range
        if (m_creature->IsWithinDistInMap(Target, TempRange->minRange) || !m_creature->IsWithinDistInMap(Target, TempRange->maxRange))
            continue;

        //All good so lets add it to the spell list
        Spell[SpellCount] = TempSpell;
        SpellCount++;
    }

    //We got our usable spells so now lets randomly pick one
    if (!SpellCount)
        return NULL;

    return Spell[rand()%SpellCount];
}

bool ScriptedAI::CanCast(Unit* Target, SpellEntry const *Spell)
{
    //No target so we can't cast
    if (!Target)
        return false;

    //Silenced so we can't cast
    if (m_creature->m_silenced)
        return false;

    //Check for power
    if (m_creature->GetPower((Powers)Spell->powerType) < Spell->manaCost)
        return false;

    SpellRangeEntry const *TempRange = NULL;

    TempRange = GetSpellRangeStore()->LookupEntry(Spell->rangeIndex);

    //Spell has invalid range store so we can't use it
    if (!TempRange)
        return false;

    //Unit is out of range of this spell
    if (m_creature->GetDistanceSq(Target) > TempRange->maxRange*TempRange->maxRange || m_creature->GetDistanceSq(Target) < TempRange->minRange*TempRange->minRange)
        return false;

    return true;
}

bool ScriptedAI::CheckTether()
{
    float rx,ry,rz;
    m_creature->GetRespawnCoord(rx, ry, rz);
    float spawndist = m_creature->GetDistanceSq(rx,ry,rz);
    //float length = m_creature->GetDistanceSq(m_creature->getVictim());
    return ( spawndist > CREATURE_THREAT_RADIUS );
}

void FillSpellSummary()
{
    SpellSummary = new TSpellSummary[GetSpellStore()->GetNumRows()];

    SpellEntry const* TempSpell;

    for (int i=0; i < GetSpellStore()->GetNumRows(); i++ )
    {
        SpellSummary[i].Effects = 0;
        SpellSummary[i].Targets = 0;

        TempSpell = GetSpellStore()->LookupEntry(i);
        //This spell doesn't exist
        if (!TempSpell)
            continue;

        for (int j=0; j<4; j++)
        {
            //Spell targets self
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_SELF )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SELF-1);

            //Spell targets a single enemy
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_CHAIN_DAMAGE ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_CURRENT_SELECTED_ENEMY )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SINGLE_ENEMY-1);

            //Spell targets AoE at enemy
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_INSTANT ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMIES_AROUND_CASTER ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_CHANNELED )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_ENEMY-1);

            //Spell targets an enemy
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_CHAIN_DAMAGE ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_CURRENT_SELECTED_ENEMY ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_INSTANT ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMIES_AROUND_CASTER ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_CHANNELED )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_ENEMY-1);

            //Spell targets a single friend(or self)
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_SELF ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_FRIEND ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_PARTY )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SINGLE_FRIEND-1);

            //Spell targets aoe friends
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_PARTY_AROUND_CASTER ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_AREAEFFECT_PARTY )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_FRIEND-1);

            //Spell targets any friend(or self)
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_SELF ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_FRIEND ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_PARTY ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_PARTY_AROUND_CASTER ||
                 TempSpell->EffectImplicitTargetA[j] == TARGET_AREAEFFECT_PARTY )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_FRIEND-1);

            //Make sure that this spell includes a damage effect
            if ( TempSpell->Effect[j] == SPELL_EFFECT_SCHOOL_DAMAGE || 
                 TempSpell->Effect[j] == SPELL_EFFECT_INSTAKILL || 
                 TempSpell->Effect[j] == SPELL_EFFECT_ENVIRONMENTAL_DAMAGE || 
                 TempSpell->Effect[j] == SPELL_EFFECT_HEALTH_LEECH )
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_DAMAGE-1);

            //Make sure that this spell includes a healing effect
            if ( TempSpell->Effect[j] == SPELL_EFFECT_HEAL || 
                 TempSpell->Effect[j] == SPELL_EFFECT_HEAL_MAX_HEALTH || 
                 TempSpell->Effect[j] == SPELL_EFFECT_HEAL_MECHANICAL )
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_HEALING-1);

            //Make sure that this spell applies an aura
            if ( TempSpell->Effect[j] == SPELL_EFFECT_APPLY_AURA )
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_AURA-1);
       }
    }
}
