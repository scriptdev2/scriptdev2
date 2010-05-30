/*  */

/* AREATRIGGER */
DELETE FROM areatrigger_scripts WHERE entry=4591;
INSERT INTO areatrigger_scripts VALUES (4591,'at_coilfang_waterfall');
DELETE FROM areatrigger_scripts WHERE entry=4560;
INSERT INTO areatrigger_scripts VALUES (4560,'at_legion_teleporter');
DELETE FROM areatrigger_scripts WHERE entry=3066;
INSERT INTO areatrigger_scripts VALUES (3066,'at_ravenholdt');
DELETE FROM areatrigger_scripts WHERE entry IN (4871, 4872, 4873);
INSERT INTO areatrigger_scripts VALUES
(4871,'at_warsong_farms'),
(4872,'at_warsong_farms'),
(4873,'at_warsong_farms');
DELETE FROM areatrigger_scripts WHERE entry BETWEEN 5284 AND 5287;
INSERT INTO areatrigger_scripts VALUES
(5284,'at_aldurthar_gate'),
(5285,'at_aldurthar_gate'),
(5286,'at_aldurthar_gate'),
(5287,'at_aldurthar_gate');
DELETE FROM areatrigger_scripts WHERE entry=4112;
INSERT INTO areatrigger_scripts VALUES (4112,'at_naxxramas');
DELETE FROM areatrigger_scripts WHERE entry=5108;
INSERT INTO areatrigger_scripts VALUES (5108,'at_stormwright_shelf');
DELETE FROM areatrigger_scripts WHERE entry IN (3546, 3547, 3548, 3549, 3550, 3552);
INSERT INTO areatrigger_scripts VALUES
-- Darnassian bank
(3546, 'at_childrens_week_spot'),
-- Undercity - thone room
(3547, 'at_childrens_week_spot'),
-- Stonewrought Dam
(3548, 'at_childrens_week_spot'),
-- The Mor'shan Rampart
(3549, 'at_childrens_week_spot'),
-- Ratchet Docks
(3550, 'at_childrens_week_spot'),
-- Westfall Lighthouse
(3552, 'at_childrens_week_spot');
DELETE FROM areatrigger_scripts WHERE entry IN (2026, 2046);
INSERT INTO areatrigger_scripts VALUES
(2026, 'at_blackrock_spire'),
(2046, 'at_blackrock_spire');

/* BATTLEGROUNDS */
UPDATE creature_template SET ScriptName='npc_spirit_guide' WHERE entry IN (13116, 13117);

/* WORLD BOSS */
UPDATE creature_template SET ScriptName='boss_ysondre' WHERE entry=14887;
UPDATE creature_template SET ScriptName='boss_emeriss' WHERE entry=14889;
UPDATE creature_template SET ScriptName='boss_taerar' WHERE entry=14890;
UPDATE creature_template SET ScriptName='boss_shade_of_taerar' WHERE entry=15302;
UPDATE creature_template SET ScriptName='boss_kruul' WHERE entry=18338;
UPDATE creature_template SET ScriptName='boss_azuregos' WHERE entry=6109;
UPDATE creature_template SET ScriptName='mob_dementeddruids' WHERE entry=15260;

/* GO */
UPDATE gameobject_template SET ScriptName='go_cat_figurine' WHERE entry=13873;
UPDATE gameobject_template SET ScriptName='go_northern_crystal_pylon' WHERE entry=164955;
UPDATE gameobject_template SET ScriptName='go_western_crystal_pylon' WHERE entry=164956;
UPDATE gameobject_template SET ScriptName='go_eastern_crystal_pylon' WHERE entry=164957;
UPDATE gameobject_template SET ScriptName='go_barov_journal' WHERE entry=180794;
UPDATE gameobject_template SET ScriptName='go_ethereum_prison' WHERE entry BETWEEN 184418 AND 184431;
UPDATE gameobject_template SET ScriptName='go_ethereum_stasis' WHERE entry BETWEEN 185465 AND 185467;
UPDATE gameobject_template SET ScriptName='go_ethereum_stasis' WHERE entry=184595;
UPDATE gameobject_template SET ScriptName='go_ethereum_stasis' WHERE entry BETWEEN 185461 AND 185464;
UPDATE gameobject_template SET ScriptName='go_field_repair_bot_74A' where entry=179552;
UPDATE gameobject_template SET ScriptName='go_gilded_brazier' WHERE entry=181956;
UPDATE gameobject_template SET ScriptName='go_jump_a_tron' WHERE entry=183146;
UPDATE gameobject_template SET ScriptName='go_orb_of_command' WHERE entry=179879;
UPDATE gameobject_template SET ScriptName='go_resonite_cask' WHERE entry=178145;
UPDATE gameobject_template SET ScriptName='go_sacred_fire_of_life' WHERE entry=175944;
UPDATE gameobject_template SET ScriptName='go_shrine_of_the_birds' WHERE entry IN (185547,185553,185551);
UPDATE gameobject_template SET ScriptName='go_tablet_of_madness' WHERE entry=180368;
UPDATE gameobject_template SET ScriptName='go_tablet_of_the_seven' WHERE entry=169294;
UPDATE gameobject_template SET ScriptName='go_tele_to_dalaran_crystal' WHERE entry=191230;
UPDATE gameobject_template SET ScriptName='go_tele_to_violet_stand' WHERE entry=191229;
UPDATE gameobject_template SET ScriptName='go_blood_filled_orb' WHERE entry=182024;

/* GUARD */
UPDATE creature_template SET ScriptName='guard_azuremyst' WHERE entry=18038;
UPDATE creature_template SET ScriptName='guard_orgrimmar' WHERE entry=3296;
UPDATE creature_template SET ScriptName='guard_stormwind' WHERE entry IN (68,1976);
UPDATE creature_template SET ScriptName='guard_contested' WHERE entry IN (9460,4624,3502,11190,15184);
UPDATE creature_template SET ScriptName='guard_elwynnforest' WHERE entry=1423;
UPDATE creature_template SET ScriptName='guard_eversong' WHERE entry=16221;
UPDATE creature_template SET ScriptName='guard_darnassus' WHERE entry=4262;
UPDATE creature_template SET ScriptName='guard_teldrassil' WHERE entry=3571;
UPDATE creature_template SET ScriptName='guard_ironforge' WHERE entry=5595;
UPDATE creature_template SET ScriptName='guard_dunmorogh' WHERE entry IN (727,13076);
UPDATE creature_template SET ScriptName='guard_undercity' WHERE entry=5624;
UPDATE creature_template SET ScriptName='guard_bluffwatcher' WHERE entry=3084;
UPDATE creature_template SET ScriptName='guard_durotar' WHERE entry=5953;
UPDATE creature_template SET ScriptName='guard_mulgore' WHERE entry IN (3212,3215,3217,3218,3219,3220,3221,3222,3223,3224);
UPDATE creature_template SET ScriptName='guard_dunmorogh' WHERE entry IN (727,13076);
UPDATE creature_template SET ScriptName='guard_tirisfal' WHERE entry IN (1735,1738,2210,1744,1745,5725,1743,2209,1746,1742);
UPDATE creature_template SET ScriptName='guard_silvermoon' WHERE entry=16222;
UPDATE creature_template SET ScriptName='guard_exodar' WHERE entry=16733;
UPDATE creature_template SET ScriptName='guard_shattrath' WHERE entry=19687;
UPDATE creature_template SET ScriptName='guard_shattrath_aldor' WHERE entry=18549;
UPDATE creature_template SET ScriptName='guard_shattrath_scryer' WHERE entry=18568;

/* ITEM */
UPDATE item_template SET ScriptName='item_arcane_charges' WHERE entry=34475;
UPDATE item_template SET ScriptName='item_flying_machine' WHERE entry IN (34060,34061);
UPDATE item_template SET ScriptName='item_gor_dreks_ointment' WHERE entry=30175;
UPDATE item_template SET ScriptName='item_nether_wraith_beacon' WHERE entry=31742;
UPDATE item_template SET ScriptName='item_tainted_core' WHERE entry=31088;
UPDATE item_template SET ScriptName='item_petrov_cluster_bombs' WHERE entry=33098;

/* NPC (usually creatures to be found in more than one specific zone) */
UPDATE creature_template SET ScriptName='npc_air_force_bots' WHERE entry IN (2614, 2615, 21974, 21993, 21996, 21997, 21999, 22001, 22002, 22003, 22063, 22065, 22066, 22068, 22069, 22070, 22071, 22078, 22079, 22080, 22086, 22087, 22088, 22090, 22124, 22125, 22126);
UPDATE creature_template SET ScriptName='npc_chicken_cluck' WHERE entry=620;
UPDATE creature_template SET ScriptName='npc_dancing_flames' WHERE entry=25305;
UPDATE creature_template SET ScriptName='npc_garments_of_quests' WHERE entry IN (12429,12423,12427,12430,12428);
UPDATE creature_template SET ScriptName='npc_guardian' WHERE entry=5764;
UPDATE creature_template SET ScriptName='npc_kingdom_of_dalaran_quests' WHERE entry IN (29169,23729,26673,27158,29158,29161,26471,29155,29159,29160,29162);
UPDATE creature_template SET ScriptName='npc_lunaclaw_spirit' WHERE entry=12144;
UPDATE creature_template SET ScriptName='npc_mount_vendor' WHERE entry IN (384,1261,1460,2357,3362,3685,4730,4731,4885,7952,7955,16264,17584);
UPDATE creature_template SET ScriptName='npc_doctor' WHERE entry IN (12939,12920);
UPDATE creature_template SET ScriptName='npc_injured_patient' WHERE entry IN (12936,12937,12938,12923,12924,12925);
UPDATE creature_template SET ScriptName='' WHERE npcflag!=npcflag|65536 AND ScriptName='npc_innkeeper';
UPDATE creature_template SET ScriptName='npc_innkeeper' WHERE npcflag=npcflag|65536;
UPDATE creature_template SET ScriptName='npc_prof_alchemy' WHERE entry IN (17909,19052,22427);
UPDATE creature_template SET ScriptName='npc_prof_blacksmith' WHERE entry IN (5164,11145,11146,11176,11177,11178,11191,11192,11193);
UPDATE creature_template SET ScriptName='npc_engineering_tele_trinket' WHERE entry IN (14742,14743,21493,21494);
UPDATE creature_template SET ScriptName='npc_prof_leather' WHERE entry IN (7866,7867,7868,7869,7870,7871);
UPDATE creature_template SET ScriptName='npc_prof_tailor' WHERE entry IN (22208,22212,22213);
UPDATE creature_template SET ScriptName='npc_rogue_trainer' WHERE entry IN (918,4163,3328,4583,5165,5167,13283,16684);
UPDATE creature_template SET ScriptName='npc_sayge' WHERE entry=14822;
UPDATE creature_template SET ScriptName='npc_tabard_vendor' WHERE entry=28776;
UPDATE creature_template SET ScriptName='npc_locksmith' WHERE entry IN (29665,29725,29728);

/* SPELL */
UPDATE creature_template SET ScriptName='spell_dummy_npc' WHERE entry IN (16880,1200,26616,26643,16518,25793,25758,25752,25792,25753,26421,26841,27808,27122,28068,12298,12296,24918,17326,17654,16847,18879,26270,26268,30146);
UPDATE gameobject_template SET ScriptName='spell_dummy_go' WHERE entry IN (181616,186949);

/*  */
/* ZONE */
/* */

/* ALTERAC MOUNTAINS */


/* ALTERAC VALLEY */


/* ARATHI HIGHLANDS */
UPDATE creature_template SET ScriptName='npc_professor_phizzlethorpe' WHERE entry=2768;

/* ASHENVALE */
UPDATE creature_template SET ScriptName='npc_muglash' WHERE entry=12717;
UPDATE gameobject_template SET ScriptName='go_naga_brazier' WHERE entry=178247;
UPDATE creature_template SET ScriptName='npc_ruul_snowhoof' WHERE entry=12818;
UPDATE creature_template SET ScriptName='npc_torek' WHERE entry=12858;

/*  */
/* AUCHINDOUN */
/*  */

/* MANA TOMBS */
UPDATE creature_template SET ScriptName='boss_pandemonius' WHERE entry=18341;
UPDATE creature_template SET ScriptName='boss_nexusprince_shaffar' WHERE entry=18344;
UPDATE creature_template SET ScriptName='mob_ethereal_beacon' WHERE entry=18431;

/* AUCHENAI CRYPTS */
UPDATE creature_template SET ScriptName='boss_exarch_maladaar' WHERE entry=18373;
UPDATE creature_template SET ScriptName='mob_avatar_of_martyred' WHERE entry=18478;
UPDATE creature_template SET ScriptName='mob_stolen_soul' WHERE entry=18441;

/* SETHEKK HALLS */
UPDATE instance_template SET script='instance_sethekk_halls' WHERE map=556;
UPDATE creature_template SET ScriptName='mob_syth_fire' WHERE entry=19203;
UPDATE creature_template SET ScriptName='mob_syth_arcane' WHERE entry=19205;
UPDATE creature_template SET ScriptName='mob_syth_frost' WHERE entry=19204;
UPDATE creature_template SET ScriptName='mob_syth_shadow' WHERE entry=19206;
UPDATE creature_template SET ScriptName='boss_talon_king_ikiss' WHERE entry=18473;
UPDATE creature_template SET ScriptName='boss_darkweaver_syth' WHERE entry=18472;

/* SHADOW LABYRINTH */
UPDATE instance_template SET script='instance_shadow_labyrinth' WHERE map=555;
UPDATE creature_template SET ScriptName='boss_murmur' WHERE entry=18708;
UPDATE creature_template SET ScriptName='boss_grandmaster_vorpil' WHERE entry=18732;
UPDATE creature_template SET ScriptName='boss_blackheart_the_inciter' WHERE entry=18667;
UPDATE creature_template SET ScriptName='boss_ambassador_hellmaw' WHERE entry=18731;

/*  */
/* AZJOL-NERUB */
/*  */

/* AHN'KAHET */
UPDATE creature_template SET ScriptName='boss_jedoga' WHERE entry=29310;
UPDATE creature_template SET ScriptName='boss_nadox' WHERE entry=29309;
UPDATE creature_template SET ScriptName = 'mob_ahnkahar_egg' WHERE entry IN (30172,30173);
UPDATE creature_template SET ScriptName='boss_taldaram' WHERE entry=29308;
UPDATE gameobject_template SET ScriptName='go_nerubian_device' WHERE entry IN (193093,193094);
UPDATE creature_template SET ScriptName='boss_volazj' WHERE entry=29311;
UPDATE instance_template SET script='instance_ahnkahet' WHERE map=619;

/* AZJOL-NERUB */
UPDATE creature_template SET ScriptName='boss_anubarak' WHERE entry=29120;
UPDATE creature_template SET ScriptName='boss_hadronox' WHERE entry=28921;
UPDATE creature_template SET ScriptName='boss_krikthir' WHERE entry=28684;
UPDATE instance_template SET script='instance_azjol-nerub' WHERE map=601;

/* AZSHARA */
UPDATE creature_template SET ScriptName='npc_rizzle_sprysprocket' WHERE entry=23002;
UPDATE creature_template SET ScriptName='npc_depth_charge' WHERE entry=23025;
UPDATE gameobject_template SET ScriptName='go_southfury_moonstone' WHERE entry=185566;
UPDATE creature_template SET ScriptName='mobs_spitelashes' WHERE entry IN (6190,6193,6194,6195,6196,7885,7886,12204,12205);
UPDATE creature_template SET ScriptName='npc_loramus_thalipedes' WHERE entry=7783;

/* AZUREMYST ISLE */
UPDATE creature_template SET ScriptName='npc_draenei_survivor' WHERE entry=16483;
UPDATE creature_template SET ScriptName='npc_engineer_spark_overgrind' WHERE entry=17243;
UPDATE creature_template SET ScriptName='npc_injured_draenei' WHERE entry=16971;
UPDATE creature_template SET ScriptName='npc_magwin' WHERE entry=17312;
UPDATE creature_template SET ScriptName='npc_susurrus' WHERE entry=17435;

/* BADLANDS */


/* BARRENS */
UPDATE creature_template SET ScriptName='npc_beaten_corpse' WHERE entry=10668;
UPDATE creature_template SET ScriptName='npc_gilthares' WHERE entry=3465;
UPDATE creature_template SET ScriptName='npc_sputtervalve' WHERE entry=3442;
UPDATE creature_template SET ScriptName='npc_taskmaster_fizzule' WHERE entry=7233;
UPDATE creature_template SET ScriptName='npc_twiggy_flathead' WHERE entry=6248;
DELETE FROM areatrigger_scripts WHERE entry=522;
INSERT INTO areatrigger_scripts VALUES (522,'at_twiggy_flathead');
UPDATE creature_template SET ScriptName='npc_wizzlecranks_shredder' WHERE entry=3439;

/* BLACK TEMPLE */
UPDATE instance_template SET script='instance_black_temple' WHERE map=564;
UPDATE creature_template SET ScriptName='npc_akama_shade' WHERE entry=22990; -- Akama at Shade of Akama
UPDATE creature_template SET ScriptName='npc_akama_illidan' WHERE entry=23089; -- Akama at Illidan
UPDATE creature_template SET ScriptName='mob_illidari_council' WHERE entry=23426; -- Illidari Council controller mob
UPDATE creature_template SET ScriptName='mob_blood_elf_council_voice_trigger' WHERE entry=23499; -- Voice Trigger Mob (Controls Aggro + Enrage yells)
UPDATE creature_template SET ScriptName='boss_veras_darkshadow' WHERE entry=22952; -- Rogue of Illidari Council
UPDATE creature_template SET ScriptName='boss_teron_gorefiend' WHERE entry=22871; -- Teron Gorefiend
UPDATE creature_template SET ScriptName='boss_supremus' WHERE entry=22898; -- Supremus
UPDATE creature_template SET ScriptName='boss_shade_of_akama' WHERE entry=22841; -- Shade of Akama
UPDATE creature_template SET ScriptName='boss_reliquary_of_souls' WHERE entry=22856; -- Reliquary Controller Mob
UPDATE creature_template SET ScriptName='boss_essence_of_suffering' WHERE entry=23418; -- Essence Of Suffering
UPDATE creature_template SET ScriptName='boss_essence_of_desire' WHERE entry=23419; -- Essence of Desire
UPDATE creature_template SET ScriptName='boss_essence_of_anger' WHERE entry=23420; -- Essence of Anger
UPDATE creature_template SET ScriptName='boss_najentus' WHERE entry=22887; -- High Warlord Naj'entus
UPDATE creature_template SET ScriptName='boss_gurtogg_bloodboil' WHERE entry=22948; -- Gurtogg Bloodboil
UPDATE creature_template SET ScriptName='boss_mother_shahraz' WHERE entry=22947; -- Mother Shahraz
UPDATE creature_template SET ScriptName='boss_lady_malande' WHERE entry=22951; -- Priest <3 at Illidari Council
UPDATE creature_template SET ScriptName='boss_illidan_stormrage' WHERE entry=22917; -- Illidan The Betrayer!
UPDATE creature_template SET ScriptName='boss_high_nethermancer_zerevor' WHERE entry=22950; -- Mage at Illidari Council
UPDATE creature_template SET ScriptName='boss_gathios_the_shatterer' WHERE entry=22949; -- Paladin at Illidari Council
UPDATE creature_template SET ScriptName='boss_maiev_shadowsong' WHERE entry=23197; -- Maiev Shadowsong
UPDATE gameobject_template SET ScriptName='gameobject_cage_trap' WHERE entry=185916; -- Cage Trap GO in Illidan Encounter
UPDATE creature_template SET ScriptName='mob_blaze' WHERE entry=23259; -- Blaze mob in Illidan Phase 2
UPDATE creature_template SET ScriptName='mob_flame_of_azzinoth' WHERE entry=22997; -- Flame of Azzinoth (Illidan Phase 2)
UPDATE creature_template SET ScriptName='mob_blade_of_azzinoth' WHERE entry=22996; -- Blade of Azzinoth (Illidan Phase 2)
UPDATE creature_template SET ScriptName='mob_demon_fire' WHERE entry=23069; -- Demon Fire in Illidan Phase 2
UPDATE creature_template SET ScriptName='mob_flame_crash' WHERE entry=23336; -- Flame Crash in Illidan Normal Form
UPDATE creature_template SET ScriptName='mob_cage_trap_trigger' WHERE entry=23304; -- Cage Trap mob in Illidan Phase 3/4 Normal
UPDATE creature_template SET ScriptName='mob_shadow_demon' WHERE entry=23375; -- Shadow Demon in Illidan Demon Form
UPDATE creature_template SET ScriptName='npc_volcano' WHERE entry=23085; -- Supremus Volcano
UPDATE creature_template SET ScriptName='molten_flame' WHERE entry=23095; -- Molten Flame in SUpremus
UPDATE creature_template SET ScriptName='mob_ashtongue_channeler' WHERE entry=23421; -- Ashtongue CHanneler in Shade of AKama
UPDATE creature_template SET ScriptName='mob_ashtongue_sorcerer' WHERE entry=23215; -- Ashtongue Sorcerer in Shade of Akama
UPDATE creature_template SET ScriptName='npc_enslaved_soul' WHERE entry=23469; -- Enslaved Soul in Reliquary Event
UPDATE creature_template SET ScriptName='mob_doom_blossom' WHERE entry=23123; -- Doom Blossoms in Teron Gorefiend's encounter
UPDATE creature_template SET ScriptName='npc_spirit_of_olum' WHERE entry=23411;
-- UPDATE creature_template SET ScriptName='mob_shadowy_construct' WHERE entry=23111; -- Shadowy Construct in Teron Gorefiend's encounter. Commented until Mind Control is implemented.

/* BLACKFATHOM DEPTHS */
UPDATE instance_template SET script='instance_blackfathom_deeps' WHERE map=48;
UPDATE gameobject_template SET ScriptName='go_fire_of_akumai' WHERE entry IN (21118,21119,21120,21121);

/* BLACKROCK DEPTHS */
DELETE FROM areatrigger_scripts WHERE entry=1526;
INSERT INTO areatrigger_scripts VALUES (1526,'at_ring_of_law');
UPDATE instance_template SET script='instance_blackrock_depths' WHERE map =230;
UPDATE creature_template SET ScriptName='boss_emperor_dagran_thaurissan' WHERE entry=9019;
UPDATE creature_template SET ScriptName='boss_moira_bronzebeard' WHERE entry=8929;
UPDATE creature_template SET ScriptName='boss_ambassador_flamelash' WHERE entry=9156;
UPDATE creature_template SET ScriptName='boss_anubshiah' WHERE entry=9031;
UPDATE creature_template SET ScriptName='boss_doomrel' WHERE entry=9039;
UPDATE creature_template SET ScriptName='boss_gloomrel' WHERE entry=9037;
UPDATE creature_template SET ScriptName='boss_general_angerforge' WHERE entry=9033;
UPDATE creature_template SET ScriptName='boss_gorosh_the_dervish' WHERE entry=9027;
UPDATE creature_template SET ScriptName='boss_grizzle' WHERE entry=9028;
UPDATE creature_template SET ScriptName='boss_high_interrogator_gerstahn' WHERE entry=9018;
UPDATE creature_template SET ScriptName='boss_magmus' WHERE entry=9938;
UPDATE creature_template SET ScriptName='mob_phalanx' WHERE entry=9502;
UPDATE creature_template SET ScriptName='npc_grimstone' WHERE entry=10096;
UPDATE creature_template SET ScriptName='npc_lokhtos_darkbargainer' WHERE entry=12944;
UPDATE creature_template SET ScriptName='npc_kharan_mighthammer' WHERE entry=9021;
UPDATE creature_template SET ScriptName='npc_rocknot' WHERE entry=9503;
UPDATE gameobject_template SET ScriptName='go_shadowforge_brazier' WHERE entry IN (174744, 174745);

/* BLACKROCK SPIRE */
UPDATE instance_template SET script='instance_blackrock_spire' WHERE map=229;
/* BLACKROCK SPIRE Lower bosses */
UPDATE creature_template SET ScriptName='boss_highlord_omokk' WHERE entry=9196;
UPDATE creature_template SET ScriptName='boss_shadow_hunter_voshgajin' WHERE entry=9236;
UPDATE creature_template SET ScriptName='boss_warmaster_voone' WHERE entry=9237;
UPDATE creature_template SET ScriptName='boss_mother_smolderweb' WHERE entry=10596;
UPDATE creature_template SET ScriptName='quartermaster_zigris' WHERE entry=9736;
UPDATE creature_template SET ScriptName='boss_halycon' WHERE entry=10220;
UPDATE creature_template SET ScriptName='boss_overlord_wyrmthalak' WHERE entry=9568;
/* BLACKROCK SPIRE Upper bosses */
UPDATE creature_template SET ScriptName='boss_the_beast' WHERE entry=10430;
UPDATE creature_template SET ScriptName='boss_drakkisath' WHERE entry=10363;
UPDATE creature_template SET ScriptName='boss_gyth' WHERE entry=10339;
UPDATE creature_template SET ScriptName='boss_rend_blackhand' WHERE entry=10429;
UPDATE creature_template SET ScriptName='boss_pyroguard_emberseer' WHERE entry=9816;

/* BLACKWING LAIR */
-- UPDATE instance_template SET script='instance_blackwing_lair' WHERE map=469;
UPDATE creature_template SET ScriptName='boss_razorgore' WHERE entry=12435;
UPDATE creature_template SET ScriptName='boss_vaelastrasz' WHERE entry=13020;
UPDATE creature_template SET ScriptName='boss_broodlord' WHERE entry=12017;
UPDATE creature_template SET ScriptName='boss_firemaw' WHERE entry=11983;
UPDATE creature_template SET ScriptName='boss_ebonroc' WHERE entry=14601;
UPDATE creature_template SET ScriptName='boss_flamegor' WHERE entry=11981;
UPDATE creature_template SET ScriptName='boss_chromaggus' WHERE entry=14020;
UPDATE creature_template SET ScriptName='boss_victor_nefarius' WHERE entry=10162;
UPDATE creature_template SET ScriptName='boss_nefarian' WHERE entry=11583;

/* BLADE'S EDGE MOUNTAINS */
UPDATE creature_template SET ScriptName='mobs_bladespire_ogre' WHERE entry IN (19998,20334,21296,21975);
UPDATE creature_template SET ScriptName='mobs_nether_drake' WHERE entry IN (20021,21817,21820,21821,21823);
UPDATE creature_template SET ScriptName='npc_daranelle' WHERE entry=21469;
UPDATE creature_template SET ScriptName='npc_overseer_nuaar' WHERE entry=21981;
UPDATE creature_template SET ScriptName='npc_saikkal_the_elder' WHERE entry=22932;
UPDATE creature_template SET ScriptName='npc_skyguard_handler_deesak' WHERE entry=23415;

/* BLASTED LANDS */
UPDATE creature_template SET ScriptName='npc_deathly_usher' WHERE entry=8816;
UPDATE creature_template SET ScriptName='npc_fallen_hero_of_horde' WHERE entry=7572;

/* BLOODMYST ISLE */
UPDATE creature_template SET ScriptName='mob_webbed_creature' WHERE entry=17680;
UPDATE creature_template SET ScriptName='npc_captured_sunhawk_agent' WHERE entry=17824;

/* BOREAN TUNDRA */
UPDATE creature_template SET ScriptName='npc_fizzcrank_fullthrottle' WHERE entry=25590;
UPDATE creature_template SET ScriptName='npc_iruk' WHERE entry=26219;
UPDATE creature_template SET ScriptName='npc_kara_thricestar' WHERE entry=26602;
UPDATE creature_template SET ScriptName='npc_surristrasz' WHERE entry=24795;
UPDATE creature_template SET ScriptName='npc_tiare' WHERE entry=30051;
UPDATE creature_template SET ScriptName='npc_lurgglbr' WHERE entry=25208;

/* BURNING STEPPES */
UPDATE creature_template SET ScriptName='npc_ragged_john' WHERE entry=9563;

/*  */
/* CAVERNS OF TIME */
/*  */

/* MT. HYJAL */
UPDATE instance_template SET script='instance_hyjal' WHERE map=534;
UPDATE creature_template SET ScriptName='npc_tyrande_whisperwind' WHERE entry=17948;
UPDATE creature_template SET ScriptName='npc_thrall' WHERE entry=17852;
UPDATE creature_template SET ScriptName='npc_jaina_proudmoore' WHERE entry=17772;
UPDATE creature_template SET ScriptName='boss_archimonde' WHERE entry=17968;
UPDATE creature_template SET ScriptName='mob_doomfire' WHERE entry=18095;
UPDATE creature_template SET ScriptName='mob_doomfire_targetting' WHERE entry=18104;
UPDATE creature_template SET ScriptName='mob_ancient_wisp' WHERE entry=17946;

/* OLD HILLSBRAD */
UPDATE instance_template SET script='instance_old_hillsbrad' WHERE map=560;
UPDATE creature_template SET ScriptName='boss_lieutenant_drake' WHERE entry=17848;
UPDATE creature_template SET ScriptName='boss_epoch_hunter' WHERE entry=18096;
UPDATE creature_template SET ScriptName='boss_captain_skarloc' WHERE entry=17862;
UPDATE gameobject_template SET ScriptName='go_barrel_old_hillsbrad' WHERE entry=182589;
UPDATE creature_template SET ScriptName='npc_brazen' WHERE entry=18725;
UPDATE creature_template SET ScriptName='npc_erozion' WHERE entry=18723;
UPDATE creature_template SET ScriptName='npc_taretha' WHERE entry=18887;
UPDATE creature_template SET ScriptName='npc_thrall_old_hillsbrad' WHERE entry=17876;

/* THE CULLING OF STRATHOLME */
UPDATE instance_template SET script='instance_culling_of_stratholme' WHERE map=595;
UPDATE creature_template SET ScriptName='npc_chromie' WHERE entry IN (26527, 27915);
UPDATE creature_template SET ScriptName='spell_dummy_npc_crates_bunny' WHERE entry=30996;

/* THE DARK PORTAL */
UPDATE creature_template SET ScriptName='boss_chrono_lord_deja' WHERE entry=17879;
UPDATE creature_template SET ScriptName='boss_aeonus' WHERE entry=17881;
UPDATE creature_template SET ScriptName='boss_temporus' WHERE entry=17880;
UPDATE instance_template SET script='instance_dark_portal' WHERE map=269;
UPDATE creature_template SET ScriptName='npc_medivh_bm' WHERE entry=15608;
UPDATE creature_template SET ScriptName='npc_time_rift' WHERE entry=17838;
UPDATE creature_template SET ScriptName='npc_saat' WHERE entry=20201;

/*  */
/* COILFANG RESERVOIR */
/*  */

/* THE SLAVE PENS */

/* THE UNDERBOG */
UPDATE creature_template SET ScriptName='mob_underbog_mushroom' WHERE entry=17990;
UPDATE creature_template SET ScriptName='boss_hungarfen' WHERE entry=17770;

/* THE STEAMVAULT */
UPDATE instance_template SET script='instance_steam_vault' WHERE map=545;
UPDATE creature_template SET ScriptName='boss_hydromancer_thespia' WHERE entry=17797;
UPDATE creature_template SET ScriptName='boss_mekgineer_steamrigger' WHERE entry=17796;
UPDATE creature_template SET ScriptName='boss_warlord_kalithresh' WHERE entry=17798;
UPDATE gameobject_template SET ScriptName='go_main_chambers_access_panel' WHERE entry IN (184125,184126);
UPDATE creature_template SET ScriptName='mob_coilfang_waterelemental' WHERE entry=17917;
UPDATE creature_template SET ScriptName='mob_naga_distiller' WHERE entry=17954;
UPDATE creature_template SET ScriptName='mob_steamrigger_mechanic' WHERE entry=17951;

/* SERPENTSHRINE CAVERN */
UPDATE instance_template SET script='instance_serpent_shrine' WHERE map=548;
UPDATE creature_template SET ScriptName='boss_hydross_the_unstable' WHERE entry=21216;
/* Leotheras the Blind event */
UPDATE creature_template SET ScriptName='boss_leotheras_the_blind' WHERE entry=21215;
UPDATE creature_template SET ScriptName='boss_leotheras_the_blind_demonform' WHERE entry=21875;
/* Fathom-lord Karathress event */
UPDATE creature_template SET ScriptName='boss_fathomlord_karathress' WHERE entry=21214;
UPDATE creature_template SET ScriptName='boss_fathomguard_sharkkis' WHERE entry=21966;
UPDATE creature_template SET ScriptName='boss_fathomguard_tidalvess' WHERE entry=21965;
UPDATE creature_template SET ScriptName='boss_fathomguard_caribdis' WHERE entry=21964;
/* Morogrim Tidewalker event */
UPDATE creature_template SET ScriptName='boss_morogrim_tidewalker' WHERE entry=21213;
UPDATE creature_template SET ScriptName='mob_water_globule' WHERE entry=21913;
/* Lady Vashj event */
UPDATE creature_template SET ScriptName='boss_lady_vashj' WHERE entry=21212;
UPDATE creature_template SET ScriptName='mob_enchanted_elemental' WHERE entry=21958;
UPDATE creature_template SET ScriptName='mob_tainted_elemental' WHERE entry=22009;
UPDATE creature_template SET ScriptName='mob_coilfang_elite' WHERE entry=22055;
UPDATE creature_template SET ScriptName='mob_coilfang_strider' WHERE entry=22056;
UPDATE creature_template SET ScriptName='mob_toxic_sporebat' WHERE entry=22140;
UPDATE creature_template SET ScriptName='mob_shield_generator_channel' WHERE entry=19870;


/* CRYSTALSONG FOREST */

/*  */
/* CRUSADER COLISEUM */
/*  */

/* TRAIL OF THE CHAMPION */

/* TRIAL OF THE CRUSADER */
UPDATE instance_template SET script='instance_trial_of_the_crusader' WHERE map=649;
UPDATE creature_template SET ScriptName='boss_gormok' WHERE entry=34796;
UPDATE creature_template SET ScriptName='boss_acidmaw' WHERE entry=35144;
UPDATE creature_template SET ScriptName='boss_dreadscale' WHERE entry=34799;
UPDATE creature_template SET ScriptName='boss_icehowl' WHERE entry=34797;
UPDATE creature_template SET ScriptName='boss_jaraxxus' WHERE entry=34780;
UPDATE creature_template SET ScriptName='boss_anubarak_trial' WHERE entry=34564;
UPDATE creature_template SET ScriptName='boss_fjola' WHERE entry=34497;
UPDATE creature_template SET ScriptName='boss_eydis' WHERE entry=34496;

/* DALARAN */
UPDATE creature_template SET ScriptName='npc_dalaran_guardian_mage' WHERE entry IN (29255, 29254);
UPDATE creature_template SET ScriptName='npc_zidormi' WHERE entry=31848;

/* DARKSHORE */
UPDATE creature_template SET ScriptName='npc_kerlonian' WHERE entry=11218;
UPDATE creature_template SET ScriptName='npc_prospector_remtravel' WHERE entry=2917;
UPDATE creature_template SET ScriptName='npc_threshwackonator' WHERE entry=6669;

/* DARNASSUS */


/* DEADMINES */
UPDATE instance_template SET script='instance_deadmines' WHERE map=36;
UPDATE gameobject_template SET ScriptName='go_defias_cannon' WHERE entry=16398;
UPDATE gameobject_template SET ScriptName='go_door_lever_dm' WHERE entry=101833;

/* DEADWIND PASS */


/* DESOLACE */
UPDATE creature_template SET ScriptName='npc_aged_dying_ancient_kodo' WHERE entry IN (4700, 4701, 4702, 11627);

/* DIRE MAUL */


/* DRAGONBLIGHT */
UPDATE creature_template SET ScriptName='npc_afrasastrasz' WHERE entry=27575;
UPDATE creature_template SET ScriptName='npc_alexstrasza_wr_gate' WHERE entry=31333;
UPDATE creature_template SET ScriptName='npc_tariolstrasz' WHERE entry=26443;
UPDATE creature_template SET ScriptName='npc_torastrasza' WHERE entry=26949;

/* DRAK'THARON KEEP */
UPDATE creature_template SET ScriptName='boss_novos' WHERE entry=26631;
UPDATE creature_template SET ScriptName='boss_tharonja' WHERE entry=26632;
UPDATE creature_template SET ScriptName='boss_trollgore' WHERE entry=26630;

/* DUN MOROGH */
UPDATE creature_template SET ScriptName='npc_narm_faulk' WHERE entry=6177;


/* DUROTAR */


/* DUSKWOOD */


/* DUSTWALLOW MARSH */
UPDATE creature_template SET ScriptName='mobs_risen_husk_spirit' WHERE entry IN (23554,23555);
UPDATE creature_template SET ScriptName='npc_deserter_agitator' WHERE entry=23602;
UPDATE creature_template SET ScriptName='npc_lady_jaina_proudmoore' WHERE entry=4968;
UPDATE creature_template SET ScriptName='npc_ogron' WHERE entry=4983;
UPDATE creature_template SET ScriptName='npc_morokk' WHERE entry=4500;
UPDATE creature_template SET ScriptName='npc_nat_pagle' WHERE entry=12919;
UPDATE creature_template SET ScriptName='npc_cassa_crimsonwing' WHERE entry=23704;
UPDATE creature_template SET ScriptName='npc_restless_apparition' WHERE entry=23861;
UPDATE creature_template SET ScriptName='npc_private_hendel' WHERE entry=4966;

/* EASTERN PLAGUELANDS */
UPDATE creature_template SET ScriptName='mobs_ghoul_flayer' WHERE entry IN (8530,8531,8532);
UPDATE creature_template SET ScriptName='npc_augustus_the_touched' WHERE entry=12384;
UPDATE creature_template SET ScriptName='npc_darrowshire_spirit' WHERE entry=11064;
UPDATE creature_template SET ScriptName='npc_tirion_fordring' WHERE entry=1855;

/* EBON HOLD */
UPDATE creature_template SET ScriptName='npc_death_knight_initiate' WHERE entry=28406;
UPDATE creature_template SET ScriptName='npc_unworthy_initiate_anchor' WHERE entry=29521;
UPDATE creature_template SET ScriptName='npc_unworthy_initiate' WHERE entry IN (29519,29520,29565,29566,29567);
UPDATE gameobject_template SET ScriptName='go_acherus_soul_prison' WHERE entry IN (191577,191580,191581,191582,191583,191584,191585,191586,191587,191588,191589,191590);
UPDATE creature_template SET ScriptName='npc_a_special_surprise' WHERE entry IN (29032,29061,29065,29067,29068,29070,29074,29072,29073,29071);
UPDATE creature_template SET ScriptName='npc_koltira_deathweaver' WHERE entry=28912;

/* ELWYNN FOREST */
UPDATE creature_template SET ScriptName='npc_henze_faulk' WHERE entry=6172;

/* EVERSONG WOODS */
UPDATE creature_template SET ScriptName='npc_kelerun_bloodmourn' WHERE entry=17807;
UPDATE gameobject_template SET ScriptName='go_harbinger_second_trial' WHERE entry=182052;
UPDATE creature_template SET ScriptName='npc_prospector_anvilward' WHERE entry=15420;
UPDATE creature_template SET ScriptName='npc_apprentice_mirveda' WHERE entry=15402;

/* FELWOOD */
UPDATE creature_template SET ScriptName='npc_kitten' WHERE entry=9937;
UPDATE creature_template SET ScriptName='npc_corrupt_saber' WHERE entry=10042;
UPDATE creature_template SET ScriptName='npcs_riverbreeze_and_silversky' WHERE entry IN (9528,9529);
UPDATE creature_template SET ScriptName='npc_niby_the_almighty' WHERE entry=14469;

/* FERALAS */
UPDATE creature_template SET ScriptName='npc_gregan_brewspewer' WHERE entry=7775;
UPDATE creature_template SET ScriptName='npc_oox22fe' WHERE entry=7807;
UPDATE creature_template SET ScriptName='npc_screecher_spirit' WHERE entry=8612;

/* GHOSTLANDS */
UPDATE creature_template SET ScriptName='npc_blood_knight_dawnstar' WHERE entry=17832;
UPDATE creature_template SET ScriptName='npc_budd_nedreck' WHERE entry=23559;
UPDATE creature_template SET ScriptName='npc_ranger_lilatha' WHERE entry=16295;
UPDATE creature_template SET ScriptName='npc_rathis_tomber' WHERE entry=16224;

/* GNOMEREGAN */


/* GRIZZLY HILLS */


/* GRUUL'S LAIR */
UPDATE instance_template SET script='instance_gruuls_lair' WHERE map =565;
UPDATE creature_template SET ScriptName='boss_gruul' WHERE entry=19044;
/* Maulgar and Event */
UPDATE creature_template SET ScriptName='boss_high_king_maulgar' WHERE entry=18831;
UPDATE creature_template SET ScriptName='boss_kiggler_the_crazed' WHERE entry=18835;
UPDATE creature_template SET ScriptName='boss_blindeye_the_seer' WHERE entry=18836;
UPDATE creature_template SET ScriptName='boss_olm_the_summoner' WHERE entry=18834;
UPDATE creature_template SET ScriptName='boss_krosh_firehand' WHERE entry=18832;

/* GUNDRAK */
UPDATE creature_template SET ScriptName='boss_colossus' WHERE entry=29307;
UPDATE creature_template SET ScriptName='boss_galdarah' WHERE entry=29306;
UPDATE creature_template SET ScriptName='boss_moorabi' WHERE entry=29305;
UPDATE creature_template SET ScriptName='boss_sladran' WHERE entry=29304;
UPDATE creature_template SET ScriptName='mob_sladran_summon_target' WHERE entry=29682;
UPDATE gameobject_template SET ScriptName='go_gundrak_altar' WHERE entry IN (192518, 192519, 192520);
UPDATE instance_template SET script='instance_gundrak' WHERE map=604;

/*  */
/* HELLFIRE CITADEL */
/*  */

/* BLOOD FURNACE */
/* The Maker,Broggok,Kelidan,Broggok's cloud */
UPDATE creature_template SET ScriptName='boss_the_maker' WHERE entry=17381;
UPDATE creature_template SET ScriptName='boss_broggok' WHERE entry=17380;
UPDATE creature_template SET ScriptName='boss_kelidan_the_breaker' WHERE entry=17377;
UPDATE creature_template SET ScriptName='mob_broggok_poisoncloud' WHERE entry=17662;
UPDATE creature_template SET ScriptName='mob_shadowmoon_channeler' WHERE entry=17653;
UPDATE instance_template SET script='instance_blood_furnace' WHERE map=542;

/* HELLFIRE RAMPARTS */
/* Vazruden,Omor the Unscarred,Watchkeeper Gargolmar */
UPDATE creature_template SET ScriptName='boss_omor_the_unscarred' WHERE entry=17308;
UPDATE creature_template SET ScriptName='boss_watchkeeper_gargolmar' WHERE entry=17306;
UPDATE creature_template SET ScriptName='boss_vazruden_herald' WHERE entry=17307;
UPDATE creature_template SET ScriptName='boss_vazruden' WHERE entry=17537;
UPDATE instance_template SET script='instance_ramparts' WHERE map=543;

/* SHATTERED HALLS */
/* Nethekurse and his spawned shadowfissure */
UPDATE creature_template SET ScriptName='boss_grand_warlock_nethekurse' WHERE entry=16807;
UPDATE creature_template SET ScriptName='boss_warbringer_omrogg' WHERE entry=16809;
UPDATE creature_template SET ScriptName='mob_fel_orc_convert' WHERE entry=17083;
UPDATE creature_template SET ScriptName='mob_lesser_shadow_fissure' WHERE entry=17471;
UPDATE creature_template SET ScriptName='mob_omrogg_heads' WHERE entry IN (19523,19524);
UPDATE creature_template SET ScriptName='boss_warchief_kargath_bladefist' WHERE entry=16808;
UPDATE instance_template SET script='instance_shattered_halls' WHERE map=540;

/* MAGTHERIDON'S LAIR */
UPDATE instance_template SET script='instance_magtheridons_lair' WHERE map=544;
UPDATE gameobject_template SET ScriptName='go_manticron_cube' WHERE entry=181713;
UPDATE creature_template SET ScriptName='boss_magtheridon' WHERE entry=17257;
UPDATE creature_template SET ScriptName='mob_abyssal' WHERE entry=17454;
UPDATE creature_template SET ScriptName='mob_hellfire_channeler' WHERE entry=17256;

/* HELLFIRE PENINSULA */
UPDATE creature_template SET ScriptName='boss_doomlord_kazzak' WHERE entry=18728;
UPDATE creature_template SET ScriptName='npc_aeranas' WHERE entry=17085;
UPDATE gameobject_template SET ScriptName='go_haaleshi_altar' WHERE entry=181606;
UPDATE creature_template SET ScriptName='npc_ancestral_wolf' WHERE entry=17077;
UPDATE creature_template SET ScriptName='npc_demoniac_scryer' WHERE entry=22258;
UPDATE creature_template SET ScriptName='npc_gryphoneer_windbellow' WHERE entry=20235;
UPDATE creature_template SET ScriptName='npc_naladu' WHERE entry=19361;
UPDATE creature_template SET ScriptName='npc_tracy_proudwell' WHERE entry=18266;
UPDATE creature_template SET ScriptName='npc_trollbane' WHERE entry=16819;
UPDATE creature_template SET ScriptName='npc_wing_commander_brack' WHERE entry=19401;
UPDATE creature_template SET ScriptName='npc_wing_commander_dabiree' WHERE entry=19409;
UPDATE creature_template SET ScriptName='npc_wounded_blood_elf' WHERE entry=16993;

/* HILLSBRAD FOOTHILLS */


/* HINTERLANDS */
UPDATE creature_template SET ScriptName='npc_00x09hl' WHERE entry=7806;
UPDATE creature_template SET ScriptName='npc_rinji' WHERE entry=7780;

/* HOWLING FJORD */
UPDATE creature_template SET ScriptName='npc_deathstalker_razael' WHERE entry=23998;
UPDATE creature_template SET ScriptName='npc_dark_ranger_lyana' WHERE entry=23778;
UPDATE creature_template SET ScriptName='npc_mcgoyver' WHERE entry=24040;


/* ICECROWN */
UPDATE creature_template SET ScriptName='npc_arete' WHERE entry=29344;
UPDATE creature_template SET ScriptName='npc_dame_evniki_kapsalis' WHERE entry=34885;

/* IRONFORGE */
UPDATE creature_template SET ScriptName='npc_royal_historian_archesonus' WHERE entry=8879;

/* ISLE OF QUEL'DANAS */
UPDATE creature_template SET ScriptName='npc_converted_sentry' WHERE entry=24981;

/* KARAZHAN */
UPDATE instance_template SET script='instance_karazhan' WHERE map=532;
UPDATE creature_template SET ScriptName='boss_midnight' WHERE entry=16151;
UPDATE creature_template SET ScriptName='boss_attumen' WHERE entry=15550;
UPDATE creature_template SET ScriptName='boss_moroes' WHERE entry=15687;
UPDATE creature_template SET ScriptName='boss_maiden_of_virtue' WHERE entry=16457;
UPDATE creature_template SET ScriptName='boss_curator' WHERE entry=15691;
UPDATE creature_template SET ScriptName='boss_julianne' WHERE entry=17534;
UPDATE creature_template SET ScriptName='boss_romulo' WHERE entry=17533;
UPDATE creature_template SET ScriptName='boss_dorothee' WHERE entry=17535;
UPDATE creature_template SET ScriptName='boss_strawman' WHERE entry=17543;
UPDATE creature_template SET ScriptName='boss_tinhead' WHERE entry=17547;
UPDATE creature_template SET ScriptName='mob_tito' WHERE entry=17548;
UPDATE creature_template SET ScriptName='boss_roar' WHERE entry=17546;
UPDATE creature_template SET ScriptName='boss_crone' WHERE entry=18168;
UPDATE creature_template SET ScriptName='boss_terestian_illhoof' WHERE entry=15688;
UPDATE creature_template SET ScriptName='boss_shade_of_aran' WHERE entry=16524;
UPDATE creature_template SET ScriptName='boss_netherspite' WHERE entry=15689;
UPDATE creature_template SET ScriptName='boss_malchezaar' WHERE entry=15690;
-- UPDATE creature_template SET ScriptName='boss_nightbane' WHERE entry=17225;
UPDATE creature_template SET ScriptName='boss_baroness_dorothea_millstipe' WHERE entry=19875;
UPDATE creature_template SET ScriptName='boss_baron_rafe_dreuger' WHERE entry=19874;
UPDATE creature_template SET ScriptName='boss_lady_catriona_von_indi' WHERE entry=19872;
UPDATE creature_template SET ScriptName='boss_lady_keira_berrybuck' WHERE entry=17007;
UPDATE creature_template SET ScriptName='boss_lord_robin_daris' WHERE entry=19876;
UPDATE creature_template SET ScriptName='boss_lord_crispin_ference' WHERE entry=19873;
UPDATE creature_template SET ScriptName='boss_bigbadwolf' WHERE entry=17521;
UPDATE creature_template SET ScriptName='mob_shadow_of_aran' WHERE entry=18254;
UPDATE creature_template SET ScriptName='mob_aran_elemental' WHERE entry=17167;
UPDATE creature_template SET ScriptName='mob_demon_chain' WHERE entry=17248;
UPDATE creature_template SET ScriptName='npc_fiendish_portal' WHERE entry=17265;
UPDATE creature_template SET ScriptName='mob_cyclone' WHERE entry=18412;
UPDATE creature_template SET ScriptName='netherspite_infernal' WHERE entry=17646;
UPDATE creature_template SET ScriptName='npc_berthold' WHERE entry=16153;
UPDATE creature_template SET ScriptName='npc_barnes' WHERE entry=16812;
UPDATE creature_template SET ScriptName='npc_grandmother' WHERE entry=17603;

/* LOCH MODAN */
UPDATE creature_template SET ScriptName='npc_mountaineer_pebblebitty' WHERE entry=3836;
UPDATE creature_template SET ScriptName='npc_miran' WHERE entry=1379;

/* MAGISTER'S TERRACE */
UPDATE instance_template SET script='instance_magisters_terrace' WHERE map=585;
UPDATE creature_template SET ScriptName='boss_selin_fireheart' WHERE entry=24723;
UPDATE creature_template SET ScriptName='mob_fel_crystal' WHERE entry=24722;
UPDATE creature_template SET ScriptName='boss_vexallus' WHERE entry=24744;
UPDATE creature_template SET ScriptName='mob_pure_energy' WHERE entry=24745;
UPDATE creature_template SET ScriptName='boss_priestess_delrissa' WHERE entry=24560;
UPDATE creature_template SET ScriptName='boss_kagani_nightstrike' WHERE entry=24557;
UPDATE creature_template SET ScriptName='boss_ellris_duskhallow' WHERE entry=24558;
UPDATE creature_template SET ScriptName='boss_eramas_brightblaze' WHERE entry=24554;
UPDATE creature_template SET ScriptName='boss_yazzai' WHERE entry=24561;
UPDATE creature_template SET ScriptName='boss_warlord_salaris' WHERE entry=24559;
UPDATE creature_template SET ScriptName='boss_garaxxas' WHERE entry=24555;
-- UPDATE creature_template SET ScriptName='mob_sliver' WHERE entry=24552;
UPDATE creature_template SET ScriptName='boss_apoko' WHERE entry=24553;
UPDATE creature_template SET ScriptName='boss_zelfan' WHERE entry=24556;
UPDATE creature_template SET ScriptName='boss_felblood_kaelthas' WHERE entry=24664;
UPDATE creature_template SET ScriptName='mob_arcane_sphere' WHERE entry=24708;
UPDATE creature_template SET ScriptName='mob_felkael_phoenix' WHERE entry=24674;
UPDATE creature_template SET ScriptName='mob_felkael_phoenix_egg' WHERE entry=24675;
UPDATE creature_template SET ScriptName='npc_kalecgos' WHERE entry IN (24844, 24848);

/* MARAUDON */
UPDATE creature_template SET ScriptName='boss_princess_theradras' WHERE entry=12201;
UPDATE creature_template SET ScriptName='boss_noxxion' WHERE entry=13282;
UPDATE creature_template SET ScriptName='boss_landslide' WHERE entry=12203;
UPDATE creature_template SET ScriptName='celebras_the_cursed' WHERE entry=12225;

/* MOLTEN CORE */
UPDATE instance_template SET script='instance_molten_core' WHERE map=409;
UPDATE creature_template SET ScriptName='boss_lucifron' WHERE entry=12118;
UPDATE creature_template SET ScriptName='boss_magmadar' WHERE entry=11982;
UPDATE creature_template SET ScriptName='boss_gehennas' WHERE entry=12259;
UPDATE creature_template SET ScriptName='boss_garr' WHERE entry=12057;
UPDATE creature_template SET ScriptName='boss_baron_geddon' WHERE entry=12056;
UPDATE creature_template SET ScriptName='boss_shazzrah' WHERE entry=12264;
UPDATE creature_template SET ScriptName='boss_golemagg' WHERE entry=11988;
UPDATE creature_template SET ScriptName='boss_sulfuron' WHERE entry=12098;
UPDATE creature_template SET ScriptName='boss_majordomo' WHERE entry=12018;
UPDATE creature_template SET ScriptName='boss_ragnaros' WHERE entry=11502;
UPDATE creature_template SET ScriptName='mob_ancient_core_hound' WHERE entry=11673;
UPDATE creature_template SET ScriptName='mob_firesworn' WHERE entry=12099;
UPDATE creature_template SET ScriptName='mob_core_rager' WHERE entry=11672;
UPDATE creature_template SET ScriptName='mob_flamewaker_priest' WHERE entry=11662;

/* MOONGLADE */
UPDATE creature_template SET ScriptName='npc_bunthen_plainswind' WHERE entry=11798;
UPDATE creature_template SET ScriptName='npc_clintar_dw_spirit' WHERE entry=22916;
UPDATE creature_template SET ScriptName='npc_great_bear_spirit' WHERE entry=11956;
UPDATE creature_template SET ScriptName='npc_silva_filnaveth' WHERE entry=11800;

/* MULGORE */
UPDATE creature_template SET ScriptName='npc_kyle_the_frenzied' WHERE entry=23616;
UPDATE creature_template SET ScriptName='npc_skorn_whitecloud' WHERE entry=3052;

/* NAGRAND */
UPDATE creature_template SET ScriptName='mob_lump' WHERE entry=18351;
UPDATE creature_template SET ScriptName='mob_shattered_rumbler' WHERE entry=17157;
UPDATE creature_template SET ScriptName='mob_sunspring_villager' WHERE entry=18240;
UPDATE creature_template SET ScriptName='npc_altruis_the_sufferer' WHERE entry=18417;
UPDATE creature_template SET ScriptName='npc_greatmother_geyah' WHERE entry=18141;
UPDATE creature_template SET ScriptName='npc_lantresor_of_the_blade' WHERE entry=18261;
UPDATE creature_template SET ScriptName='npc_maghar_captive' WHERE entry=18210;
UPDATE creature_template SET ScriptName='npc_creditmarker_visit_with_ancestors' WHERE entry IN (18840,18841,18842,18843);

/* NAXXRAMAS */
UPDATE instance_template SET script='instance_naxxramas' WHERE map=533;
UPDATE creature_template SET ScriptName='boss_anubrekhan' WHERE entry=15956;
UPDATE creature_template SET ScriptName='boss_faerlina' WHERE entry=15953;
UPDATE creature_template SET ScriptName='boss_maexxna' WHERE entry=15952;
UPDATE creature_template SET ScriptName='boss_noth' WHERE entry=15954;
UPDATE creature_template SET ScriptName='boss_heigan' WHERE entry=15936;
UPDATE creature_template SET ScriptName='boss_loatheb' WHERE entry=16011;
UPDATE creature_template SET ScriptName='boss_razuvious' WHERE entry=16061;
UPDATE creature_template SET ScriptName='boss_gothik' WHERE entry=16060;
UPDATE creature_template SET ScriptName='spell_anchor' WHERE entry=16137;
UPDATE creature_template SET ScriptName='boss_thane_korthazz' WHERE entry=16064;
UPDATE creature_template SET ScriptName='boss_sir_zeliek' WHERE entry=16063;
UPDATE creature_template SET ScriptName='boss_lady_blaumeux' WHERE entry=16065;
UPDATE creature_template SET ScriptName='boss_rivendare_naxx' WHERE entry=30549;
UPDATE creature_template SET ScriptName='boss_patchwerk' WHERE entry=16028;
-- UPDATE creature_template SET ScriptName='boss_grobbulus' WHERE entry=15931;
UPDATE creature_template SET ScriptName='boss_gluth' WHERE entry=15932;
-- UPDATE creature_template SET ScriptName='boss_thaddius' WHERE entry=15928;
-- UPDATE creature_template SET ScriptName='boss_stalagg' WHERE entry=15929;
-- UPDATE creature_template SET ScriptName='boss_feugen' WHERE entry=15930;
UPDATE creature_template SET ScriptName='boss_sapphiron' WHERE entry=15989;
UPDATE creature_template SET ScriptName='boss_kelthuzad' WHERE entry=15990;

/* NETHERSTORM */
DELETE FROM areatrigger_scripts WHERE entry=4497;
INSERT INTO areatrigger_scripts VALUES (4497,'at_commander_dawnforge');
UPDATE gameobject_template SET ScriptName='go_manaforge_control_console' WHERE entry IN (183770,183956,184311,184312);
UPDATE creature_template SET ScriptName='npc_manaforge_control_console' WHERE entry IN (20209,20417,20418,20440);
UPDATE creature_template SET ScriptName='npc_commander_dawnforge' WHERE entry=19831;
UPDATE creature_template SET ScriptName='npc_protectorate_nether_drake' WHERE entry=20903;
UPDATE creature_template SET ScriptName='npc_veronia' WHERE entry=20162;

/*  */
/* THE NEXUS */
/*  */

/* EYE OF ETERNITY */

/* NEXUS */
UPDATE creature_template SET ScriptName='boss_anomalus' WHERE entry=26763;
UPDATE creature_template SET ScriptName='mob_chaotic_rift' WHERE entry=26918;
UPDATE creature_template SET ScriptName='boss_keristrasza' WHERE entry=26723;
UPDATE creature_template SET ScriptName='boss_ormorok' WHERE entry=26794;
UPDATE creature_template SET ScriptName='boss_telestra' WHERE entry=26731;
UPDATE gameobject_template SET ScriptName='go_containment_sphere' WHERE entry IN (188526, 188527, 188528);
UPDATE instance_template SET script='instance_nexus' WHERE map=576;

/* OCULUS */


/* OBSIDIAN SANCTUM */
UPDATE instance_template SET script='instance_obsidian_sanctum' WHERE map=615;
UPDATE creature_template SET ScriptName='boss_sartharion' WHERE entry=28860;
UPDATE creature_template SET ScriptName='mob_vesperon' WHERE entry=30449;
UPDATE creature_template SET ScriptName='mob_shadron' WHERE entry=30451;
UPDATE creature_template SET ScriptName='mob_tenebron' WHERE entry=30452;
UPDATE creature_template SET ScriptName='mob_twilight_eggs' WHERE entry=30882;
UPDATE creature_template SET ScriptName='mob_twilight_whelp' WHERE entry IN (30890, 31214);
UPDATE creature_template SET ScriptName='mob_acolyte_of_shadron' WHERE entry=31218;
UPDATE creature_template SET ScriptName='mob_acolyte_of_vesperon' WHERE entry=31219;

/* ONYXIA'S LAIR */
UPDATE creature_template SET ScriptName='boss_onyxia' WHERE entry=10184;

/* ORGRIMMAR */
UPDATE creature_template SET ScriptName='npc_neeru_fireblade' WHERE entry=3216;
UPDATE creature_template SET ScriptName='npc_shenthul' WHERE entry=3401;
UPDATE creature_template SET ScriptName='npc_thrall_warchief' WHERE entry=4949;

/* RAGEFIRE CHASM */


/* RAZORFEN DOWNS */
UPDATE creature_template SET ScriptName='boss_amnennar_the_coldbringer' WHERE entry=7358;
UPDATE creature_template SET ScriptName='npc_henry_stern' WHERE entry=8696;

/* RAZORFEN KRAUL */
UPDATE instance_template SET script='instance_razorfen_kraul' WHERE map=47;

/* REDRIDGE MOUNTAINS */
UPDATE creature_template SET ScriptName='npc_corporal_keeshan' WHERE entry=349;

/* RUINS OF AHN'QIRAJ */
UPDATE instance_template SET script='instance_ruins_of_ahnqiraj' WHERE map=509;
UPDATE creature_template SET ScriptName='mob_anubisath_guardian' WHERE entry=15355;
UPDATE creature_template SET ScriptName='boss_kurinnaxx' WHERE entry=15348;
UPDATE creature_template SET ScriptName='boss_ayamiss' WHERE entry=15369;
UPDATE creature_template SET ScriptName='boss_moam' WHERE entry=15340;

/* SCARLET MONASTERY */
UPDATE instance_template SET script='instance_scarlet_monastery' WHERE map=189;
UPDATE creature_template SET ScriptName='boss_arcanist_doan' WHERE entry=6487;
UPDATE creature_template SET ScriptName='boss_azshir_the_sleepless' WHERE entry=6490;
UPDATE creature_template SET ScriptName='boss_bloodmage_thalnos' WHERE entry=4543;
UPDATE creature_template SET ScriptName='boss_herod' WHERE entry=3975;
UPDATE creature_template SET ScriptName='boss_high_inquisitor_fairbanks' WHERE entry=4542;
UPDATE creature_template SET ScriptName='boss_high_inquisitor_whitemane' WHERE entry=3977;
UPDATE creature_template SET ScriptName='boss_houndmaster_loksey' WHERE entry=3974;
UPDATE creature_template SET ScriptName='boss_interrogator_vishas' WHERE entry=3983;
UPDATE creature_template SET ScriptName='boss_scarlet_commander_mograine' WHERE entry=3976;
UPDATE creature_template SET ScriptName='boss_scorn' WHERE entry=14693;
UPDATE creature_template SET ScriptName='mob_scarlet_trainee' WHERE entry=6575;
UPDATE creature_template SET ScriptName='boss_headless_horseman' WHERE entry=23682;

/* SCHOLOMANCE */
UPDATE instance_template SET script='instance_scholomance' WHERE map=289;
UPDATE creature_template SET ScriptName='boss_darkmaster_gandling' WHERE entry=1853;
UPDATE creature_template SET ScriptName='boss_death_knight_darkreaver' WHERE entry=14516;
UPDATE creature_template SET ScriptName='boss_lord_alexei_barov' WHERE entry=10504;
UPDATE creature_template SET ScriptName='boss_instructor_malicia' WHERE entry=10505;
UPDATE creature_template SET ScriptName='boss_boss_ras_frostwhisper' WHERE entry=10508;
UPDATE creature_template SET ScriptName='boss_the_ravenian' WHERE entry=10507;
UPDATE creature_template SET ScriptName='boss_vectus' WHERE entry=10432;
UPDATE creature_template SET ScriptName='boss_illucia_barov' WHERE entry=10502;
UPDATE creature_template SET ScriptName='boss_doctor_theolen_krastinov' WHERE entry=11261;
UPDATE creature_template SET ScriptName='boss_jandice_barov' WHERE entry=10503;
UPDATE creature_template SET ScriptName='boss_lorekeeper_polkelt' WHERE entry=10901;
UPDATE creature_template SET ScriptName='boss_kormok' WHERE entry=16118;
UPDATE creature_template SET ScriptName='mob_illusionofjandicebarov' WHERE entry=11439;

/* SEARING GORGE */
UPDATE creature_template SET ScriptName='npc_kalaran_windblade' WHERE entry=8479;
UPDATE creature_template SET ScriptName='npc_lothos_riftwaker' WHERE entry=14387;
UPDATE creature_template SET ScriptName='npc_zamael_lunthistle' WHERE entry=8436;

/* SHADOWFANG KEEP */
UPDATE instance_template SET script='instance_shadowfang_keep' WHERE map=33;
UPDATE creature_template SET ScriptName='npc_shadowfang_prisoner' WHERE entry IN (3849,3850);
UPDATE creature_template SET ScriptName='npc_arugal' WHERE entry=10000;
UPDATE creature_template SET ScriptName='npc_deathstalker_vincent' WHERE entry=4444;
UPDATE creature_template SET ScriptName='mob_arugal_voidwalker' WHERE entry=4627;
UPDATE creature_template SET ScriptName='boss_arugal' WHERE entry=4275;

/* SHADOWMOON VALLEY */
UPDATE creature_template SET ScriptName='boss_doomwalker' WHERE entry=17711;
UPDATE creature_template SET ScriptName='npc_drake_dealer_hurlunk' WHERE entry=23489;
UPDATE creature_template SET ScriptName='npcs_flanis_swiftwing_and_kagrosh' WHERE entry IN (21725,21727);
UPDATE creature_template SET ScriptName='npc_murkblood_overseer' WHERE entry=23309;
UPDATE creature_template SET ScriptName='npc_neltharaku' WHERE entry=21657;
UPDATE creature_template SET ScriptName='npc_oronok_tornheart' WHERE entry=21183;
UPDATE creature_template SET ScriptName='mob_mature_netherwing_drake' WHERE entry=21648;
UPDATE creature_template SET ScriptName='mob_enslaved_netherwing_drake' WHERE entry=21722;
UPDATE creature_template SET ScriptName='npc_karynaku' WHERE entry=22112;
UPDATE creature_template SET ScriptName='npc_wilda' WHERE entry=21027;
UPDATE creature_template SET ScriptName='mob_torloth' WHERE entry=22076;
UPDATE creature_template SET ScriptName='npc_lord_illidan_stormrage' WHERE entry=22083;
UPDATE gameobject_template SET ScriptName='go_crystal_prison' WHERE entry=185126;

/* SHATTRATH */
UPDATE creature_template SET ScriptName='npc_dirty_larry' WHERE entry=19720;
UPDATE creature_template SET ScriptName='npc_ishanah' WHERE entry=18538;
UPDATE creature_template SET ScriptName='npc_khadgar' WHERE entry=18166;
UPDATE creature_template SET ScriptName='npc_khadgars_servant' WHERE entry=19685;
UPDATE creature_template SET ScriptName='npc_raliq_the_drunk' WHERE entry=18585;
UPDATE creature_template SET ScriptName='npc_salsalabim' WHERE entry=18584;
UPDATE creature_template SET ScriptName='npc_shattrathflaskvendors' WHERE entry IN (23483,23484);
UPDATE creature_template SET ScriptName='npc_zephyr' WHERE entry=25967;

/* SHOLAZAR BASIN */
UPDATE creature_template SET ScriptName='npc_vekjik' WHERE entry=28315;

/* SILITHUS */
UPDATE creature_template SET ScriptName='npc_highlord_demitrian' WHERE entry=14347;
UPDATE creature_template SET ScriptName='npcs_rutgar_and_frankal' WHERE entry IN (15170,15171);

/* SILVERMOON */
UPDATE creature_template SET ScriptName='npc_blood_knight_stillblade' WHERE entry=17768;

/* SILVERPINE FOREST */
UPDATE creature_template SET ScriptName='npc_astor_hadren' WHERE entry=6497;
UPDATE creature_template SET ScriptName='npc_deathstalker_erland' WHERE entry=1978;
UPDATE creature_template SET ScriptName='npc_deathstalker_faerleia' WHERE entry=2058;

/* STOCKADES */

/* STONETALON MOUNTAINS */
UPDATE creature_template SET ScriptName='npc_braug_dimspirit' WHERE entry=4489;
UPDATE creature_template SET ScriptName='npc_kaya' WHERE entry=11856;

/* STORM PEAKS */
UPDATE creature_template SET ScriptName='npc_loklira_the_crone' WHERE entry=29975;
UPDATE creature_template SET ScriptName='npc_thorim' WHERE entry=29445;
UPDATE creature_template SET ScriptName='npc_roxi_ramrocket' WHERE entry=31247;
UPDATE creature_template SET ScriptName = 'npc_frostborn_scout' WHERE entry = 29811;

/* STORMWIND CITY */
UPDATE creature_template SET ScriptName='npc_archmage_malin' WHERE entry=2708;
UPDATE creature_template SET ScriptName='npc_bartleby' WHERE entry=6090;
UPDATE creature_template SET ScriptName='npc_dashel_stonefist' WHERE entry=4961;
UPDATE creature_template SET ScriptName='npc_lady_katrana_prestor' WHERE entry=1749;

/* STRANGLETHORN VALE */
UPDATE creature_template SET ScriptName='mob_yenniku' WHERE entry=2530;

/* STRATHOLME */
UPDATE instance_template SET script='instance_stratholme' WHERE map=329;
UPDATE creature_template SET ScriptName='boss_dathrohan_balnazzar' WHERE entry=10812;
UPDATE creature_template SET ScriptName='boss_magistrate_barthilas' WHERE entry=10435;
UPDATE creature_template SET ScriptName='boss_maleki_the_pallid' WHERE entry=10438;
UPDATE creature_template SET ScriptName='boss_nerubenkan' WHERE entry=10437;
UPDATE creature_template SET ScriptName='boss_cannon_master_willey' WHERE entry=10997;
UPDATE creature_template SET ScriptName='boss_baroness_anastari' WHERE entry=10436;
UPDATE creature_template SET ScriptName='boss_ramstein_the_gorger' WHERE entry=10439;
UPDATE creature_template SET ScriptName='boss_timmy_the_cruel' WHERE entry=10808;
UPDATE creature_template SET ScriptName='boss_silver_hand_bosses' WHERE entry IN (17910,17911,17912,17913,17914);
UPDATE creature_template SET ScriptName='boss_postmaster_malown' WHERE entry=11143;
UPDATE creature_template SET ScriptName='boss_baron_rivendare' WHERE entry=10440;
UPDATE creature_template SET ScriptName='mobs_spectral_ghostly_citizen' WHERE entry IN (10384,10385);
UPDATE creature_template SET ScriptName='mob_restless_soul' WHERE entry=11122;
UPDATE creature_template SET ScriptName='mob_freed_soul' WHERE entry=11136;
UPDATE gameobject_template SET ScriptName='go_gauntlet_gate' WHERE entry=175357;

/* SUNKEN TEMPLE */
UPDATE instance_template SET script='instance_sunken_temple' WHERE map=109;
DELETE FROM areatrigger_scripts WHERE entry=4016;
INSERT INTO areatrigger_scripts VALUES (4016,'at_shade_of_eranikus');
UPDATE creature_template SET ScriptName='npc_malfurion_stormrage' WHERE entry=15362;

/* SUNWELL PLATEAU */
UPDATE instance_template SET script='instance_sunwell_plateau' WHERE map=580;
UPDATE creature_template SET ScriptName='boss_brutallus' WHERE entry=24882;
UPDATE creature_template SET ScriptName='boss_kalecgos' WHERE entry=24850;
UPDATE creature_template SET ScriptName='boss_kalecgos_humanoid' WHERE entry=24891;
UPDATE creature_template SET ScriptName='boss_sathrovarr' WHERE entry=24892;
UPDATE gameobject_template SET ScriptName='go_spectral_rift' WHERE entry=187055;
DELETE FROM areatrigger_scripts WHERE entry=4853;
INSERT INTO areatrigger_scripts VALUES (4853,'at_madrigosa');

/* SWAMP OF SORROWS */


/* TANARIS */
UPDATE creature_template SET ScriptName='mob_aquementas' WHERE entry=9453;
UPDATE creature_template SET ScriptName='npc_custodian_of_time' WHERE entry=20129;
UPDATE creature_template SET ScriptName='npc_marin_noggenfogger' WHERE entry=7564;
UPDATE creature_template SET ScriptName='npc_oox17tn' WHERE entry=7784;
UPDATE creature_template SET ScriptName='npc_steward_of_time' WHERE entry=20142;
UPDATE creature_template SET ScriptName='npc_stone_watcher_of_norgannon' WHERE entry=7918;
UPDATE creature_template SET ScriptName='npc_tooga' WHERE entry=5955;

/* TELDRASSIL */
UPDATE creature_template SET ScriptName='npc_mist' WHERE entry=3568;

/*  */
/* TEMPEST KEEP */
/*  */

/* THE MECHANAR */
UPDATE creature_template SET ScriptName='boss_gatewatcher_iron_hand' WHERE entry=19710;
UPDATE creature_template SET ScriptName='boss_nethermancer_sepethrea' WHERE entry=19221;
UPDATE creature_template SET ScriptName='mob_ragin_flames' WHERE entry=20481;
UPDATE creature_template SET ScriptName='boss_pathaleon_the_calculator' WHERE entry=19220;
UPDATE creature_template SET ScriptName='mob_nether_wraith' WHERE entry=21062;
UPDATE instance_template SET script='instance_mechanar' WHERE map=554;

/* THE BOTANICA */
UPDATE creature_template SET ScriptName='boss_high_botanist_freywinn' WHERE entry=17975;
UPDATE creature_template SET ScriptName='boss_laj' WHERE entry=17980;
UPDATE creature_template SET ScriptName='boss_warp_splinter' WHERE entry=17977;
UPDATE creature_template SET ScriptName='mob_warp_splinter_treant' WHERE entry=19949;

/* THE ARCATRAZ */
UPDATE instance_template SET script='instance_arcatraz' WHERE map=552;
UPDATE creature_template SET ScriptName='mob_zerekethvoidzone' WHERE entry=21101;
UPDATE creature_template SET ScriptName='boss_harbinger_skyriss' WHERE entry=20912;
UPDATE creature_template SET ScriptName='boss_harbinger_skyriss_illusion' WHERE entry IN (21466,21467);
UPDATE creature_template SET ScriptName='npc_warden_mellichar' WHERE entry=20904;
UPDATE creature_template SET ScriptName='npc_millhouse_manastorm' WHERE entry=20977;

/* THE EYE */
UPDATE instance_template SET script='instance_the_eye' WHERE map=550;
/* The Eye Trash Mobs */
UPDATE creature_template SET ScriptName='mob_crystalcore_devastator' WHERE entry=20040;
/* Void Reaver event */
UPDATE creature_template SET ScriptName='boss_void_reaver' WHERE entry=19516;
/* Astromancer event */
UPDATE creature_template SET ScriptName='boss_high_astromancer_solarian' WHERE entry=18805;
UPDATE creature_template SET ScriptName='mob_solarium_priest' WHERE entry=18806;
/* Kael'thas event */
UPDATE creature_template SET ScriptName='boss_kaelthas' WHERE entry=19622;
UPDATE creature_template SET ScriptName='boss_thaladred_the_darkener' WHERE entry=20064;
UPDATE creature_template SET ScriptName='boss_lord_sanguinar' WHERE entry=20060;
UPDATE creature_template SET ScriptName='boss_grand_astromancer_capernian' WHERE entry=20062;
UPDATE creature_template SET ScriptName='boss_master_engineer_telonicus' WHERE entry=20063;
UPDATE creature_template SET ScriptName='mob_phoenix_tk' WHERE entry=21362;
UPDATE creature_template SET ScriptName='mob_phoenix_egg_tk' WHERE entry=21364;

/* TEMPLE OF AHN'QIRAJ */
UPDATE instance_template SET script='instance_temple_of_ahnqiraj' WHERE map=531;
UPDATE creature_template SET ScriptName='boss_cthun' WHERE entry=15727;
UPDATE creature_template SET ScriptName='boss_skeram' WHERE entry=15263;
UPDATE creature_template SET ScriptName='boss_vem' WHERE entry=15544;
UPDATE creature_template SET ScriptName='boss_yauj' WHERE entry=15543;
UPDATE creature_template SET ScriptName='boss_kri' WHERE entry=15511;
UPDATE creature_template SET ScriptName='boss_sartura' WHERE entry=15516;
UPDATE creature_template SET ScriptName='boss_fankriss' WHERE entry=15510;
-- UPDATE creature_template SET ScriptName='boss_viscidus' WHERE entry=15299;
-- UPDATE creature_template SET ScriptName='boss_glob_of_viscidus' WHERE entry=15667;
UPDATE creature_template SET ScriptName='boss_huhuran' WHERE entry=15509;
UPDATE creature_template SET ScriptName='boss_veklor' WHERE entry=15276;
UPDATE creature_template SET ScriptName='boss_veknilash' WHERE entry=15275;
UPDATE creature_template SET ScriptName='boss_ouro' WHERE entry=15517;
UPDATE creature_template SET ScriptName='boss_eye_of_cthun' WHERE entry=15589;
UPDATE creature_template SET ScriptName='mob_sartura_royal_guard' WHERE entry=15984;
UPDATE creature_template SET ScriptName='mob_claw_tentacle' WHERE entry=15725;
UPDATE creature_template SET ScriptName='mob_eye_tentacle' WHERE entry=15726;
UPDATE creature_template SET ScriptName='mob_giant_claw_tentacle' WHERE entry=15728;
UPDATE creature_template SET ScriptName='mob_giant_eye_tentacle' WHERE entry=15334;
UPDATE creature_template SET ScriptName='mob_giant_flesh_tentacle' WHERE entry=15802;
UPDATE creature_template SET ScriptName='mob_anubisath_sentinel' WHERE entry=15264;

/* TEROKKAR FOREST */
UPDATE creature_template SET ScriptName='mob_infested_root_walker' WHERE entry=22095;
UPDATE creature_template SET ScriptName='mob_netherweb_victim' WHERE entry=22355;
UPDATE creature_template SET ScriptName='mob_rotting_forest_rager' WHERE entry=22307;
UPDATE creature_template SET ScriptName='mob_unkor_the_ruthless' WHERE entry=18262;
UPDATE creature_template SET ScriptName='npc_akuno' WHERE entry=22377;
UPDATE creature_template SET ScriptName='npc_floon' WHERE entry=18588;
UPDATE creature_template SET ScriptName='npc_letoll' WHERE entry=22458;
UPDATE creature_template SET ScriptName='npc_mana_bomb_exp_trigger' WHERE entry=20767;
UPDATE gameobject_template SET ScriptName='go_mana_bomb' WHERE entry=184725;
UPDATE creature_template SET ScriptName='npc_skyguard_handler_irena' WHERE entry=23413;
UPDATE creature_template SET ScriptName='npc_slim' WHERE entry=19679;

/* THOUSAND NEEDLES */
UPDATE creature_template SET ScriptName='npc_kanati' WHERE entry=10638;
UPDATE creature_template SET ScriptName='npc_plucky_johnson' WHERE entry=6626;
UPDATE creature_template SET ScriptName='npc_paoka_swiftmountain' WHERE entry=10427;
UPDATE creature_template SET ScriptName='npc_lakota_windsong' WHERE entry=10646;

/* THUNDER BLUFF */
UPDATE creature_template SET ScriptName='npc_cairne_bloodhoof' WHERE entry=3057;

/* TIRISFAL GLADES */
UPDATE gameobject_template SET ScriptName='go_mausoleum_trigger' WHERE entry=104593;
UPDATE gameobject_template SET ScriptName='go_mausoleum_door' WHERE entry=176594;
UPDATE creature_template SET ScriptName='npc_calvin_montague' WHERE entry=6784;

/* ULDAMAN */
UPDATE creature_template SET ScriptName='boss_ironaya' WHERE entry=7228;
UPDATE creature_template SET ScriptName='mob_jadespine_basilisk' WHERE entry=4863;
UPDATE creature_template SET ScriptName='npc_lore_keeper_of_norgannon' WHERE entry=7172;
UPDATE gameobject_template SET ScriptName='go_altar_of_keepers' WHERE entry=130511;
UPDATE instance_template SET script='instance_uldaman' WHERE map=70;

/*  */
/* ULDUAR */
/*  */

/* HALLS OF LIGHTNING */
UPDATE instance_template SET script='instance_halls_of_lightning' WHERE map=602;
UPDATE creature_template SET ScriptName='boss_bjarngrim' WHERE entry=28586;
UPDATE creature_template SET ScriptName='mob_stormforged_lieutenant' WHERE entry=29240;
UPDATE creature_template SET ScriptName='boss_volkhan' WHERE entry=28587;
UPDATE creature_template SET ScriptName='mob_molten_golem' WHERE entry=28695;
UPDATE creature_template SET ScriptName='npc_volkhan_anvil' WHERE entry=28823;
UPDATE creature_template SET ScriptName='boss_ionar' WHERE entry=28546;
UPDATE creature_template SET ScriptName='mob_spark_of_ionar' WHERE entry=28926;
UPDATE creature_template SET ScriptName='boss_loken' WHERE entry=28923;

/* HALLS OF STONE */
UPDATE instance_template SET script='instance_halls_of_stone' WHERE map=599;
UPDATE creature_template SET ScriptName='boss_maiden_of_grief' WHERE entry=27975;
UPDATE creature_template SET ScriptName='boss_sjonnir' WHERE entry=27978;
UPDATE creature_template SET ScriptName='npc_brann_hos' WHERE entry=28070;

/* ULDUAR */
UPDATE instance_template SET script='instance_ulduar' WHERE map=603;

/* UN'GORO CRATER */
UPDATE creature_template SET ScriptName='npc_ame01' WHERE entry=9623;
UPDATE creature_template SET ScriptName='npc_ringo' WHERE entry=9999;

/* UNDERCITY */
UPDATE creature_template SET ScriptName='npc_lady_sylvanas_windrunner' WHERE entry=10181;
UPDATE creature_template SET ScriptName='npc_highborne_lamenter' WHERE entry=21628;
UPDATE creature_template SET ScriptName='npc_parqual_fintallas' WHERE entry=4488;

/*  */
/* UTGARDE KEEP */
/*  */

/* UTGARDE KEEP */
UPDATE instance_template SET script='instance_utgarde_keep' WHERE map=574;
UPDATE creature_template SET ScriptName='mob_dragonflayer_forge_master' WHERE entry=24079;
UPDATE creature_template SET ScriptName='boss_skarvald' WHERE entry=24200;
UPDATE creature_template SET ScriptName='boss_dalronn' WHERE entry=24201;
UPDATE creature_template SET ScriptName='boss_ingvar' WHERE entry=23954;
UPDATE creature_template SET ScriptName='npc_annhylde' WHERE entry=24068;
UPDATE creature_template SET ScriptName='boss_keleseth' WHERE entry=23953;
UPDATE creature_template SET ScriptName='mob_vrykul_skeleton' WHERE entry=23970;

/* UTGARDE PINNACLE */
UPDATE creature_template SET ScriptName='boss_gortok' WHERE entry=26687;
DELETE FROM areatrigger_scripts WHERE entry=4991;
INSERT INTO areatrigger_scripts VALUES (4991,'at_skadi');
UPDATE creature_template SET ScriptName='boss_skadi' WHERE entry=26693;
UPDATE creature_template SET ScriptName='boss_svala' WHERE entry=29281;
DELETE FROM areatrigger_scripts WHERE entry=5140;
INSERT INTO areatrigger_scripts VALUES (5140,'at_svala_intro');
UPDATE creature_template SET ScriptName='boss_ymiron' WHERE entry=26861;
UPDATE instance_template SET script='instance_pinnacle' WHERE map=575;

/* VAULT OF ARCHAVON */


/* VIOLET HOLD */
UPDATE instance_template SET script='instance_violet_hold' WHERE map=608;
UPDATE gameobject_template SET ScriptName='go_activation_crystal' WHERE entry=193611;
UPDATE creature_template SET ScriptName='npc_door_seal' WHERE entry=30896;
UPDATE creature_template SET ScriptName='npc_sinclari' WHERE entry=30658;
UPDATE creature_template SET ScriptName='npc_teleportation_portal' WHERE entry IN (31011,30679,32174);

/* WAILING CAVERNS */
UPDATE instance_template SET script='instance_wailing_caverns' WHERE map=43;

/* WESTERN PLAGUELANDS */
UPDATE creature_template SET ScriptName='npcs_dithers_and_arbington' WHERE entry IN (11056,11057);
UPDATE creature_template SET ScriptName='npc_myranda_the_hag' WHERE entry=11872;
UPDATE creature_template SET ScriptName='npc_the_scourge_cauldron' WHERE entry=11152;

/* WESTFALL */
UPDATE creature_template SET ScriptName='npc_daphne_stilwell' WHERE entry=6182;
UPDATE creature_template SET ScriptName='npc_defias_traitor' WHERE entry=467;

/* WETLANDS */
UPDATE creature_template SET ScriptName='npc_tapoke_slim_jahn' WHERE entry=4962;
UPDATE creature_template SET ScriptName='npc_mikhail' WHERE entry=4963;

/* WINTERSPRING */
UPDATE creature_template SET ScriptName='npc_lorax' WHERE entry=10918;
UPDATE creature_template SET ScriptName='npc_rivern_frostwind' WHERE entry=10618;
UPDATE creature_template SET ScriptName='npc_witch_doctor_mauari' WHERE entry=10307;

/* ZANGARMARSH */
UPDATE creature_template SET ScriptName='npcs_ashyen_and_keleth' WHERE entry IN (17900,17901);
UPDATE creature_template SET ScriptName='npc_cooshcoosh' WHERE entry=18586;
UPDATE creature_template SET ScriptName='npc_elder_kuruti' WHERE entry=18197;
UPDATE creature_template SET ScriptName='npc_kayra_longmane' WHERE entry=17969;
UPDATE creature_template SET ScriptName='npc_mortog_steamhead' WHERE entry=23373;
UPDATE creature_template SET ScriptName='npc_timothy_daniels' WHERE entry=18019;

/* ZUL'AMAN */
UPDATE instance_template SET script='instance_zulaman' WHERE map=568;
UPDATE creature_template SET ScriptName='npc_harrison_jones_za' WHERE entry=24358;
UPDATE gameobject_template SET ScriptName='go_strange_gong' WHERE entry=187359;
UPDATE creature_template SET ScriptName='boss_akilzon' WHERE entry=23574;
UPDATE creature_template SET ScriptName='mob_soaring_eagle' WHERE entry=24858;
UPDATE creature_template SET ScriptName='boss_halazzi' WHERE entry=23577;
UPDATE creature_template SET ScriptName='boss_spirit_lynx' WHERE entry=24143;
UPDATE creature_template SET ScriptName='boss_janalai' WHERE entry=23578;
UPDATE creature_template SET ScriptName='boss_malacrass' WHERE entry=24239;
UPDATE creature_template SET ScriptName='mob_alyson_antille' WHERE entry=24240;
UPDATE creature_template SET ScriptName='mob_thurg' WHERE entry=24241;
UPDATE creature_template SET ScriptName='mob_slither' WHERE entry=24242;
UPDATE creature_template SET ScriptName='mob_lord_raadan' WHERE entry=24243;
UPDATE creature_template SET ScriptName='mob_gazakroth' WHERE entry=24244;
UPDATE creature_template SET ScriptName='mob_fenstalker' WHERE entry=24245;
UPDATE creature_template SET ScriptName='mob_darkheart' WHERE entry=24246;
UPDATE creature_template SET ScriptName='mob_koragg' WHERE entry=24247;
UPDATE creature_template SET ScriptName='boss_nalorakk' WHERE entry=23576;
UPDATE creature_template SET ScriptName='boss_zuljin' WHERE entry=23863;
UPDATE creature_template SET ScriptName='mob_jandalai_firebomb' WHERE entry=23920;
UPDATE creature_template SET ScriptName='mob_amanishi_hatcher' WHERE entry IN (23818,24504);
UPDATE creature_template SET ScriptName='mob_hatchling' WHERE entry=23598;
UPDATE creature_template SET ScriptName='npc_forest_frog' WHERE entry=24396;

/* ZUL'DRAK */


/* ZUL'FARRAK */
UPDATE creature_template SET ScriptName='npc_sergeant_bly' WHERE entry=7604;
UPDATE creature_template SET ScriptName='npc_weegli_blastfuse' WHERE entry=7607;

/* ZUL'GURUB */
UPDATE instance_template SET script='instance_zulgurub' WHERE map=309;
UPDATE creature_template SET ScriptName='boss_jeklik' WHERE entry=14517;
UPDATE creature_template SET ScriptName='boss_venoxis' WHERE entry=14507;
UPDATE creature_template SET ScriptName='boss_marli' WHERE entry=14510;
UPDATE creature_template SET ScriptName='boss_mandokir' WHERE entry=11382;
UPDATE creature_template SET ScriptName='mob_ohgan' WHERE entry=14988;
UPDATE creature_template SET ScriptName='boss_gahzranka' WHERE entry=15114;
UPDATE creature_template SET ScriptName='boss_jindo' WHERE entry=11380;
UPDATE creature_template SET ScriptName='boss_hakkar' WHERE entry=14834;
UPDATE creature_template SET ScriptName='boss_thekal' WHERE entry=14509;
UPDATE creature_template SET ScriptName='boss_arlokk' WHERE entry=14515;
UPDATE gameobject_template SET ScriptName='go_gong_of_bethekk' WHERE entry=180526;
UPDATE creature_template SET ScriptName='boss_grilek' WHERE entry=15082;
UPDATE creature_template SET ScriptName='boss_hazzarah' WHERE entry=15083;
UPDATE creature_template SET ScriptName='boss_renataki' WHERE entry=15084;
UPDATE creature_template SET ScriptName='boss_wushoolay' WHERE entry=15085;
UPDATE creature_template SET ScriptName='mob_zealot_lorkhan' WHERE entry=11347;
UPDATE creature_template SET ScriptName='mob_zealot_zath' WHERE entry=11348;
UPDATE creature_template SET ScriptName='mob_healing_ward' WHERE entry=14987;
UPDATE creature_template SET ScriptName='mob_spawn_of_marli' WHERE entry=15041;
UPDATE creature_template SET ScriptName='mob_batrider' WHERE entry=14965;
UPDATE creature_template SET ScriptName='mob_shade_of_jindo' WHERE entry=14986;

/* EOF */
