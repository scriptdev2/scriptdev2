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
#include "../../shared/WorldPacket.h"
#include "../../game/Player.h"
#include "../../game/GameObject.h"
#include "../../game/GossipDef.h"
#include "../../game/QuestDef.h"
#include "../../game/TargetedMovementGenerator.h"
#include "../../game/Spell.h"
#include "../../shared/Database/DBCStores.h"

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

// -- Custom --
extern void AddSC_custom_example();
extern void AddSC_test();

// -- GO --
extern void AddSC_go_teleporter();
extern void AddSC_go_orb_of_command();
extern void AddSC_go_barov_journal();

// -- Guard --
extern void AddSC_guards();

// -- Honor --
extern void AddSC_Honor_Vendor();

// -- Item --
extern void AddSC_item_test();
extern void AddSC_area_52_special();
extern void AddSC_draenei_fishing_net();
extern void AddSC_nether_wraith_beacon();
extern void AddSC_purification_mixture();
extern void AddSC_vorenthals_presence();

// -- NPC --
extern void AddSC_not_selectable();
extern void AddSC_npc_guardian();

// -- Servers --

//--------------------
//------ ZONE --------

//Alterac Mountains
//Arathi Highlands
//Ashenvale Forest
//Aunchindoun
//--Auchenai Crypts
extern void AddSC_boss_shirrak_the_dead_watcher();
//--Mana Tombs
extern void AddSC_boss_nexusprince_shaffar();
extern void AddSC_boss_pandemonius();
extern void AddSC_boss_tavarok();

//--Sekketh Halls
extern void AddSC_boss_darkweaver_syth();
extern void AddSC_boss_tailonking_ikiss();

//--Shadow Labyrinth
extern void AddSC_boss_ambassador_hellmaw();
extern void AddSC_boss_blackheart_the_inciter();
extern void AddSC_boss_grandmaster_vorpil();
extern void AddSC_boss_murmur();
extern void AddSC_instance_shadow_labyrinth();

//Azshara
extern void AddSC_boss_azuregos();
extern void AddSC_mobs_spitelashes();

//Azuremyst Isle
extern void AddSC_injured_draenei();
extern void AddSC_draenei_survivor();

//Badlands
//Barrens
extern void AddSC_npc_beaten_corpse();

//Black Temple
extern void AddSC_mob_blacktemple();
extern void AddSC_boss_illidan();
extern void AddSC_boss_shade_of_akama();
extern void AddSC_boss_supremus();
extern void AddSC_boss_gurtogg_bloodboil();
extern void AddSC_boss_mother_shahraz();
extern void AddSC_boss_reliquary_of_souls();
extern void AddSC_boss_teron_gorefiend();
extern void AddSC_boss_najentus();
extern void AddSC_boss_illidari_council();
extern void AddSC_instance_black_temple();

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
extern void AddSC_npc_lokhtos_darkbargainer();

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
extern void AddSC_boss_pyroguard_emberseer();
extern void AddSC_boss_gyth();
extern void AddSC_boss_rend_blackhand();
extern void AddSC_mob_chromatic_elite_guard();

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

//Blade's Edge Mountains
extern void AddSC_mobs_blades_edge_mountains();

//Blasted lands
extern void AddSC_boss_kruul();
extern void AddSC_boss_teremusthedevourer();

//Burning steppes
//Caverns of Time
//--Battle for Mt. Hyjal
//--Old Hillsbrad
extern void AddSC_boss_captain_skarloc();
extern void AddSC_boss_epoch_hunter();
extern void AddSC_boss_lieutenant_drake();

//--The Dark Portal
extern void AddSC_boss_aeonus();
extern void AddSC_boss_chrono_lord_deja();
extern void AddSC_boss_temporus();

//Coilfang Resevoir
//--Serpent Shrine Cavern
extern void AddSC_boss_fathomlord_karathress();
extern void AddSC_boss_hydross_the_unstable();
extern void AddSC_boss_leotheras_the_blind();
extern void AddSC_boss_morogrim_tidewalker();
extern void AddSC_instance_serpentshrine_cavern();
extern void AddSC_mob_serpentshrine_cavern();

//--Slave Pens
extern void AddSC_boss_rokmar_the_crackler();

//--Steam Vault
extern void AddSC_boss_warlord_kalithresh();
extern void AddSC_boss_hydromancer_thespia();
extern void AddSC_instance_steam_vault();

//--Underbog
extern void AddSC_boss_ghazan();
extern void AddSC_boss_hungarfen();

//Darkshore
extern void AddSC_mobs_darkshore();

//Darnassus
//Deadmines
extern void AddSC_boss_deadmines();

//Deadwind pass
//Desolace
//Dire Maul
//Dun Morogh
extern void AddSC_npc_narm_faulk();

//Durotar
//Duskwood
//Duskwallow marsh
extern void AddSC_npc_lady_jaina_proudmoore();

//Eversong Woods
extern void AddSC_mobs_mana_tapped();

//Exodar
//Eastern Plaguelands
extern void AddSC_mobs_ghoul_flayer();
extern void AddSC_npc_darrowshire_spirit();

//Elwynn Forest
extern void AddSC_npc_henze_faulk();

//Felwood
//Feralas
//Ghostlands
//Gnomeregan
//Gruul's Lair
extern void AddSC_boss_gruul();
extern void AddSC_boss_high_king_maulgar();
extern void AddSC_instance_gruuls_lair();
extern void AddSC_mob_gruuls_lair();

//Hellfire Citadel
//--Blood Furnace
extern void AddSC_boss_broggok();
extern void AddSC_boss_kelidan_the_breaker();
extern void AddSC_boss_the_maker();

//--Magtheridon's Lair
extern void AddSC_boss_magtheridon();

//--Shattered Halls
extern void AddSC_boss_grand_warlock_nethekurse();

//--Ramparts
extern void AddSC_boss_watchkeeper_gargolmar();
extern void AddSC_boss_omor_the_unscarred();

//Hellfire Peninsula
extern void AddSC_boss_doomlordkazzak();
extern void AddSC_mobs_hellfire_peninsula();
extern void AddSC_npcs_hellfire_peninsula();

//Hillsbrad Foothills
//Hinterlands
//Ironforge
//Karazhan
extern void AddSC_boss_attumen();
extern void AddSC_boss_curator();
extern void AddSC_boss_maiden_of_virtue();
extern void AddSC_boss_shade_of_aran();
extern void AddSC_boss_malchezaar();
extern void AddSC_boss_terestian_illhoof();
extern void AddSC_mob_homunculus();
extern void AddSC_mob_kilrek();
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
extern void AddSC_mob_molten_giant();
extern void AddSC_mob_ancient_core_hound();
extern void AddSC_mob_molten_destroyer();
extern void AddSC_mob_firewalker();
extern void AddSC_mob_flame_guard();
extern void AddSC_mob_flamewaker();
extern void AddSC_mob_flamewaker_elite();
extern void AddSC_mob_flamewaker_healer();
extern void AddSC_mob_flamewaker_protector();
extern void AddSC_mob_core_rager();
extern void AddSC_mob_lavaspawn();

//Moonglade
extern void AddSC_silva_filnaveth();
extern void AddSC_bunthen_plainswind();
extern void AddSC_npc_great_bear_spirit();

//Mulgore
extern void AddSC_skorn_whitecloud();

//Nagrand
extern void AddSC_mobs_nagrand();
extern void AddSC_npcs_nagrand();

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
extern void AddSC_boss_faerlina();
extern void AddSC_boss_loatheb();
extern void AddSC_boss_noth();
extern void AddSC_boss_gluth();

//Netherstorm
extern void AddSC_npcs_netherstorm();

//Onyxia's Lair
extern void AddSC_boss_onyxia();

//Orgrimmar
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
extern void AddSC_boss_death_knight_darkreaver();
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
//Shadowmoon Valley
extern void AddSC_neltharaku();
extern void AddSC_mobs_shadowmoon_valley();
extern void AddSC_npcs_shadowmoon_valley();

//Shattrath
extern void AddSC_npc_shattrathflaskvendors();

//Silithus
extern void AddSC_npcs_captains_blackanvil_and_skullsplit();
extern void AddSC_npcs_rutgar_and_frankal();

//Silvermoon
extern void AddSC_npc_blood_knight_stillblade();

//Silverpine forest
//Stockade
//Stonetalon mountains
//Stormwind City
extern void AddSC_npc_bartleby();
extern void AddSC_npc_dashel_stonefist();

//Stranglethorn Vale
extern void AddSC_mobs_stranglethorn_vale();

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
extern void AddSC_mobs_stratholme();

//Sunken Temple
//Tanaris
extern void AddSC_npcs_tanaris();

//Teldrassil
//Tempest Keep
//--Arcatraz
//--Botanica
//--The Eye
extern void AddSC_boss_kaelthas();
extern void AddSC_boss_void_reaver();
extern void AddSC_instance_the_eye();
extern void AddSC_mob_crystalcore_devastator();
extern void AddSC_mob_the_eye();

//--The Mechanar
//Temple of ahn'qiraj
extern void AddSC_Yauj();
extern void AddSC_Kri();
extern void AddSC_Vem();
extern void AddSC_boss_cthun();

//Terokkar Forest
extern void AddSC_mobs_terokkar_forest();

//Thousand Needles
//Thunder Bluff
extern void AddSC_npc_cairne_bloodhoof();

//Tirisfal Glades
//Uldaman
extern void AddSC_boss_ironaya();
extern void AddSC_mob_jadespine_basilisk();
extern void AddSC_mob_uldaman();

//Undercity
//Un'Goro Crater
//Upper blackrock spire
//Wailing caverns
extern void AddSC_boss_wailing_caverns();

//Western plaguelands
//Westfall
//Wetlands
//Winterspring
extern void AddSC_npcs_winterspring();

//Zangarmarsh
extern void AddSC_npcs_zangarmarsh();

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

    nrscripts = 0;
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

    // -- Custom --
    AddSC_custom_example();
    AddSC_test();

    // -- GO --
    AddSC_go_teleporter();
    AddSC_go_orb_of_command();
    AddSC_go_barov_journal();

    // -- Guard --
    AddSC_guards();

    // -- Honor --
    AddSC_Honor_Vendor();

    // -- Item --
    AddSC_item_test();
    AddSC_area_52_special();
    AddSC_draenei_fishing_net();
    AddSC_nether_wraith_beacon();
    AddSC_purification_mixture();
    AddSC_vorenthals_presence();

    // -- NPC --
    AddSC_not_selectable();
    AddSC_npc_guardian();

    // -- Servers --

    //--------------------
    //------ ZONE --------

    //Alterac Mountains
    //Arathi Highlands
    //Ashenvale Forest
    //Aunchindoun
    //--Auchenai Crypts
    AddSC_boss_shirrak_the_dead_watcher();
    //--Mana Tombs
    AddSC_boss_nexusprince_shaffar();
    AddSC_boss_pandemonius();
    AddSC_boss_tavarok();

    //--Sekketh Halls
    AddSC_boss_darkweaver_syth();
    AddSC_boss_tailonking_ikiss();

    //--Shadow Labyrinth
    AddSC_boss_ambassador_hellmaw();
    AddSC_boss_blackheart_the_inciter();
    AddSC_boss_grandmaster_vorpil();
    AddSC_boss_murmur();
    AddSC_instance_shadow_labyrinth();

    //Azshara
    AddSC_boss_azuregos();
    AddSC_mobs_spitelashes();

    //Azuremyst Isle
    AddSC_injured_draenei();
    AddSC_draenei_survivor();

    //Badlands
    //Barrens
    AddSC_npc_beaten_corpse();

    //Black Temple
    AddSC_mob_blacktemple();
    AddSC_boss_illidan();
    AddSC_boss_shade_of_akama();
    AddSC_boss_supremus();
    AddSC_boss_gurtogg_bloodboil();
    AddSC_boss_mother_shahraz();
    AddSC_boss_reliquary_of_souls();
    AddSC_boss_teron_gorefiend();
    AddSC_boss_najentus();
    AddSC_boss_illidari_council();
    AddSC_instance_black_temple();
 
    //Blackfathom Depths
    //Blackrock Depths
    AddSC_boss_ambassador_flamelash();
    AddSC_boss_angerrel();
    AddSC_boss_anubshiah();
    AddSC_boss_baelgar();
    AddSC_boss_doomrel();
    AddSC_boss_doperel();
    AddSC_boss_draganthaurissan();
    AddSC_boss_fineous_darkvire();
    AddSC_boss_general_angerforge();
    AddSC_boss_gloomrel();
    AddSC_boss_golemlordargelmach();
    AddSC_boss_gorosh_the_dervish();
    AddSC_boss_grizzle();
    AddSC_boss_haterel();
    AddSC_boss_high_interrogator_gerstahn();
    AddSC_boss_lord_incendius();
    AddSC_boss_lord_roccor();
    AddSC_boss_magmus();
    AddSC_boss_moira_bronzebeard();
    AddSC_boss_pyromancer_loregrain();
    AddSC_boss_seethrel();
    AddSC_boss_verek();
    AddSC_boss_vilerel();
    AddSC_boss_warder_stilgiss();
    AddSC_overmaster_pyron();
    AddSC_phalanx();
    AddSC_npc_lokhtos_darkbargainer();

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
    AddSC_boss_pyroguard_emberseer();
    AddSC_boss_gyth();
    AddSC_boss_rend_blackhand();
    AddSC_mob_chromatic_elite_guard();

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

    //Blade's Edge Mountains
    AddSC_mobs_blades_edge_mountains();

    //Blasted lands
    AddSC_boss_kruul();
    AddSC_boss_teremusthedevourer();

    //Burning steppes
    //Caverns of Time
    //--Battle for Mt. Hyjal
    //--Old Hillsbrad
    AddSC_boss_captain_skarloc();
    AddSC_boss_epoch_hunter();
    AddSC_boss_lieutenant_drake();

    //--The Dark Portal
    AddSC_boss_aeonus();
    AddSC_boss_chrono_lord_deja();
    AddSC_boss_temporus();

    //Coilfang Resevoir
    //--Serpent Shrine Cavern
    AddSC_boss_fathomlord_karathress();
    AddSC_boss_hydross_the_unstable();
    AddSC_boss_leotheras_the_blind();
    AddSC_boss_morogrim_tidewalker();
    AddSC_instance_serpentshrine_cavern();
    AddSC_mob_serpentshrine_cavern();

    //--Slave Pens
    AddSC_boss_rokmar_the_crackler();

    //--Steam Vault
    AddSC_boss_warlord_kalithresh();
    AddSC_boss_hydromancer_thespia();
    AddSC_instance_steam_vault();

    //--Underbog
    AddSC_boss_ghazan();
    AddSC_boss_hungarfen();

    //Darkshore
    AddSC_mobs_darkshore();

    //Darnassus
    //Deadmines
    AddSC_boss_deadmines();

    //Deadwind pass
    //Desolace
    //Dire Maul
    //Dun Morogh
    AddSC_npc_narm_faulk();

    //Durotar
    //Duskwood
    //Duskwallow marsh
    AddSC_npc_lady_jaina_proudmoore();

    //Eversong Woods
    AddSC_mobs_mana_tapped();

    //Exodar
    //Eastern Plaguelands
    AddSC_mobs_ghoul_flayer();
    AddSC_npc_darrowshire_spirit();

    //Elwynn Forest
    AddSC_npc_henze_faulk();

    //Felwood
    //Feralas
    //Ghostlands
    //Gnomeregan
    //Gruul's Lair
    AddSC_boss_gruul();
    AddSC_boss_high_king_maulgar();
    AddSC_instance_gruuls_lair();
    AddSC_mob_gruuls_lair();

    //Hellfire Citadel
    //--Blood Furnace
    AddSC_boss_broggok();
    AddSC_boss_kelidan_the_breaker();
    AddSC_boss_the_maker();

    //--Magtheridon's Lair
    AddSC_boss_magtheridon();

    //--Shattered Halls
    AddSC_boss_grand_warlock_nethekurse();

    //--Ramparts
    AddSC_boss_watchkeeper_gargolmar();
    AddSC_boss_omor_the_unscarred();

    //Hellfire Peninsula
    AddSC_boss_doomlordkazzak();
    AddSC_mobs_hellfire_peninsula();
    AddSC_npcs_hellfire_peninsula();

    //Hillsbrad Foothills
    //Hinterlands
    //Ironforge
    //Karazhan
    AddSC_boss_attumen();
    AddSC_boss_curator();
    AddSC_boss_maiden_of_virtue();
    AddSC_boss_shade_of_aran();
    AddSC_boss_malchezaar();
    AddSC_boss_terestian_illhoof();
    AddSC_mob_homunculus();
    AddSC_mob_kilrek();
    AddSC_netherspite_infernal();

    //Loch Modan
    //Lower Blackrock Spire
    //Magtheridon's Lair
    //Maraudon
    AddSC_boss_landslide();
    AddSC_boss_lord_vyletongue();
    AddSC_boss_noxxion();
    AddSC_boss_ptheradras();
    AddSC_boss_rotgrip();
    AddSC_boss_tinkerergizlock();
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
    AddSC_mob_molten_giant();
    AddSC_mob_ancient_core_hound();
    AddSC_mob_molten_destroyer();
    AddSC_mob_firewalker();
    AddSC_mob_flame_guard();
    AddSC_mob_flamewaker();
    AddSC_mob_flamewaker_elite();
    AddSC_mob_flamewaker_healer();
    AddSC_mob_flamewaker_protector();
    AddSC_mob_core_rager();
    AddSC_mob_lavaspawn();

    //Moonglade
    AddSC_silva_filnaveth();
    AddSC_bunthen_plainswind();
    AddSC_npc_great_bear_spirit();

    //Mulgore
    AddSC_skorn_whitecloud();

    //Nagrand
    AddSC_mobs_nagrand();
    AddSC_npcs_nagrand();

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
    AddSC_boss_faerlina();
    AddSC_boss_loatheb();
    AddSC_boss_noth();
    AddSC_boss_gluth();

    //Netherstorm
    AddSC_npcs_netherstorm();

    //Onyxia's Lair
    AddSC_boss_onyxia();

    //Orgrimmar
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
    AddSC_boss_death_knight_darkreaver();
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
    //Shadowmoon Valley
    AddSC_neltharaku();
    AddSC_mobs_shadowmoon_valley();
    AddSC_npcs_shadowmoon_valley();

    //Shattrath
    AddSC_npc_shattrathflaskvendors();

    //Silithus
    AddSC_npcs_captains_blackanvil_and_skullsplit();
    AddSC_npcs_rutgar_and_frankal();

    //Silvermoon
    AddSC_npc_blood_knight_stillblade();

    //Silverpine forest
    //Stockade
    //Stonetalon mountains
    //Stormwind City
    AddSC_npc_bartleby();
    AddSC_npc_dashel_stonefist();

    //Stranglethorn Vale
    AddSC_mobs_stranglethorn_vale();

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
    AddSC_mobs_stratholme();

    //Sunken Temple
    //Tanaris
    AddSC_npcs_tanaris();

    //Teldrassil
    //Tempest Keep
    //--Arcatraz
    //--Botanica
    //--The Eye
    AddSC_boss_kaelthas();
    AddSC_boss_void_reaver();
    AddSC_instance_the_eye();
    AddSC_mob_crystalcore_devastator();
    AddSC_mob_the_eye();

    //--The Mechanar
    //Temple of ahn'qiraj
    AddSC_Yauj();
    AddSC_Kri();
    AddSC_Vem();
    AddSC_boss_cthun();

    //Terokkar Forest
    AddSC_mobs_terokkar_forest();

    //Thousand Needles
    //Thunder Bluff
    AddSC_npc_cairne_bloodhoof();

    //Tirisfal Glades
    //Uldaman
    AddSC_boss_ironaya();
    AddSC_mob_jadespine_basilisk();
    AddSC_mob_uldaman();

    //Undercity
    //Un'Goro Crater
    //Upper blackrock spire
    //Wailing caverns
    AddSC_boss_wailing_caverns();

    //Western plaguelands
    //Westfall
    //Wetlands
    //Winterspring
    AddSC_npcs_winterspring();

    //Zangarmarsh
    AddSC_npcs_zangarmarsh();

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
bool QuestAccept( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestSelect( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestComplete( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestComplete) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool ChooseReward( Player *player, Creature *_Creature, Quest const *_Quest, uint32 opt )
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
bool ItemHello( Player *player, Item *_Item, Quest const *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept( Player *player, Item *_Item, Quest const *_Quest )
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
bool GOQuestAccept( Player *player, GameObject *_GO, Quest const *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(player,_GO,_Quest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward( Player *player, GameObject *_GO, Quest const *_Quest, uint32 opt )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(player,_GO,_Quest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger      ( Player *player, Quest const *_Quest, uint32 triggerID )
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
bool ItemUse( Player *player, Item* _Item, SpellCastTargets const& targets)
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemUse) return false;

    return tmpscript->pItemUse(player,_Item,targets);
}

MANGOS_DLL_EXPORT
bool ReceiveEmote( Player *player, Creature *_Creature, uint32 emote )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pReceiveEmote) return false;

    return tmpscript->pReceiveEmote(player, _Creature, emote);
}

MANGOS_DLL_EXPORT
InstanceData* CreateInstanceData(Map *map)
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(map->GetScript());
    if(!tmpscript || !tmpscript->GetInstanceData) return false;

    return tmpscript->GetInstanceData(map);
}

bool ScriptedAI::IsVisible(Unit* who) const
{
    if (!who)
        return false;

    return m_creature->IsWithinDistInMap(who, VISIBLE_RANGE) && who->isVisibleForOrDetect(m_creature,true);
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
    if( m_creature->isAlive() && m_creature->SelectHostilTarget() && m_creature->getVictim())
    {
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
        m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*victim));
        m_creature->AddThreat(victim, 0.0f);
        m_creature->resetAttackTimer();

        if (victim->GetTypeId() == TYPEID_PLAYER)
            m_creature->SetLootRecipient((Player*)victim);
    }
}

void ScriptedAI::DoMeleeAttackIfReady()
{
    //If we are within range melee the target
    if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
    {
        //Make sure our attack is ready and we arn't currently casting
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
            m_creature->AttackerStateUpdate(m_creature->getVictim());
            m_creature->resetAttackTimer();
        }
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

void ScriptedAI::DoCast(Unit* victim, uint32 spelId, bool triggered)
{
    if (!victim || m_creature->IsNonMeleeSpellCasted(false))
        return;

    m_creature->StopMoving();
    m_creature->CastSpell(victim, spelId, triggered);
}

void ScriptedAI::DoCastSpell(Unit* who,SpellEntry const *spellInfo, bool triggered)
{
    if (!who || m_creature->IsNonMeleeSpellCasted(false))
        return;

    m_creature->StopMoving();
    m_creature->CastSpell(who, spellInfo, triggered);
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
        if( (*m_creature).GetMotionMaster()->top()->GetMovementGeneratorType() == TARGETED_MOTION_TYPE )
            m_creature->GetMotionMaster()->TargetedHome();
    }
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

Creature* ScriptedAI::DoSpawnCreature(uint32 id, float x, float y, float z, float angle, uint32 type, uint32 despawntime)
{
    return m_creature->SummonCreature(id,m_creature->GetPositionX() + x,m_creature->GetPositionY() + y,m_creature->GetPositionZ() + z, angle, (TempSummonType)type, despawntime);
}

Unit* ScriptedAI::SelectUnit(SelectAggroTarget target, uint32 position)
{
    //ThreatList m_threatlist;
    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
    std::list<HostilReference*>::iterator i = m_threatlist.begin();
    std::list<HostilReference*>::reverse_iterator r = m_threatlist.rbegin();

    if (position >= m_threatlist.size() || !m_threatlist.size())
        return NULL;

    switch (target)
    {
    case SELECT_TARGET_RANDOM:
        advance ( i , position +  (rand() % (m_threatlist.size() - position ) ));
        return Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
        break;

    case SELECT_TARGET_TOPAGGRO:
        advance ( i , position);
        return Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
        break;

    case SELECT_TARGET_BOTTOMAGGRO:
        advance ( r , position);
        return Unit::GetUnit((*m_creature),(*r)->getUnitGuid());
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
    if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
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
    if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
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
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_AROUND_CASTER ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_CHANNELED )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_ENEMY-1);

            //Spell targets an enemy
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_CHAIN_DAMAGE ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_CURRENT_SELECTED_ENEMY ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_INSTANT ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_AROUND_CASTER ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_ENEMY_IN_AREA_CHANNELED )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_ENEMY-1);

            //Spell targets a single friend(or self)
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_SELF ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_FRIEND ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_PARTY )
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SINGLE_FRIEND-1);

            //Spell targets aoe friends
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_PARTY_AROUND_CASTER ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_AREAEFFECT_PARTY ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_AROUND_CASTER)
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_FRIEND-1);

            //Spell targets any friend(or self)
            if ( TempSpell->EffectImplicitTargetA[j] == TARGET_SELF ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_FRIEND ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_SINGLE_PARTY ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_PARTY_AROUND_CASTER ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_AREAEFFECT_PARTY ||
                TempSpell->EffectImplicitTargetA[j] == TARGET_ALL_AROUND_CASTER)
                SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_FRIEND-1);

            //Make sure that this spell includes a damage effect
            if ( TempSpell->Effect[j] == SPELL_EFFECT_SCHOOL_DAMAGE || 
                TempSpell->Effect[j] == SPELL_EFFECT_INSTAKILL || 
                TempSpell->Effect[j] == SPELL_EFFECT_ENVIRONMENTAL_DAMAGE || 
                TempSpell->Effect[j] == SPELL_EFFECT_HEALTH_LEECH )
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_DAMAGE-1);

            //Make sure that this spell includes a healing effect (or an apply aura with a periodic heal)
            if ( TempSpell->Effect[j] == SPELL_EFFECT_HEAL || 
                TempSpell->Effect[j] == SPELL_EFFECT_HEAL_MAX_HEALTH || 
                TempSpell->Effect[j] == SPELL_EFFECT_HEAL_MECHANICAL ||
                (TempSpell->Effect[j] == SPELL_EFFECT_APPLY_AURA  && TempSpell->EffectApplyAuraName[j]== 8 ))
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_HEALING-1);

            //Make sure that this spell applies an aura
            if ( TempSpell->Effect[j] == SPELL_EFFECT_APPLY_AURA )
                SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_AURA-1);
        }
    }
}
