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
extern void AddSC_boss_emeriss();
extern void AddSC_boss_taerar();
extern void AddSC_boss_ysondre();

// -- Creature --
extern void AddSC_kobold();
extern void AddSC_generic_creature();
extern void AddSC_defias();
extern void AddSC_lumberjack();
extern void AddSC_miner();
extern void AddSC_boss_shadeoftaerar();

// -- Custom --
extern void AddSC_custom_example();
extern void AddSC_test();

// -- GO --
extern void AddSC_go_teleporter();

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
extern void AddSC_not_selectable();

// -- Servers --

//--------------------
//------ ZONE --------

//Alterac Mountains
//Arathi Highlands
//Ashenvale Forest
//Aunchindoun
//Azshara
extern void AddSC_boss_azuregos();

//Azuremyst Isle
extern void AddSC_injured_draenei();
extern void AddSC_draenei_survivor();

//Badlands
//Barrens
//Black Temple
//Blackfathom Depths
//Blackrock Depths
extern void AddSC_boss_ambassador_flamelash();
extern void AddSC_boss_angerrel();
extern void AddSC_boss_anubshiah();
extern void AddSC_boss_baelgar();
extern void AddSC_boss_doomrel();
extern void AddSC_boss_doperel();
extern void AddSC_boss_draganthaurissan();
extern void AddSC_boss_fineous_darkvire();
extern void AddSC_boss_general_angerforge();
extern void AddSC_boss_gloomrel();
extern void AddSC_boss_golemlordargelmach();
extern void AddSC_boss_gorosh_the_dervish();
extern void AddSC_boss_grizzle();
extern void AddSC_boss_haterel();
extern void AddSC_boss_high_interrogator_gerstahn();
extern void AddSC_boss_lord_incendius();
extern void AddSC_boss_lord_roccor();
extern void AddSC_boss_magmus();
extern void AddSC_boss_moira_bronzebeard();
extern void AddSC_boss_pyromancer_loregrain();
extern void AddSC_boss_seethrel();
extern void AddSC_boss_verek();
extern void AddSC_boss_vilerel();
extern void AddSC_boss_warder_stilgiss();
extern void AddSC_overmaster_pyron();
extern void AddSC_phalanx();


//Blackrock Spire
extern void AddSC_boss_drakkisath();
extern void AddSC_boss_halycon();
extern void AddSC_boss_highlordomokk();
extern void AddSC_boss_mothersmolderweb();
extern void AddSC_boss_overlordwyrmthalak();
extern void AddSC_boss_shadowvosh();
extern void AddSC_boss_thebeast();
extern void AddSC_boss_warmastervoone();
extern void AddSC_boss_quatermasterzigris();

//Blackwing lair
extern void AddSC_boss_razorgore();
extern void AddSC_boss_vael();
extern void AddSC_boss_broodlord();
extern void AddSC_boss_firemaw();
extern void AddSC_boss_ebonroc();
extern void AddSC_boss_flamegor();
extern void AddSC_boss_chromaggus();
extern void AddSC_boss_nefarian();
extern void AddSC_boss_victor_nefarius();

//Blasted lands
extern void AddSC_boss_kruul();
extern void AddSC_boss_teremusthedevourer();

//Burning steppes
//Darkshore
//Deadmines
//Deadwind pass
//Desolace
//Dire Maul
//Dun Morogh
extern void AddSC_npc_narm_faulk();

//Durotar
//Duskwood
//Duskwallow marsh
//Eversong Woods
extern void AddSC_mob_q8346();

//Eastern Plaguelands
extern void AddSC_mob_q5211();
extern void AddSC_npc_darrowshire_spirit();

//Elwynn Forest
extern void AddSC_marshal_mcbride();
extern void AddSC_henze_faulk();

//Felwood
//Feralas
//Ghostlands
//Gnomeregan
//Gruul's Lair
extern void AddSC_boss_gruul();

//Hellfire Citadel
//Hellfire Peninsula
extern void AddSC_boss_doomlordkazzak();

//Hillsbrad Foothills
//Hinterlands
//Karazhan
extern void AddSC_boss_maiden_of_virtue();
extern void AddSC_boss_malchezaar();
extern void AddSC_netherspite_infernal();

//Loch Modan
//Lower Blackrock Spire
//Magtheridon's Lair
//Maraudon
extern void AddSC_boss_landslide();
extern void AddSC_boss_lord_vyletongue();
extern void AddSC_boss_noxxion();
extern void AddSC_boss_ptheradras();
extern void AddSC_boss_rotgrip();
extern void AddSC_boss_tinkerergizlock();
extern void AddSC_celebras_the_cursed();
extern void AddSC_meshlok_the_harvester();
extern void AddSC_razorlash();

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
extern void AddSC_mob_firelord();

//Moonglade
extern void AddSC_silva_filnaveth();
extern void AddSC_bunthen_plainswind();

//Mulgore
extern void AddSC_skorn_whitecloud();

//Nagrand
//Naxxramas
extern void AddSC_boss_anubrekhan();
extern void AddSC_boss_maexxna();
extern void AddSC_boss_patchwerk();
extern void AddSC_boss_razuvious();
extern void AddSC_boss_highlord_mograine();
extern void AddSC_boss_lady_blaumeux();
extern void AddSC_boss_sir_zeliek();
extern void AddSC_boss_thane_korthazz();
extern void AddSC_boss_kelthuzad();

//Netherstorm
//Onyxia's Lair
extern void AddSC_boss_onyxia();

//Ragefire Chasm
//Razorfen Downs
extern void AddSC_boss_amnennar_the_coldbringer();

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
extern void AddSC_boss_theolenkrastinov();
extern void AddSC_boss_illuciabarov();
extern void AddSC_boss_instructormalicia();
extern void AddSC_boss_jandicebarov();
extern void AddSC_boss_kormok();
extern void AddSC_boss_lordalexeibarov();
extern void AddSC_boss_lorekeeperpolkelt();
extern void AddSC_boss_rasfrost();
extern void AddSC_boss_theravenian();
extern void AddSC_boss_vectus();

//Searing gorge
extern void AddSC_npc_lothos_riftwalker();

//Shadowfang keep
//Silithus
extern void AddSC_npc_q8304();
extern void AddSC_npc_q8507_q8731();

//Silverpine forest
//Stockade
//Stonetalon mountains
//Stormwind City
extern void AddSC_npc_bartleby();

//Stratholme
extern void AddSC_boss_fras_siabi();
extern void AddSC_boss_hearthsinger_forresten();
extern void AddSC_boss_magistrate_barthilas();
extern void AddSC_boss_maleki_the_pallid();
extern void AddSC_boss_skul();
extern void AddSC_boss_nerubenkan();
extern void AddSC_boss_the_unforgiven();
extern void AddSC_boss_cannon_master_willey();
extern void AddSC_boss_baroness_anastari();
extern void AddSC_boss_archivist_galford();
extern void AddSC_boss_crimson_hammersmith();
extern void AddSC_boss_ramstein_the_gorger();
extern void AddSC_boss_malor_the_zealous();
extern void AddSC_boss_black_guard_swordsmith();
extern void AddSC_boss_timmy_the_cruel();
extern void AddSC_boss_stonespine();
extern void AddSC_boss_postmaster_malown();
extern void AddSC_boss_baron_rivendare();
extern void AddSC_mob_crimson_battle_mage();
extern void AddSC_mob_crimson_conjuror();
extern void AddSC_mob_crimson_defender();
extern void AddSC_mob_crimson_gallant();
extern void AddSC_mob_crimson_guardsman();
extern void AddSC_mob_crimson_initiate();
extern void AddSC_mob_crimson_inquisitor();
extern void AddSC_mob_crimson_monk();
extern void AddSC_mob_crimson_priest();
extern void AddSC_mob_crimson_rifleman();
extern void AddSC_mob_crimson_sorcerer();
extern void AddSC_mob_bile_spewer();
extern void AddSC_mob_bile_slime();
extern void AddSC_mob_black_guard_sentry();
extern void AddSC_mob_fleshflayer_ghoul();
extern void AddSC_mob_ghoul_ravener();
extern void AddSC_mob_plague_ghoul();
extern void AddSC_mob_eye_of_naxxramas();
extern void AddSC_mob_rockwing_screecher();
extern void AddSC_mob_rockwing_gargoyle();
extern void AddSC_mob_venom_belcher();
extern void AddSC_mob_patchwork_horror();
extern void AddSC_mob_wailing_banshee();
extern void AddSC_mob_shrieking_banshee();
extern void AddSC_mob_vengeful_phantom();
extern void AddSC_mob_wrath_phantom();
extern void AddSC_mob_spiteful_phantom();
extern void AddSC_mob_crypt_crawler();
extern void AddSC_mob_crypt_beast();
extern void AddSC_mob_undead_scarab();
extern void AddSC_mob_skeletal_guardian();
extern void AddSC_mob_skeletal_berserker();
extern void AddSC_mob_mangled_cadaver();
extern void AddSC_mob_ravaged_cadaver();
extern void AddSC_mob_broken_cadaver();
extern void AddSC_mob_mindless_skeleton();
extern void AddSC_mob_thuzadin_acolyte();
extern void AddSC_mob_thuzadin_necromancer();
extern void AddSC_mob_thuzadin_shadowcaster();
extern void AddSC_mob_spectral_citizen();
extern void AddSC_mob_ghostly_citizen();
extern void AddSC_mob_restless_soul();
extern void AddSC_mob_freed_soul();

//Sunken Temple
//Tanaris
//Teldrassil
//Tempest Keep
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
//Zangramarsh
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
    AddSC_boss_emeriss();
    AddSC_boss_taerar();
    AddSC_boss_ysondre();

    // -- Creature --
    AddSC_kobold();
    AddSC_generic_creature();
    AddSC_defias();
    AddSC_lumberjack();
    AddSC_miner();
    AddSC_boss_shadeoftaerar();

    // -- Custom --
    AddSC_custom_example();
    AddSC_test();

    // -- GO --
    AddSC_go_teleporter();

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
    AddSC_not_selectable();

    // -- Servers --

    //--------------------
    //------ ZONE --------

    //Alterac Mountains
    //Arathi Highlands
    //Ashenvale Forest
    //Aunchindoun
    //Azshara
    AddSC_boss_azuregos();

    //Azuremyst Isle
    AddSC_injured_draenei();
    AddSC_draenei_survivor();

    //Badlands
    //Barrens
    //Black Temple
    //Blackfathom Depths
    //Blackrock Depths
    AddSC_boss_baelgar();
    AddSC_boss_draganthaurissan();
    AddSC_boss_golemlordargelmach();
    AddSC_boss_moira_bronzebeard();
    AddSC_boss_ambassador_flamelash();
    AddSC_boss_angerrel();
    AddSC_boss_anubshiah();
    AddSC_boss_doomrel();
    AddSC_boss_doperel();
    AddSC_boss_fineous_darkvire();
    AddSC_boss_general_angerforge();
    AddSC_boss_gloomrel();
    AddSC_boss_gorosh_the_dervish();
    AddSC_boss_grizzle();
    AddSC_boss_haterel();
    AddSC_boss_high_interrogator_gerstahn();
    AddSC_boss_lord_incendius();
    AddSC_boss_lord_roccor();
    AddSC_boss_magmus();
    AddSC_boss_pyromancer_loregrain();
    AddSC_boss_seethrel();
    AddSC_boss_verek();
    AddSC_boss_vilerel();
    AddSC_boss_warder_stilgiss();
    AddSC_overmaster_pyron();
    AddSC_phalanx();

    //Blackrock Spire
    AddSC_boss_drakkisath();
    AddSC_boss_halycon();
    AddSC_boss_highlordomokk();
    AddSC_boss_mothersmolderweb();
    AddSC_boss_overlordwyrmthalak();
    AddSC_boss_shadowvosh();
    AddSC_boss_thebeast();
    AddSC_boss_warmastervoone();
    AddSC_boss_quatermasterzigris();

    //Blackwing lair
    AddSC_boss_razorgore();
    AddSC_boss_vael();
    AddSC_boss_broodlord();
    AddSC_boss_firemaw();
    AddSC_boss_ebonroc();
    AddSC_boss_flamegor();
    AddSC_boss_chromaggus();
    AddSC_boss_nefarian();
    AddSC_boss_victor_nefarius();

    //Blasted lands
    AddSC_boss_kruul();
    AddSC_boss_teremusthedevourer();

    //Burning steppes
    //Darkshore
    //Deadmines
    //Deadwind pass
    //Desolace
    //Dire Maul
    //Dun Morogh
    AddSC_npc_narm_faulk();

    //Durotar
    //Duskwood
    //Duskwallow marsh
    //Eversong Woods
    AddSC_mob_q8346();

    //Eastern Plaguelands   
    AddSC_mob_q5211();
    AddSC_npc_darrowshire_spirit();

    //Elwynn Forest
    AddSC_marshal_mcbride();
    AddSC_henze_faulk();

    //Felwood
    //Feralas
    //Ghostlands
    //Gnomeregan
    //Gruul's Lair
    AddSC_boss_gruul();

    //Hellfire Citadel
    //Hellfire Peninsula
    AddSC_boss_doomlordkazzak();

    //Hillsbrad Foothills
    //Hinterlands
    //Karazhan
    AddSC_boss_maiden_of_virtue();
    AddSC_boss_malchezaar();
    AddSC_netherspite_infernal();

    //Loch Modan
    //Lower Blackrock Spire
    //Magtheridon's Lair
    //Maraudon
    AddSC_boss_ptheradras();
    AddSC_boss_tinkerergizlock();
    AddSC_boss_landslide();
    AddSC_boss_lord_vyletongue();
    AddSC_boss_noxxion();
    AddSC_boss_rotgrip();
    AddSC_celebras_the_cursed();
    AddSC_meshlok_the_harvester();
    AddSC_razorlash();

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
    AddSC_mob_firelord();

    //Moonglade
    AddSC_silva_filnaveth();
    AddSC_bunthen_plainswind();

    //Mulgore
    AddSC_skorn_whitecloud();

    //Nagrand
    //Naxxramas
    AddSC_boss_anubrekhan();
    AddSC_boss_maexxna();
    AddSC_boss_patchwerk();
    AddSC_boss_razuvious();
    AddSC_boss_highlord_mograine();
    AddSC_boss_lady_blaumeux();
    AddSC_boss_sir_zeliek();
    AddSC_boss_thane_korthazz();
    AddSC_boss_kelthuzad();

    //Netherstorm
    //Onyxia's Lair
    AddSC_boss_onyxia();

    //Ragefire Chasm
    //Razorfen Downs
    AddSC_boss_amnennar_the_coldbringer();

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
    AddSC_boss_theolenkrastinov();
    AddSC_boss_illuciabarov();
    AddSC_boss_instructormalicia();
    AddSC_boss_jandicebarov();
    AddSC_boss_kormok();
    AddSC_boss_lordalexeibarov();
    AddSC_boss_lorekeeperpolkelt();
    AddSC_boss_rasfrost();
    AddSC_boss_theravenian();
    AddSC_boss_vectus();

    //Searing gorge
    AddSC_npc_lothos_riftwalker();
    //Shadowfang keep
    //Silithus
    AddSC_npc_q8304();
    AddSC_npc_q8507_q8731();

    //Silverpine forest
    //Stockade
    //Stonetalon mountains
    //Stormwind City
    AddSC_npc_bartleby();

    //Stratholme
    AddSC_boss_fras_siabi();
    AddSC_boss_hearthsinger_forresten();
    AddSC_boss_magistrate_barthilas();
    AddSC_boss_maleki_the_pallid();
    AddSC_boss_skul();
    AddSC_boss_nerubenkan();
    AddSC_boss_the_unforgiven();
    AddSC_boss_cannon_master_willey();
    AddSC_boss_baroness_anastari();
    AddSC_boss_archivist_galford();
    AddSC_boss_crimson_hammersmith();
    AddSC_boss_ramstein_the_gorger();
    AddSC_boss_malor_the_zealous();
    AddSC_boss_black_guard_swordsmith();
    AddSC_boss_timmy_the_cruel();
    AddSC_boss_stonespine();
    AddSC_boss_postmaster_malown();
    AddSC_boss_baron_rivendare();
    AddSC_mob_crimson_battle_mage();
    AddSC_mob_crimson_conjuror();
    AddSC_mob_crimson_defender();
    AddSC_mob_crimson_gallant();
    AddSC_mob_crimson_guardsman();
    AddSC_mob_crimson_initiate();
    AddSC_mob_crimson_inquisitor();
    AddSC_mob_crimson_monk();
    AddSC_mob_crimson_priest();
    AddSC_mob_crimson_rifleman();
    AddSC_mob_crimson_sorcerer();
    AddSC_mob_bile_spewer();
    AddSC_mob_bile_slime();
    AddSC_mob_black_guard_sentry();
    AddSC_mob_fleshflayer_ghoul();
    AddSC_mob_ghoul_ravener();
    AddSC_mob_plague_ghoul();
    AddSC_mob_eye_of_naxxramas();
    AddSC_mob_rockwing_screecher();
    AddSC_mob_rockwing_gargoyle();
    AddSC_mob_venom_belcher();
    AddSC_mob_patchwork_horror();
    AddSC_mob_wailing_banshee();
    AddSC_mob_shrieking_banshee();
    AddSC_mob_vengeful_phantom();
    AddSC_mob_wrath_phantom();
    AddSC_mob_spiteful_phantom();
    AddSC_mob_crypt_crawler();
    AddSC_mob_crypt_beast();
    AddSC_mob_undead_scarab();
    AddSC_mob_skeletal_guardian();
    AddSC_mob_skeletal_berserker();
    AddSC_mob_mangled_cadaver();
    AddSC_mob_ravaged_cadaver();
    AddSC_mob_broken_cadaver();
    AddSC_mob_mindless_skeleton();
    AddSC_mob_thuzadin_acolyte();
    AddSC_mob_thuzadin_necromancer();
    AddSC_mob_thuzadin_shadowcaster();
    AddSC_mob_spectral_citizen();
    AddSC_mob_ghostly_citizen();
    AddSC_mob_restless_soul();
    AddSC_mob_freed_soul();

    //Sunken Temple
    //Tanaris
    //Teldrassil
    //Tempest Keep
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
    //Zangramarsh
    //Zul'Farrak
    //Zul'Gurub

    // -------------------

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
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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
    if (!victim || m_creature->m_currentSpell)
        return;
    m_creature->CastSpell(victim, spelId, false);
}

void ScriptedAI::DoCastSpell(Unit* who,SpellEntry const *spellInfo)
{
    if (!who || m_creature->m_currentSpell)
        return;

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
    ThreatList m_threatlist;
    m_threatlist = m_creature->GetThreatList();

    ThreatList::iterator  i = m_threatlist.begin();

    if (position >= m_threatlist.size() || !m_threatlist.size())
        return NULL;

    switch (target)
    {
        case SELECT_TARGET_RANDOM:
            advance ( i , rand()%m_threatlist.size());
            return Unit::GetUnit((*m_creature), i->UnitGuid);
        break;

        case SELECT_TARGET_TOPAGGRO:
            advance ( i , position);
            return Unit::GetUnit((*m_creature), i->UnitGuid);
        break;

        case SELECT_TARGET_BOTTOMAGGRO:
            advance ( i , position);
            return Unit::GetUnit((*m_creature), i->UnitGuid);
        break;
    }


    return NULL;
}

SpellEntry const* ScriptedAI::SelectSpell(Unit* Target, int32 School, int32 Mechanic, SelectTarget Targets, uint32 PowerCostMin, uint32 PowerCostMax, float RangeMin, float RangeMax, SelectEffect Effects)
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
        if (School >= 0 && TempSpell->School != School)
            continue;

        //Check for spell mechanic if specified
        if (Mechanic >= 0 && TempSpell->Mechanic != Mechanic)
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

        for (int j=0; j<3; j++)
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
