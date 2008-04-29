/*  */

/* WORLD BOSS */
UPDATE `creature_template` SET `ScriptName` = 'boss_ysondre' WHERE `entry` = 14887;
UPDATE `creature_template` SET `ScriptName` = 'boss_emeriss' WHERE `entry` = 14889;
UPDATE `creature_template` SET `ScriptName` = 'boss_taerar' WHERE `entry` = 14890;
UPDATE `creature_template` SET `ScriptName` = 'boss_shade_of_taerar' WHERE `entry` = 15302;
UPDATE `creature_template` SET `ScriptName` = 'boss_teremus_the_devourer' WHERE `entry` = 7846;
UPDATE `creature_template` SET `ScriptName` = 'boss_kruul' WHERE `entry` = 18338;
UPDATE `creature_template` SET `ScriptName` = 'boss_azuregos' WHERE `entry` = 6109;
UPDATE `creature_template` SET `ScriptName` = 'mob_dementeddruids' WHERE `entry` = 15260;

/* WORLD CREATURE  These are creatures to be found in more than one specific zone */
UPDATE `creature_template` SET `ScriptName` = 'npc_sayge' WHERE `entry` = 14822;

/* GO */
UPDATE `gameobject_template` SET `ScriptName` = 'go_orb_of_command' WHERE `entry` = 179879;
UPDATE `gameobject_template` SET `ScriptName` = 'go_barov_journal' WHERE `entry` = 180794;
UPDATE `gameobject_template` SET `ScriptName` = 'go_field_repair_bot_74A' where `entry`= 179552;
UPDATE `gameobject_template` SET `ScriptName` = 'go_northern_crystal_pylon' WHERE `entry` = 164955;
UPDATE `gameobject_template` SET `ScriptName` = 'go_western_crystal_pylon' WHERE `entry` = 164956;
UPDATE `gameobject_template` SET `ScriptName` = 'go_eastern_crystal_pylon' WHERE `entry` = 164957;
UPDATE `gameobject_template` SET `ScriptName` = 'go_tablet_of_madness' WHERE `entry` = 180368;


/* GUARD */
UPDATE `creature_template` SET `ScriptName` = 'guard_azuremyst' WHERE `entry` = 18038;
UPDATE `creature_template` SET `ScriptName` = 'guard_orgrimmar' WHERE `entry` = 3296;
UPDATE `creature_template` SET `ScriptName` = 'guard_stormwind' WHERE `entry` IN (68, 1976);
UPDATE `creature_template` SET `ScriptName` = 'guard_contested' WHERE `entry` IN (9460, 4624, 3502, 11190, 15184);
UPDATE `creature_template` SET `ScriptName` = 'guard_elwynnforest' WHERE `entry` = 1423;
UPDATE `creature_template` SET `ScriptName` = 'guard_eversong' WHERE `entry` = 16221;
UPDATE `creature_template` SET `ScriptName` = 'guard_darnassus' WHERE `entry` = 4262;
UPDATE `creature_template` SET `ScriptName` = 'guard_teldrassil' WHERE `entry` = 3571;
UPDATE `creature_template` SET `ScriptName` = 'guard_ironforge' WHERE `entry` = 5595;
UPDATE `creature_template` SET `ScriptName` = 'guard_dunmorogh' WHERE `entry` IN (727, 13076);
UPDATE `creature_template` SET `ScriptName` = 'guard_undercity' WHERE `entry` = 5624;
UPDATE `creature_template` SET `ScriptName` = 'guard_bluffwatcher' WHERE `entry` = 3084;
UPDATE `creature_template` SET `ScriptName` = 'guard_durotar' WHERE `entry` = 5953;
UPDATE `creature_template` SET `ScriptName` = 'guard_mulgore' WHERE `entry` IN (3212, 3215, 3217, 3218, 3219, 3220, 3221, 3222, 3223, 3224);
UPDATE `creature_template` SET `ScriptName` = 'guard_dunmorogh' WHERE `entry` IN (727, 13076);
UPDATE `creature_template` SET `ScriptName` = 'guard_tirisfal' WHERE `entry` IN (1735, 1738, 2210, 1744, 1745, 5725, 1743, 2209, 1746, 1742);
UPDATE `creature_template` SET `scriptName` = 'guard_silvermoon' WHERE `entry` = 16222;
UPDATE `creature_template` SET `scriptName` = 'guard_exodar' WHERE `entry` = 16733;
UPDATE `creature_template` SET `ScriptName` = 'guard_shattrath' WHERE `entry` = 19687;

/* ITEM */
UPDATE `item_template` SET `ScriptName` = 'item_nether_wraith_beacon' WHERE `entry` = 31742;
UPDATE `item_template` SET `ScriptName` = 'item_area_52_special' WHERE `entry` = 28132;
UPDATE `item_template` SET `ScriptName` = 'item_vorenthals_presence' WHERE `entry` = 30259;
UPDATE `item_template` SET `ScriptName` = 'item_draenei_fishing_net' WHERE `entry` = 23654;
UPDATE `item_template` SET `ScriptName` = 'item_tainted_core' WHERE `entry` = 31088;
UPDATE `item_template` SET `ScriptName` = 'item_yehkinyas_bramble' WHERE `entry` = 10699;
UPDATE `item_template` SET `ScriptName` = 'item_zezzaks_shard' WHERE `entry` = 31463;

/* NPC */
UPDATE `creature_template` SET `ScriptName` = 'npc_guardian' WHERE `entry` = 5764;
UPDATE `creature_template` SET `ScriptName` = 'npc_mount_vendor' WHERE `entry` IN (384, 1261, 1460, 2357, 3362, 3685, 4730, 4731, 4885, 7952, 7955, 16264, 17584);

/*  */
/* ZONE */
/* */

/* ALTERAC MOUNTAINS */


/* ALTERAC VALLEY */


/* ARATHI HIGHLANDS */


/* ASHENVALE */


/*  */
/* AUCHINDOUN */
/*  */

/* MANA TOMBS */
UPDATE `creature_template` SET `ScriptName` = 'boss_tavarok' WHERE `entry` = 18343;
UPDATE `creature_template` SET `ScriptName` = 'boss_pandemonius' WHERE `entry` = 18341;
UPDATE `creature_template` SET `ScriptName` = 'boss_nexusprince_shaffar' WHERE `entry` = 18344;
UPDATE `creature_template` SET `ScriptName` = 'mob_ethereal_beacon' WHERE `entry` = 18431;
UPDATE `creature_template` SET `ScriptName` = 'mob_ethereal_apprentice' WHERE `entry` = 18430;

/* AUCHENAI CRYPTS */
UPDATE `creature_template` SET `ScriptName` = 'boss_shirrak_the_dead_watcher' WHERE `entry` = 18371;
UPDATE `creature_template` SET `ScriptName` = 'boss_shirrak_the_dead_watcher' WHERE `entry` = 20318;
UPDATE `creature_template` SET `ScriptName` = 'boss_exarch_maladaar' WHERE `entry` = 18373;

/* SETHEKK HALLS */
UPDATE `instance_template` SET `script` = 'instance_sethekk_halls' WHERE `map` = 556;
UPDATE `creature_template` SET `ScriptName` = 'mob_syth_fire' WHERE `entry` = 19203;
UPDATE `creature_template` SET `ScriptName` = 'mob_syth_arcane' WHERE `entry` = 19205;
UPDATE `creature_template` SET `ScriptName` = 'mob_syth_frost' WHERE `entry` = 19204;
UPDATE `creature_template` SET `ScriptName` = 'mob_syth_shadow' WHERE `entry` = 19206;
UPDATE `creature_template` SET `ScriptName` = 'boss_tailonking_ikiss' WHERE `entry` = 18473;
UPDATE `creature_template` SET `ScriptName` = 'boss_darkweaver_syth' WHERE `entry` = 18472;

/* SHADOW LABYRINTH */
UPDATE `instance_template` SET `script` = 'instance_shadow_labyrinth' WHERE `map` = 555;
UPDATE `creature_template` SET `ScriptName` = 'boss_murmur' WHERE `entry` = 18708;
UPDATE `creature_template` SET `ScriptName` = 'boss_ambassador_hellmaw' WHERE `entry` = 18731;
UPDATE `creature_template` SET `ScriptName` = 'boss_grandmaster_vorpil' WHERE `entry` = 18732;
UPDATE `creature_template` SET `ScriptName` = 'boss_blackheart_the_inciter' WHERE `entry` = 18667;

/* AZSHARA */
UPDATE `creature_template` SET `ScriptName` = 'mobs_spitelashes' WHERE `entry` IN (6190,6193,6194,6195,6196,7885,7886,12204,12205);
UPDATE `creature_template` SET `ScriptName` = 'npc_loramus_thalipedes' WHERE `entry` = 7783;

/* AZUREMYST ISLE */
UPDATE `creature_template` SET `ScriptName` = 'injured_draenei' WHERE `entry` = 16971;
UPDATE `creature_template` SET `ScriptName` = 'npc_susurrus' WHERE `entry` = 17435;

/* BADLANDS */


/* BARRENS */
UPDATE `creature_template` SET `ScriptName` = 'npc_beaten_corpse' WHERE `entry` = 10668;
UPDATE `creature_template` SET `ScriptName` = 'npc_sputtervalve' WHERE `entry` = 3442;

/* BLACK TEMPLE */
UPDATE `instance_template` SET `script` = 'instance_black_temple' WHERE `map` = 564;
UPDATE creature_template SET ScriptName = 'npc_akama_shade' WHERE `entry` = 23089; -- Akama at Shade of Akama
UPDATE creature_template SET ScriptName = 'npc_akama_illidan' WHERE `entry` = 22990; -- Akama at Illidan
UPDATE creature_template SET ScriptName = 'illidari_council' WHERE `entry` = 23426; -- Illidari Council controller mob
UPDATE creature_template SET ScriptName = 'boss_veras_darkshadow' WHERE `entry` = 22952; -- Rogue of Illidari Council
UPDATE creature_template SET ScriptName = 'boss_teron_gorefiend' WHERE `entry` = 22871; -- Teron Gorefiend
UPDATE creature_template SET ScriptName = 'boss_supremus' WHERE `entry` = 22898; -- Supremus
UPDATE creature_template SET ScriptName = 'boss_shade_of_akama' WHERE `entry` = 22841; -- Shade of Akama
UPDATE creature_template SET ScriptName = 'boss_reliquary_of_souls' WHERE `entry` = 22856; -- Reliquary Controller Mob
UPDATE creature_template SET ScriptName = 'boss_essence_of_suffering' WHERE `entry` = 23418; -- Essence Of Suffering
UPDATE creature_template SET ScriptName = 'boss_essence_of_desire' WHERE `entry` = 23419; -- Essence of Desire
UPDATE creature_template SET ScriptName = 'boss_essence_of_anger' WHERE `entry` = 23420; -- Essence of Anger
UPDATE creature_template SET ScriptName = 'boss_najentus' WHERE `entry` = 22887; -- High Warlord Naj'entus
UPDATE creature_template SET ScriptName = 'boss_gurtogg_bloodboil' WHERE `entry` = 22948; -- Gurtogg Bloodboil
UPDATE creature_template SET ScriptName = 'boss_mother_shahraz' WHERE `entry` = 22947; -- Mother Shahraz
UPDATE creature_template SET ScriptName = 'boss_lady_malande' WHERE `entry` = 22951; -- Priest <3 at Illidari Council
UPDATE creature_template SET ScriptName = 'boss_illidan_stormrage' WHERE `entry` = 22917; -- Illidan The Betrayer!
UPDATE creature_template SET ScriptName = 'boss_high_nethermancer_zerevor' WHERE `entry` = 22950; -- Mage at Illidari Council
UPDATE creature_template SET ScriptName = 'boss_gathios_the_shatterer' WHERE `entry` = 22949; -- Paladin at Illidari Council
UPDATE creature_template SET ScriptName = 'boss_maiev_shadowsong' WHERE `entry` = 23197; -- Maiev Shadowsong
UPDATE creature_template SET ScriptName = 'mob_blaze' WHERE `entry` = 23259; -- Blaze mob in Illidan Phase 2
UPDATE creature_template SET ScriptName = 'mob_flame_of_azzinoth' WHERE `entry` = 22997; -- Flame of Azzinoth (Illidan Phase 2)
UPDATE creature_template SET ScriptName = 'mob_blade_of_azzinoth' WHERE `entry` = 22996; -- Blade of Azzinoth (Illidan Phase 2)
UPDATE creature_template SET ScriptName = 'mob_demon_fire' WHERE `entry` = 23069; -- Demon Fire in Illidan Phase 2
UPDATE creature_template SET ScriptName = 'mob_flame_crash' WHERE `entry` = 23336; -- Flame Crash in Illidan Normal Form
UPDATE creature_template SET ScriptName = 'mob_cage_trap_trigger' WHERE `entry` = 23304; -- Cage Trap mob in Illidan Phase 3/4 Normal
UPDATE creature_template SET ScriptName = 'mob_shadow_demon' WHERE `entry` = 23375; -- Shadow Demon in Illidan Demon Form
UPDATE creature_template SET ScriptName = 'npc_volcano' WHERE `entry` = 23085; -- Supremus Volcano
UPDATE creature_template SET ScriptName = 'molten_flame' WHERE `entry` = 23095; -- Molten Flame in SUpremus
UPDATE creature_template SET ScriptName = 'mob_ashtongue_channeler' WHERE `entry` = 23421; -- Ashtongue CHanneler in Shade of AKama
UPDATE creature_template SET ScriptName = 'mob_ashtongue_sorcerer' WHERE `entry` = 23215; -- Ashtongue Sorcerer in Shade of Akama
UPDATE creature_template SET ScriptName = 'npc_enslaved_soul' WHERE `entry` = 23469; -- Enslaved Soul in Reliquary Event
UPDATE creature_template SET ScriptName = 'mob_doom_blossom' WHERE `entry` = 23123; -- Doom Blossoms in Teron Gorefiend's encounter
-- UPDATE creature_template sET ScriptName = 'mob_shadowy_construct' WHERE `entry` = 23111; -- Shadowy Construct in Teron Gorefiend's encounter. Commented until Mind Control is implemented.
-- UPDATE creature_template SET ScriptName = 'mob_najentus_spine' WHERE `entry` = 500000; -- Workaround creature for spine in Najentus event

/* BT Trash Mobs */
UPDATE `creature_template` SET `ScriptName` = 'mob_blacktemple' WHERE `entry` IN (22844,22845,22846,22849,22853,22855,22869,22873,22875,22876,22877,22878,22880,22881,22882,22883,22884,22945,22953,22954,23337,23339);

/* BLACKFATHOM DEPTHS */


/* BLACKROCK DEPTHS */
UPDATE `creature_template` SET `ScriptName` = 'boss_emperor_dagran_thaurissan' WHERE `entry` = 9019;
UPDATE `creature_template` SET `ScriptName` = 'boss_moira_bronzebeard' WHERE `entry` = 8929;
UPDATE `creature_template` SET `ScriptName` = 'boss_golem_lord_argelmach' WHERE `entry` = 8983;
UPDATE `creature_template` SET `ScriptName` = 'boss_baelgar' WHERE `entry` = 9016;
UPDATE `creature_template` SET `ScriptName` = 'boss_ambassador_flamelash' WHERE `entry` = 9156;
UPDATE `creature_template` SET `ScriptName` = 'boss_angerrel' WHERE `entry` = 9035;
UPDATE `creature_template` SET `ScriptName` = 'boss_anubshiah' WHERE `entry` = 9031;
UPDATE `creature_template` SET `ScriptName` = 'boss_doomrel' WHERE `entry` = 9039;
UPDATE `creature_template` SET `ScriptName` = 'boss_doperel' WHERE `entry` = 9040;
UPDATE `creature_template` SET `ScriptName` = 'boss_fineous_darkvire' WHERE `entry` = 9056;
UPDATE `creature_template` SET `ScriptName` = 'boss_general_angerforge' WHERE `entry` = 9033;
UPDATE `creature_template` SET `ScriptName` = 'boss_gloomrel' WHERE `entry` = 9037;
UPDATE `creature_template` SET `ScriptName` = 'boss_gorosh_the_dervish' WHERE `entry` = 9027;
UPDATE `creature_template` SET `ScriptName` = 'boss_grizzle' WHERE `entry` = 9028;
UPDATE `creature_template` SET `ScriptName` = 'boss_haterel' WHERE `entry` = 9034;
UPDATE `creature_template` SET `ScriptName` = 'boss_high_interrogator_gerstahn' WHERE `entry` = 9018;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_incendius' WHERE `entry` = 9017;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_roccor' WHERE `entry` = 9025;
UPDATE `creature_template` SET `ScriptName` = 'boss_magmus' WHERE `entry` = 9938;
UPDATE `creature_template` SET `ScriptName` = 'boss_pyromancer_loregrain' WHERE `entry` = 9024;
UPDATE `creature_template` SET `ScriptName` = 'boss_seethrel' WHERE `entry` = 9038;
UPDATE `creature_template` SET `ScriptName` = 'boss_verek' WHERE `entry` = 9042;
UPDATE `creature_template` SET `ScriptName` = 'boss_vilerel' WHERE `entry` = 9036;
UPDATE `creature_template` SET `ScriptName` = 'boss_warder_stilgiss' WHERE `entry` = 9041;
UPDATE `creature_template` SET `ScriptName` = 'overmaster_pyron' WHERE `entry` = 9026;
UPDATE `creature_template` SET `ScriptName` = 'phalanx' WHERE `entry` = 9502;
UPDATE `creature_template` SET `ScriptName` = 'npc_lokhtos_darkbargainer' WHERE `entry` = 12944;
UPDATE `creature_template` SET `ScriptName` = 'npc_kharan_mighthammer' WHERE `entry` = 9021;

/* BLACKROCK SPIRE */
UPDATE `creature_template` SET `ScriptName` = 'mob_chromatic_elite_guard' WHERE `entry` = 10814;

/* BLACKROCK SPIRE Lower bosses */
UPDATE `creature_template` SET `ScriptName` = 'boss_highlord_omokk' WHERE `entry` = 9196;
UPDATE `creature_template` SET `ScriptName` = 'boss_shadow_hunter_voshgajin' WHERE `entry` = 9236;
UPDATE `creature_template` SET `ScriptName` = 'boss_warmaster_voone' WHERE `entry` = 9237;
UPDATE `creature_template` SET `ScriptName` = 'boss_mother_smolderweb' WHERE `entry` = 10596;
UPDATE `creature_template` SET `ScriptName` = 'quartermaster_zigris' WHERE `entry` = 9736;
UPDATE `creature_template` SET `ScriptName` = 'boss_halycon' WHERE `entry` = 10220;
UPDATE `creature_template` SET `ScriptName` = 'boss_overlord_wyrmthalak' WHERE `entry` = 9568;

/* BLACKROCK SPIRE Upper bosses */
UPDATE `creature_template` SET `ScriptName` = 'boss_the_beast' WHERE `entry` = 10430;
UPDATE `creature_template` SET `ScriptName` = 'boss_drakkisath' WHERE `entry` = 10363;
UPDATE `creature_template` SET `ScriptName` = 'boss_gyth' WHERE `entry` = 10339;
UPDATE `creature_template` SET `ScriptName` = 'boss_rend_blackhand' WHERE `entry` = 10429;
UPDATE `creature_template` SET `ScriptName` = 'boss_pyroguard_emberseer' WHERE `entry` = 9816;

/* BLACKWING LAIR */
UPDATE `instance_template` SET `script` = 'instance_blackwing_lair' WHERE `map` = 469;
/* Trash Mobs */
UPDATE `creature_template` SET `ScriptName` = 'mob_blackwing_lair' WHERE `entry` IN (14265, 12457, 13996, 12459, 14261, 14263, 12467, 12463, 12461, 12464, 12460, 12465, 14262, 14264, 12557, 12468);
/* Bosses */
UPDATE `creature_template` SET `ScriptName` = 'boss_razorgore' WHERE `entry` = 12435;
UPDATE `creature_template` SET `ScriptName` = 'boss_vaelastrasz' WHERE `entry` = 13020;
UPDATE `creature_template` SET `ScriptName` = 'boss_broodlord' WHERE `entry` = 12017;
UPDATE `creature_template` SET `ScriptName` = 'boss_firemaw' WHERE `entry` = 11983;
UPDATE `creature_template` SET `ScriptName` = 'boss_ebonroc' WHERE `entry` = 14601;
UPDATE `creature_template` SET `ScriptName` = 'boss_flamegor' WHERE `entry` = 11981;
UPDATE `creature_template` SET `ScriptName` = 'boss_chromaggus' WHERE `entry` = 14020;
UPDATE `creature_template` SET `ScriptName` = 'boss_victor_nefarius' WHERE `entry` = 10162;
UPDATE `creature_template` SET `ScriptName` = 'boss_nefarian' WHERE `entry` = 11583;

/* BLADE'S EDGE MOUNTAINS */
UPDATE `creature_template` SET `ScriptName` = 'mobs_bladespire_ogre' WHERE `entry` IN (19998, 20334, 21296, 21975);
UPDATE `creature_template` SET `ScriptName` = 'mobs_bloodmaul_ogre' WHERE `entry` IN (19948, 19952, 19957);
UPDATE `creature_template` SET `ScriptName` = 'npc_daranelle' WHERE `entry` = 21469;
UPDATE `creature_template` SET `ScriptName` = 'npc_overseer_nuaar' WHERE `entry` = 21981;
UPDATE `creature_template` SET `ScriptName` = 'npc_saikkal_the_elder' WHERE `entry` = 22932;
UPDATE `creature_template` SET `ScriptName` = 'npc_skyguard_handler_deesak' WHERE `entry` = 23415;

/* BLASTED LANDS */
UPDATE `creature_template` SET `ScriptName` = 'npc_deathly_usher' WHERE `entry` = 8816;
UPDATE `creature_template` SET `ScriptName` = 'npc_fallen_hero_of_horde' WHERE `entry` = 7572;

/* BLOODMYST ISLE */
UPDATE `creature_template` SET `ScriptName` = 'mob_webbed_creature' WHERE `entry` = 17680;
UPDATE `creature_template` SET `ScriptName` = 'npc_captured_sunhawk_agent' WHERE `entry` = 17824;

/* BURNING STEPPES */
UPDATE `creature_template` SET `ScriptName` = 'npc_ragged_john' WHERE `entry` = 9563;

/*  */
/* CAVERNS OF TIME */
/*  */

/* MT. HYJAL */
UPDATE `instance_template` SET `script` = 'instance_hyjal' WHERE `map` = 534;
UPDATE creature_template SET ScriptName = 'npc_tyrande_whisperwind' WHERE `entry` = 17948;
UPDATE creature_template SET ScriptName = 'npc_thrall' WHERE `entry`  = 17852;
UPDATE creature_template SET ScriptName = 'npc_jaina_proudmoore' WHERE `entry` = 17772;

/* OLD HILLSBRAD */
UPDATE `creature_template` SET `ScriptName` = 'boss_lieutenant_drake' WHERE `entry` = 17848;
UPDATE `creature_template` SET `ScriptName` = 'boss_epoch_hunter' WHERE `entry` = 18096;
UPDATE `creature_template` SET `ScriptName` = 'boss_captain_skarloc' WHERE `entry` = 17862;

/* THE DARK PORTAL */
UPDATE `creature_template` SET `ScriptName` = 'boss_chrono_lord_deja' WHERE `entry` = 17879;
UPDATE `creature_template` SET `ScriptName` = 'boss_aeonus' WHERE `entry` = 17881;
UPDATE `creature_template` SET `ScriptName` = 'boss_temporus' WHERE `entry` = 17880;


/*  */
/* COILFANG RESERVOIR */
/*  */

/* THE SLAVE PENS */
UPDATE `creature_template` SET `ScriptName` = 'boss_rokmar_the_crackler' WHERE `entry` = 17991;

/* THE UNDERBOG */
UPDATE `creature_template` SET `ScriptName` = 'boss_ghazan' WHERE `entry` = 18105;
UPDATE `creature_template` SET `ScriptName` = 'mob_underbog_mushroom' WHERE `entry` = 17990;
UPDATE `creature_template` SET `ScriptName` = 'boss_hungarfen' WHERE `entry` = 17770;

/* THE STEAMVAULT */
UPDATE `instance_template` SET `script` = 'instance_steam_vault' WHERE `map` = 545;
UPDATE `creature_template` SET `ScriptName` = 'boss_warlord_kalithresh' WHERE `entry` = 17798;
UPDATE `creature_template` SET `ScriptName` = 'boss_hydromancer_thespia' WHERE `entry` = 17797;
UPDATE `creature_template` SET `ScriptName` = 'mob_coilfang_waterelemental' WHERE `entry` = 17917;

/* SERPENTSHRINE CAVERN */
UPDATE `instance_template` SET `script` = 'instance_serpent_shrine' WHERE `map` = 548;
/* Serpentshrine Cavern Trash Mobs */
UPDATE `creature_template` SET `ScriptName` = 'mob_serpentshrine_cavern' WHERE `entry` IN (21246, 21339, 21221, 21224, 21227, 21228, 21226, 21225, 21298, 21299);
/* Hydross the Unstable */
UPDATE `creature_template` SET `ScriptName` = 'boss_hydross_the_unstable' WHERE `entry` = 21216;
/* Leotheras the Blind event */
UPDATE `creature_template` SET `ScriptName` = 'boss_leotheras_the_blind' WHERE `entry` = 21215;
UPDATE `creature_template` SET `ScriptName` = 'boss_leotheras_the_blind_demonform' WHERE `entry` = 21845;
/* Fathom-lord Karathress event */
UPDATE `creature_template` SET `ScriptName` = 'boss_fathomlord_karathress' WHERE `entry` = 21214;
UPDATE `creature_template` SET `ScriptName` = 'boss_fathomguard_sharkkis' WHERE `entry` = 21966;
UPDATE `creature_template` SET `ScriptName` = 'boss_fathomguard_tidalvess' WHERE `entry` = 21965;
UPDATE `creature_template` SET `ScriptName` = 'boss_fathomguard_caribdis' WHERE `entry` = 21964;
/* Morogrim Tidewalker event */
UPDATE `creature_template` SET `ScriptName` = 'boss_morogrim_tidewalker' WHERE `entry`=21213;
UPDATE `creature_template` SET `ScriptName` = 'mob_water_globule' WHERE `entry`=21913;
/* Lady Vashj event */
UPDATE `creature_template` SET `ScriptName` = 'boss_lady_vashj' WHERE `entry` = 21212;
UPDATE `creature_template` SET `ScriptName` = 'mob_enchanted_elemental' WHERE `entry` = 21958;
UPDATE `creature_template` SET `ScriptName` = 'mob_tainted_elemental' WHERE `entry` = 22009;
UPDATE `creature_template` SET `ScriptName` = 'mob_coilfang_elite' WHERE `entry` = 22055;
UPDATE `creature_template` SET `ScriptName` = 'mob_coilfang_strider' WHERE `entry` = 22056;
UPDATE `creature_template` SET `ScriptName` = 'mob_fathom_sporebat' WHERE `entry` = 22120;
UPDATE `creature_template` SET `ScriptName` = 'mob_shield_generator_channel' WHERE `entry` = 19870;


/* DARKSHORE */
UPDATE `creature_template` SET `ScriptName` = 'mob_lunaclaw' WHERE `entry` = 12138;

/* DEADMINES */
UPDATE `creature_template` SET `ScriptName` = 'boss_deadmines' WHERE `entry` IN (645, 1763, 599, 3586);

/* DEADWIND PASS */


/* DESOLACE */


/* DIRE MAUL */


/* DUN MOROGH */
UPDATE `creature_template` SET `ScriptName` = 'npc_narm_faulk' WHERE `entry` = 6177;


/* DUROTAR */


/* DUSKWOOD */


/* DUSTWALLOW MARSH */
UPDATE `creature_template` SET `ScriptName` = 'npc_doctor' WHERE `entry` IN (12939, 12920);
UPDATE `creature_template` SET `ScriptName` = 'npc_injured_patient' WHERE `entry` IN (12936, 12937, 12938, 12923, 12924, 12925);
UPDATE `creature_template` SET `ScriptName` = 'npc_deserter_agitator' WHERE `entry` = 23602;
UPDATE `creature_template` SET `ScriptName` = 'npc_lady_jaina_proudmoore' WHERE `entry` = 4968;
UPDATE `creature_template` SET `ScriptName` = 'npc_nat_pagle' WHERE `entry` = 12919;
UPDATE `creature_template` SET `ScriptName` = 'npc_restless_apparition' WHERE `entry` = 23861;
UPDATE `creature_template` SET `ScriptName` = 'mobs_direhorn_grimtotem' WHERE `entry` = 23595;
UPDATE `creature_template` SET `ScriptName` = 'mobs_risen_husk_spirit' WHERE `entry` IN (23554, 23555);

/* EASTERN PLAGUELANDS */
UPDATE `creature_template` SET `ScriptName` = 'mobs_ghoul_flayer' WHERE `entry` IN (8530, 8531, 8532);
UPDATE `creature_template` SET `ScriptName` = 'npc_darrowshire_spirit' WHERE `entry` = 11064;
UPDATE `creature_template` SET `ScriptName` = 'npc_tirion_fordring' WHERE `entry` = 1855;

/* ELWYNN FOREST */
UPDATE `creature_template` SET `ScriptName` = 'npc_henze_faulk' WHERE `entry` = 6172;

/* EVERSONG WOODS */
UPDATE `creature_template` SET `ScriptName` = 'mobs_mana_tapped' WHERE `entry` IN (15273,15274,15294,15298,15367);

/* FELWOOD */
UPDATE `creature_template` SET `ScriptName` = 'npcs_riverbreeze_and_silversky' WHERE `entry` IN (9528, 9529);

/* FERALAS */
UPDATE `creature_template` SET `ScriptName` = 'npc_screecher_spirit' WHERE `entry` = 8612;

/* GHOSTLANDS */
UPDATE `creature_template` SET `ScriptName` = 'npc_blood_knight_dawnstar' WHERE `entry` = 17832;
UPDATE `creature_template` SET `ScriptName` = 'npc_budd_nedreck' WHERE `entry` = 23559;

/* GNOMEREGAN */


/* GRUUL'S LAIR */
UPDATE `instance_template` SET `script` = 'instance_gruuls_lair' WHERE `map` =565;
/* Gruul's Lair Trash Mobs */
UPDATE `creature_template` SET `ScriptName` = 'mob_gruuls_lair' WHERE `entry` IN (19389, 21350);
/* Gruul the Dragonkiller */
UPDATE `creature_template` SET `ScriptName` = 'boss_gruul' WHERE `entry` = 19044;
/* Maulgar and Event */
UPDATE `creature_template` SET `ScriptName` = 'boss_high_king_maulgar' WHERE `entry` = 18831;
UPDATE `creature_template` SET `ScriptName` = 'boss_kiggler_the_crazed' WHERE `entry` = 18835;
UPDATE `creature_template` SET `ScriptName` = 'boss_blindeye_the_seer' WHERE `entry` = 18836;
UPDATE `creature_template` SET `ScriptName` = 'boss_olm_the_summoner' WHERE `entry` = 18834;
UPDATE `creature_template` SET `ScriptName` = 'boss_krosh_firehand' WHERE `entry` = 18832;

/*  */
/* HELLFIRE CITADEL */
/*  */

/* BLOOD FURNACE */
/* The Maker, Broggok, Kelidan, Broggok's cloud */
UPDATE `creature_template` SET `ScriptName` = 'boss_the_maker' WHERE `entry` = 17381;
UPDATE `creature_template` SET `ScriptName` = 'boss_broggok' WHERE `entry` = 17380;
UPDATE `creature_template` SET `ScriptName` = 'boss_kelidan_the_breaker' WHERE `entry` = 17377;
UPDATE `creature_template` SET `ScriptName` = 'mob_broggok_poisoncloud' WHERE `entry` = 17662;

/* HELLFIRE RAMPARTS */
/* Vazruden, Omor the Unscarred, Watchkeeper Gargolmar */
UPDATE `creature_template` SET `ScriptName` = 'boss_omor_the_unscarred' WHERE `entry` = 17308;
UPDATE `creature_template` SET `ScriptName` = 'boss_watchkeeper_gargolmar' WHERE `entry` = 17306;

/* SHATTERED HALLS */
/* Nethekurse and his spawned shadowfissure */
UPDATE `creature_template` SET `ScriptName` = 'boss_grand_warlock_nethekurse' WHERE `entry` = 16807;
UPDATE `creature_template` SET `ScriptName` = 'mob_nethe_shadowfissure' WHERE `entry` = 17471;

/* MAGTHERIDON'S LAIR */
UPDATE `instance_template` SET `script` = 'instance_magtheridons_lair' WHERE `map` = 544;
UPDATE `gameobject_template` SET `ScriptName` = 'go_manticron_cube' WHERE `entry` = 181713;
UPDATE `creature_template` SET `ScriptName` = 'boss_magtheridon' WHERE `entry` =17257;
UPDATE `creature_template` SET `ScriptName` = 'mob_hellfire_channeler' WHERE `entry` =17256;

/* HELLFIRE PENINSULA */
UPDATE `creature_template` SET `ScriptName` = 'boss_doomlord_kazzak' WHERE `entry` = 18728;
UPDATE `creature_template` SET `ScriptName` = 'mobs_bonechewer_orc' WHERE `entry` IN (16876, 16925, 18952, 19701);
UPDATE `creature_template` SET `ScriptName` = 'mobs_shattered_hand_orc' WHERE `entry` IN (19411, 19410, 19413, 19414, 16867, 19295, 16870, 16878, 19415);
UPDATE `creature_template` SET `ScriptName` = 'npc_wing_commander_brack' WHERE `entry` = 19401;
UPDATE `creature_template` SET `ScriptName` = 'npc_wing_commander_dabiree' WHERE `entry` = 19409;
UPDATE `creature_template` SET `ScriptName` = 'npc_gryphoneer_windbellow' WHERE `entry` = 20235;

/* HILLSBRAD FOOTHILLS */


/* HINTERLANDS */


/* IRONFORGE */
UPDATE `creature_template` SET `ScriptName` = 'npc_royal_historian_archesonus' WHERE `entry` = 8879;

/* ISLE OF QUEL'DANAS */
UPDATE `creature_template` SET `ScriptName` = 'npc_ayren_cloudbreaker' WHERE `entry` = 25059;
UPDATE `creature_template` SET `ScriptName` = 'npc_unrestrained_dragonhawk' WHERE `entry` = 25236;

/* KARAZHAN */
UPDATE `instance_template` SET `script` = 'instance_karazhan' WHERE `map` = 532;
UPDATE `creature_template` SET `ScriptName` = 'boss_midnight' WHERE `entry` = 16151;
UPDATE `creature_template` SET `ScriptName` = 'boss_attumen' WHERE `entry` = 15550;
UPDATE `creature_template` SET `ScriptName` = 'boss_moroes' WHERE `entry` = 15687;
UPDATE `creature_template` SET `ScriptName` = 'boss_maiden_of_virtue' WHERE `entry` = 16457;
UPDATE `creature_template` SET `ScriptName` = 'boss_curator' WHERE `entry` = 15691;
UPDATE `creature_template` SET `ScriptName` = 'boss_big_bad_wolf' WHERE `entry` = 17521;
UPDATE `creature_template` SET `ScriptName` = 'boss_julianne' WHERE `entry` = 17534;
UPDATE `creature_template` SET `ScriptName` = 'boss_romulo' WHERE `entry` = 17533;
UPDATE `creature_template` SET `ScriptName` = 'boss_dorothee' WHERE `entry` = 17535;
UPDATE `creature_template` SET `ScriptName` = 'boss_strawman' WHERE `entry` = 17543;
UPDATE `creature_template` SET `ScriptName` = 'boss_tinhead' WHERE `entry` = 17547;
UPDATE `creature_template` SET `ScriptName` = 'boss_tito' WHERE `entry` = 17548;
UPDATE `creature_template` SET `ScriptName` = 'boss_roar' WHERE `entry` = 17546;
UPDATE `creature_template` SET `ScriptName` = 'boss_crone' WHERE `entry` = 18168;
UPDATE `creature_template` SET `ScriptName` = 'boss_terestian_illhoof' WHERE `entry` = 15688;
UPDATE `creature_template` SET `ScriptName` = 'boss_shade_of_aran' WHERE `entry` = 16524;
UPDATE `creature_template` SET `ScriptName` = 'boss_netherspite' WHERE `entry` = 15689;
UPDATE `creature_template` SET `ScriptName` = 'boss_malchezaar' WHERE `entry` = 15690;
UPDATE `creature_template` SET `ScriptName` = 'boss_nightbane' WHERE `entry` = 17225;
UPDATE `creature_template` SET `ScriptName` = 'mob_shadow_of_aran' WHERE `entry` = 18254;
UPDATE `creature_template` SET `ScriptName` = 'mob_aran_elemental' WHERE `entry` = 17167;
UPDATE `creature_template` SET `ScriptName` = 'mob_aran_blizzard' WHERE `entry` = 17161;
UPDATE `creature_template` SET `ScriptName` = 'mob_homunculus' WHERE `entry` = 16539;
UPDATE `creature_template` SET `ScriptName` = 'mob_kilrek' WHERE `entry` = 17229;
UPDATE `creature_template` SET `ScriptName` = 'mob_demon_chain' WHERE `entry` = 17248;
UPDATE `creature_template` SET `ScriptName` = 'netherspite_infernal' WHERE `entry` = 17646;
UPDATE `creature_template` SET `ScriptName` = 'npc_berthold' WHERE `entry` = 16153;
update creature_template set scriptname = "boss_moroes" where entry = 15687;
update creature_template set scriptname = "boss_baroness_dorothea_millstipe" where entry = 19875;
update creature_template set scriptname = "boss_baron_rafe_dreuger" where entry = 19874;
update creature_template set scriptname = "boss_lady_catriona_von_indi" where entry = 19872;
update creature_template set scriptname = "boss_lady_keira_berrybuck" where entry = 17007;
update creature_template set scriptname = "boss_lord_robin_daris" where entry = 19876;
update creature_template set scriptname = "boss_lord_crispin_ference" where entry = 19873;
update creature_template set scriptname = "npc_barnes" where entry = 16812;
update creature_template set scriptname = "boss_dorothee" where entry = 17535;
update creature_template set scriptname = "boss_strawman" where entry = 17543;
update creature_template set scriptname = "boss_tinhead" where entry = 17547;
update creature_template set scriptname = "boss_roar" where entry = 17546;
update creature_template set scriptname = "boss_crone" where entry = 18168;
update creature_template set scriptname = "mob_cyclone" where entry = 18412;
update creature_template set scriptname = "npc_grandmother" where entry = 17603;
update creature_template set scriptname = "boss_bigbadwolf" where entry = 17521;

/* LOCH MODAN */
UPDATE `creature_template` SET `ScriptName` = 'npc_mountaineer_pebblebitty' WHERE `entry` = 3836;


/* MARAUDON */
UPDATE `creature_template` SET `ScriptName` = 'boss_princess_theradras' WHERE `entry` = 12201;
UPDATE `creature_template` SET `ScriptName` = 'boss_tinkerer_gizlock' WHERE `entry` = 13601;
UPDATE `creature_template` SET `ScriptName` = 'boss_landslide' WHERE `entry` = 12203;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_vyletongue' WHERE `entry` = 12236;
UPDATE `creature_template` SET `ScriptName` = 'boss_noxxion' WHERE `entry` = 13282;
UPDATE `creature_template` SET `ScriptName` = 'boss_rotgrip' WHERE `entry` = 13596;
UPDATE `creature_template` SET `ScriptName` = 'celebras_the_cursed' WHERE `entry` = 12225;
UPDATE `creature_template` SET `ScriptName` = 'meshlok_the_harvester' WHERE `entry` = 12237;
UPDATE `creature_template` SET `ScriptName` = 'razorlash' WHERE `entry` = 12258;

/* MOLTEN CORE */
UPDATE `instance_template` SET `script` = 'instance_molten_core' WHERE `map` = 409;
UPDATE `creature_template` SET `ScriptName` = 'boss_lucifron' WHERE `entry` = 12118;
UPDATE `creature_template` SET `ScriptName` = 'boss_magmadar' WHERE `entry` = 11982;
UPDATE `creature_template` SET `ScriptName` = 'boss_gehennas' WHERE `entry` = 12259;
UPDATE `creature_template` SET `ScriptName` = 'boss_garr' WHERE `entry` = 12057;
UPDATE `creature_template` SET `ScriptName` = 'boss_baron_geddon' WHERE `entry` = 12056;
UPDATE `creature_template` SET `ScriptName` = 'boss_shazzrah' WHERE `entry` = 12264;
UPDATE `creature_template` SET `ScriptName` = 'boss_golemagg' WHERE `entry` = 11988;
UPDATE `creature_template` SET `ScriptName` = 'boss_sulfuron' WHERE `entry` = 12098;
UPDATE `creature_template` SET `ScriptName` = 'boss_majordomo' WHERE `entry` = 12018;
UPDATE `creature_template` SET `ScriptName` = 'boss_ragnaros' WHERE `entry` = 11502;
UPDATE `creature_template` SET `ScriptName` = 'mob_firelord' WHERE `entry` =11668;
UPDATE `creature_template` SET `ScriptName` = 'mob_ancient_core_hound' WHERE `entry` = 11673;
UPDATE `creature_template` SET `ScriptName` = 'mob_core_rager' WHERE `entry` = 11672;
UPDATE `creature_template` SET `ScriptName` = 'mob_firesworn' WHERE `entry` = 12099;
UPDATE `creature_template` SET `ScriptName` = 'mob_firewalker' WHERE `entry` = 11666;
UPDATE `creature_template` SET `ScriptName` = 'mob_flame_guard' WHERE `entry` = 11667;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker' WHERE `entry` = 11661;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_elite' WHERE `entry` = 11664;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_healer' WHERE `entry` = 11663;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_priest' WHERE `entry` = 11662;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_protector' WHERE `entry` = 12119;
UPDATE `creature_template` SET `ScriptName` = 'mob_lavaspawn' WHERE `entry` = 12265;
UPDATE `creature_template` SET `ScriptName` = 'mob_molten_destroyer' WHERE `entry` = 11659;
UPDATE `creature_template` SET `ScriptName` = 'mob_molten_giant' WHERE `entry` = 11658;

/* MOONGLADE */
UPDATE `creature_template` SET `ScriptName` = 'npc_bunthen_plainswind' WHERE `entry` = 11798;
UPDATE `creature_template` SET `ScriptName` = 'npc_great_bear_spirit' WHERE `entry` = 11956;
UPDATE `creature_template` SET `ScriptName` = 'npc_silva_filnaveth' WHERE `entry` = 11800;

/* MULGORE */
UPDATE `creature_template` SET `ScriptName` = 'skorn_whitecloud' WHERE `entry` = 3052;

/* NAGRAND */
UPDATE `creature_template` SET `ScriptName` = 'mob_gurok_the_usurper' WHERE `entry` = 18182;
UPDATE `creature_template` SET `ScriptName` = 'mob_shattered_rumbler' WHERE `entry` = 17157;
UPDATE `creature_template` SET `ScriptName` = 'mobs_kilsorrow_agent' WHERE `entry` IN (17146, 17147, 17148, 18397, 18658);
UPDATE `creature_template` SET `ScriptName` = 'mob_sunspring_villager' WHERE `entry` = 18240;
UPDATE `creature_template` SET `ScriptName` = 'npc_altruis_the_sufferer' WHERE `entry` = 18417;
UPDATE `creature_template` SET `ScriptName` = 'npc_greatmother_geyah' WHERE `entry` = 18141;
UPDATE `creature_template` SET `ScriptName` = 'npc_lantresor_of_the_blade' WHERE `entry` = 18261;
UPDATE `creature_template` SET `ScriptName` = 'npc_creditmarker_visit_with_ancestors' WHERE `entry` IN (18840,18841,18842,18843);

/* NAXXRAMAS */
UPDATE `instance_template` SET `script` = 'instance_naxxramas' WHERE `map` = 533;
UPDATE `creature_template` SET `ScriptName` = 'boss_anubrekhan' WHERE `entry` = 15956;
UPDATE `creature_template` SET `ScriptName` = 'boss_faerlina' WHERE `entry` = 15953;
UPDATE `creature_template` SET `ScriptName` = 'boss_maexxna' WHERE `entry` = 15952;
UPDATE `creature_template` SET `ScriptName` = 'boss_noth' WHERE `entry` = 15954;
UPDATE `creature_template` SET `ScriptName` = 'boss_heigan' WHERE `entry` = 15936;
UPDATE `creature_template` SET `ScriptName` = 'boss_loatheb' WHERE `entry` = 16011;
UPDATE `creature_template` SET `ScriptName` = 'boss_razuvious' WHERE `entry` = 16061;
UPDATE `creature_template` SET `ScriptName` = 'boss_gothik' WHERE `entry` = 16060;
UPDATE `creature_template` SET `ScriptName` = 'boss_highlord_mograine' WHERE `entry` = 16062;
UPDATE `creature_template` SET `ScriptName` = 'boss_thane_korthazz' WHERE `entry` = 16064;
UPDATE `creature_template` SET `ScriptName` = 'boss_sir_zeliek' WHERE `entry` = 16063;
UPDATE `creature_template` SET `ScriptName` = 'boss_lady_blaumeux' WHERE `entry` = 16065;
UPDATE `creature_template` SET `ScriptName` = 'boss_patchwerk' WHERE `entry` = 16028;
UPDATE `creature_template` SET `ScriptName` = 'boss_grobbulus' WHERE `entry` = 15931;
UPDATE `creature_template` SET `ScriptName` = 'boss_gluth' WHERE `entry` = 15932;
UPDATE `creature_template` SET `ScriptName` = 'boss_thaddius' WHERE `entry` = 15928;
UPDATE `creature_template` SET `ScriptName` = 'boss_stalagg' WHERE `entry` = 15929;
UPDATE `creature_template` SET `ScriptName` = 'boss_fugen' WHERE `entry` = 15930;
UPDATE `creature_template` SET `ScriptName` = 'boss_sapphiron' WHERE `entry` = 15989;
UPDATE `creature_template` SET `ScriptName` = 'boss_kelthuzad' WHERE `entry` = 15990;

/* NETHERSTORM */
UPDATE `creature_template` SET `ScriptName` = 'npc_protectorate_nether_drake' WHERE `entry` = 20903;
UPDATE `creature_template` SET `ScriptName` = 'npc_veronia' WHERE `entry` = 20162;

/* ONYXIA'S LAIR */
UPDATE `creature_template` SET `ScriptName` = 'boss_onyxia' WHERE `entry` = 10184;

/* ORGRIMMAR */


/* RAGEFIRE CHASM */


/* RAZORFEN DOWNS */
UPDATE `creature_template` SET `ScriptName` = 'boss_amnennar_the_coldbringer' WHERE `entry` = 7358;

/* REDRIDGE MOUNTAINS */


/* RUINS OF AHN'QIRAJ */
UPDATE `instance_template` SET `script` = 'instance_ruins_of_ahnqiraj' WHERE `map` = 509;

/* SCARLET MONASTERY */
UPDATE `creature_template` SET `ScriptName` = 'boss_arcanist_doan' WHERE `entry` = 6487;
UPDATE `creature_template` SET `ScriptName` = 'boss_azshir_the_sleepless' WHERE `entry` = 6490;
UPDATE `creature_template` SET `ScriptName` = 'boss_bloodmage_thalnos' WHERE `entry` = 4543;
UPDATE `creature_template` SET `ScriptName` = 'boss_herod' WHERE `entry` = 3975;
UPDATE `creature_template` SET `ScriptName` = 'boss_high_inquisitor_fairbanks' WHERE `entry` = 4542;
UPDATE `creature_template` SET `ScriptName` = 'boss_high_inquisitor_whitemane' WHERE `entry` = 3977;
UPDATE `creature_template` SET `ScriptName` = 'boss_houndmaster_loksey' WHERE `entry` = 3974;
UPDATE `creature_template` SET `ScriptName` = 'boss_interrogator_vishas' WHERE `entry` = 3983;
UPDATE `creature_template` SET `ScriptName` = 'boss_scarlet_commander_mograine' WHERE `entry` = 3976;
UPDATE `creature_template` SET `ScriptName` = 'boss_scorn' WHERE `entry` = 14693;
UPDATE `creature_template` SET `ScriptName` = 'scarlet_torturer' WHERE `entry` = 4306;

/* SCHOLOMANCE */
UPDATE `instance_template` SET `script` = 'instance_scholomance' WHERE `map` = 289;
UPDATE `creature_template` SET `ScriptName` = 'boss_darkmaster_gandling' WHERE `entry` = 1853;
UPDATE `creature_template` SET `ScriptName` = 'boss_death_knight_darkreaver' WHERE `entry` = 14516;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_alexei_barov' WHERE `entry` = 10504;
UPDATE `creature_template` SET `ScriptName` = 'boss_instructor_malicia' WHERE `entry` = 10505;
UPDATE `creature_template` SET `ScriptName` = 'boss_boss_ras_frostwhisper' WHERE `entry` = 10508;
UPDATE `creature_template` SET `ScriptName` = 'boss_the_ravenian' WHERE `entry` = 10507;
UPDATE `creature_template` SET `ScriptName` = 'boss_vectus' WHERE `entry` = 10432;
UPDATE `creature_template` SET `ScriptName` = 'boss_illucia_barov' WHERE `entry` = 10502;
UPDATE `creature_template` SET `ScriptName` = 'boss_doctor_theolen_krastinov' WHERE `entry` = 11261;
UPDATE `creature_template` SET `ScriptName` = 'boss_jandice_barov' WHERE `entry` = 10503;
UPDATE `creature_template` SET `ScriptName` = 'boss_lorekeeper_polkelt' WHERE `entry` = 10901;
UPDATE `creature_template` SET `ScriptName` = 'boss_kormok' WHERE `entry` = 16118;
UPDATE `creature_template` SET `ScriptName` = 'mob_illusionofjandicebarov' WHERE `entry` = 11439;

/* SEARING GORGE */
UPDATE `creature_template` SET `ScriptName` = 'npc_kalaran_windblade' WHERE `entry` = 8479;
UPDATE `creature_template` SET `ScriptName` = 'npc_lothos_riftwaker' WHERE `entry` = 14387;
UPDATE `creature_template` SET `ScriptName` = 'npc_zamael_lunthistle' WHERE `entry` = 8436;

/* SHADOWFANG KEEP */


/* SHADOWMOON VALLEY */
UPDATE `creature_template` SET `ScriptName` = 'npc_drake_dealer_hurlunk' WHERE `entry` = 23489;
UPDATE `creature_template` SET `ScriptName` = 'npc_invis_legion_teleporter' WHERE `entry` = 21807;
UPDATE `creature_template` SET `ScriptName` = 'npcs_flanis_swiftwing_and_kagrosh' WHERE `entry` IN (21725, 21727);
UPDATE `creature_template` SET `ScriptName` = 'npc_neltharaku' WHERE `entry` = 21657;
UPDATE `creature_template` SET `ScriptName` = 'npc_oronok_tornheart' WHERE `entry` = 21183;
UPDATE `creature_template` SET `ScriptName` = 'mobs_dragonmaw_orc' WHERE `entry` IN (21717, 21718, 21719, 21720, 22331);
UPDATE `creature_template` SET `ScriptName` = 'mobs_shadowmoon_valley_wildlife' WHERE `entry` IN (21878, 21879);
/* Doomwalker */
UPDATE `creature_template` SET `ScriptName` = 'boss_doomwalker' WHERE `entry` = 17711;

/* SHATTRATH */
UPDATE `creature_template` SET ScriptName = 'npc_shattrathflaskvendors' WHERE `entry` = 23483;
UPDATE `creature_template` SET ScriptName = 'npc_shattrathflaskvendors' WHERE `entry` = 23484;

/* SILITHUS */
UPDATE `creature_template` SET `ScriptName` = 'npcs_rutgar_and_frankal' WHERE `entry` IN (15170, 15171);

/* SILVERMOON */
UPDATE `creature_template` SET `ScriptName` = 'blood_knight_stillblade' WHERE `entry` = 17768;

/* SILVERPINE FOREST */
UPDATE `creature_template` SET `ScriptName` = 'npc_astor_hadren' WHERE `entry` = 6497;

/* STOCKADES */

/* STONETALON MOUNTAINS */
UPDATE `creature_template` SET `ScriptName` = 'npc_braug_dimspirit' WHERE `entry` = 4489;

/* STORMWIND CITY */
UPDATE `creature_template` SET `ScriptName` = 'npc_archmage_malin' WHERE `entry` = 2708;
UPDATE `creature_template` SET `ScriptName` = 'npc_bartleby' WHERE `entry` = 6090;
UPDATE `creature_template` SET `ScriptName` = 'npc_dashel_stonefist' WHERE `entry` = 4961;
UPDATE `creature_template` SET `ScriptName` = 'npc_general_marcus_jonathan' WHERE `entry` = 466;
UPDATE `creature_template` SET `ScriptName` = 'npc_lady_katrana_prestor' WHERE `entry` = 1749;

/* STRANGLETHORN VALE */
UPDATE `creature_template` SET `ScriptName` = 'mob_yenniku' WHERE `entry` = 2530;

/* STRATHOLME */
UPDATE `instance_template` SET `script` = 'instance_stratholme' WHERE `map` = 329;
UPDATE `creature_template` SET `ScriptName` = 'boss_dathrohan_balnazzar' WHERE `entry` = 10812;
UPDATE `creature_template` SET `ScriptName` = 'boss_fras_siabi' WHERE `entry` = 11058;
UPDATE `creature_template` SET `ScriptName` = 'boss_skul' WHERE `entry` = 10393;
UPDATE `creature_template` SET `ScriptName` = 'boss_hearthsinger_forresten' WHERE `entry` = 10558;
UPDATE `creature_template` SET `ScriptName` = 'boss_magistrate_barthilas' WHERE `entry` = 10435;
UPDATE `creature_template` SET `ScriptName` = 'boss_maleki_the_pallid' WHERE `entry` = 10438;
UPDATE `creature_template` SET `ScriptName` = 'boss_nerubenkan' WHERE `entry` = 10437;
UPDATE `creature_template` SET `ScriptName` = 'boss_cannon_master_willey' WHERE `entry` = 10997;
UPDATE `creature_template` SET `ScriptName` = 'boss_the_unforgiven' WHERE `entry` = 10516;
UPDATE `creature_template` SET `ScriptName` = 'boss_baroness_anastari' WHERE `entry` = 10436;
UPDATE `creature_template` SET `ScriptName` = 'boss_archivist_galford' WHERE `entry` = 10811;
UPDATE `creature_template` SET `ScriptName` = 'boss_crimson_hammersmith' WHERE `entry` = 11120;
UPDATE `creature_template` SET `ScriptName` = 'boss_ramstein_the_gorger' WHERE `entry` = 10439;
UPDATE `creature_template` SET `ScriptName` = 'boss_malor_the_zealous' WHERE `entry` = 11032;
UPDATE `creature_template` SET `ScriptName` = 'boss_black_guard_swordsmith' WHERE `entry` = 11121;
UPDATE `creature_template` SET `ScriptName` = 'boss_timmy_the_cruel' WHERE `entry` = 10808;
UPDATE `creature_template` SET `ScriptName` = 'boss_silver_hand_bosses' WHERE `entry` IN (17910, 17911, 17912, 17913, 17914);
UPDATE `creature_template` SET `ScriptName` = 'boss_stonespine' WHERE `entry` = 10809;
UPDATE `creature_template` SET `ScriptName` = 'boss_postmaster_malown' WHERE `entry` = 11143;
UPDATE `creature_template` SET `ScriptName` = 'boss_baron_rivendare' WHERE `entry` = 10440;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_guardsman' WHERE `entry` = 10418;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_conjuror' WHERE `entry` = 10419;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_initiate' WHERE `entry` = 10420;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_defender' WHERE `entry` = 10421;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_sorcerer' WHERE `entry` = 10422;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_priest' WHERE `entry` = 10423;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_gallant' WHERE `entry` = 10424;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_battle_mage' WHERE `entry` = 10425;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_inquisitor' WHERE `entry` = 10426;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_monk' WHERE `entry` = 11043;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_rifleman' WHERE `entry` = 11054;
UPDATE `creature_template` SET `ScriptName` = 'mob_bile_spewer' WHERE `entry` = 10416;
UPDATE `creature_template` SET `ScriptName` = 'mob_bile_slime' WHERE `entry` = 10697;
UPDATE `creature_template` SET `ScriptName` = 'mob_black_guard_sentry' WHERE `entry` = 10394;
UPDATE `creature_template` SET `ScriptName` = 'mob_ghoul_ravener' WHERE `entry` = 10406;
UPDATE `creature_template` SET `ScriptName` = 'mob_fleshflayer_ghoul' WHERE `entry` = 10407;
UPDATE `creature_template` SET `ScriptName` = 'mob_plague_ghoul' WHERE `entry` = 10405;
UPDATE `creature_template` SET `ScriptName` = 'mob_eye_of_naxxramas' WHERE `entry` = 10411;
UPDATE `creature_template` SET `ScriptName` = 'mob_rockwing_screecher' WHERE `entry` = 10409;
UPDATE `creature_template` SET `ScriptName` = 'mob_rockwing_gargoyle' WHERE `entry` = 10408;
UPDATE `creature_template` SET `ScriptName` = 'mob_venom_belcher' WHERE `entry` = 10417;
UPDATE `creature_template` SET `ScriptName` = 'mob_patchwork_horror' WHERE `entry` = 10414;
UPDATE `creature_template` SET `ScriptName` = 'mob_wailing_banshee' WHERE `entry` = 10464;
UPDATE `creature_template` SET `ScriptName` = 'mob_shrieking_banshee' WHERE `entry` = 10463;
UPDATE `creature_template` SET `ScriptName` = 'mob_vengeful_phantom' WHERE `entry` = 10387;
UPDATE `creature_template` SET `ScriptName` = 'mob_wrath_phantom' WHERE `entry` = 10389;
UPDATE `creature_template` SET `ScriptName` = 'mob_spiteful_phantom' WHERE `entry` = 10388;
UPDATE `creature_template` SET `ScriptName` = 'mob_crypt_beast' WHERE `entry` = 10413;
UPDATE `creature_template` SET `ScriptName` = 'mob_crypt_crawler' WHERE `entry` = 10412;
UPDATE `creature_template` SET `ScriptName` = 'mob_undead_scarab' WHERE `entry` = 10876;
UPDATE `creature_template` SET `ScriptName` = 'mob_skeletal_guardian' WHERE `entry` = 10390;
UPDATE `creature_template` SET `ScriptName` = 'mob_skeletal_berserker' WHERE `entry` = 10391;
UPDATE `creature_template` SET `ScriptName` = 'mob_mangled_cadaver' WHERE `entry` = 10382;
UPDATE `creature_template` SET `ScriptName` = 'mob_ravaged_cadaver' WHERE `entry` = 10381;
UPDATE `creature_template` SET `ScriptName` = 'mob_broken_cadaver' WHERE `entry` = 10383;
UPDATE `creature_template` SET `ScriptName` = 'mob_mindless_skeleton' WHERE `entry` = 11197;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_acolyte' WHERE `entry` = 10399;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_necromancer' WHERE `entry` = 10400;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_shadowcaster' WHERE `entry` = 10398;
UPDATE `creature_template` SET `ScriptName` = 'mobs_spectral_ghostly_citizen' WHERE `entry` IN (10384, 10385);
UPDATE `creature_template` SET `ScriptName` = 'mob_restless_soul' WHERE `entry` = 11122;
UPDATE `creature_template` SET `ScriptName` = 'mob_freed_soul' WHERE `entry` = 11136;

/* SUNKEN TEMPLE */


/* SWAMP OF SORROWS */


/* TANARIS */
UPDATE `creature_template` SET `ScriptName` = 'mob_aquementas' WHERE `entry` = 9453;
UPDATE `creature_template` SET `ScriptName` = 'npc_marin_noggenfogger' WHERE `entry` = 7564;
UPDATE `creature_template` SET `ScriptName` = 'npc_steward_of_time' WHERE `entry` = 20142;
UPDATE `creature_template` SET `ScriptName` = 'npc_stone_watcher_of_norgannon' WHERE `entry` = 7918;

/* TELDRASSIL */


/*  */
/* TEMPEST KEEP */
/*  */

/* THE MECHANAR */
UPDATE `creature_template` SET `ScriptName` = 'boss_gatewatcher_iron_hand' WHERE `entry` = 19710;
UPDATE `creature_template` SET `ScriptName` = 'boss_nethermancer_sepethrea' WHERE `entry` = 19221;
UPDATE `creature_template` SET `ScriptName` = 'mob_ragin_flames' WHERE `entry` = 20481;

/* THE BOTANICA */

/* THE ARCATRAZ */
UPDATE `instance_template` SET `script` = 'instance_arcatraz' WHERE `map` = 552;

/* THE EYE */
UPDATE `instance_template` SET `script` = 'instance_the_eye' WHERE `map` = 550;
/* The Eye Trash Mobs */
UPDATE `creature_template` SET `ScriptName` = 'mob_the_eye' WHERE `entry` IN (20031,20032,20033,20034,20035,20039,20041,20043,20046,20052);
UPDATE `creature_template` SET `ScriptName` = 'mob_crystalcore_devastator' WHERE `entry` = 20040;
/* Void Reaver event */
UPDATE `creature_template` SET `ScriptName` = 'boss_void_reaver' WHERE `entry` = 19516;
UPDATE `creature_template` SET `ScriptName` = 'mob_arcane_orb_target' WHERE `entry` = 19577;
/* Kael'thas event */
UPDATE `creature_template` SET `ScriptName` = 'boss_kaelthas' WHERE `entry` = 19622;
UPDATE `creature_template` SET `ScriptName` = 'boss_thaladred_the_darkener' WHERE `entry` = 20064;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_sanguinar' WHERE `entry` = 20060;
UPDATE `creature_template` SET `ScriptName` = 'boss_grand_astromancer_capernian' WHERE `entry` = 20062;
UPDATE `creature_template` SET `ScriptName` = 'boss_master_engineer_telonicus' WHERE `entry` = 20063;
UPDATE `creature_template` SET `ScriptName` = 'mob_phoenix' WHERE `entry` = 21362;
UPDATE `creature_template` SET `ScriptName` = 'mob_nether_vapor' WHERE `entry` = 21002;
UPDATE `creature_template` SET `ScriptName` = 'mob_kael_flamestrike' WHERE `entry` = 21369;

/* TEMPLE OF AHN'QIRAJ */
UPDATE `instance_template` SET `script` = 'instance_temple_of_ahnqiraj' WHERE `map` = 531;
/* AQ40 Bosses */
UPDATE `creature_template` SET `ScriptName` = 'boss_skeram' WHERE `entry` = 15263;
UPDATE `creature_template` SET `ScriptName` = 'boss_vem' WHERE `entry` = 15544;
UPDATE `creature_template` SET `ScriptName` = 'boss_yauj' WHERE `entry` = 15543;
UPDATE `creature_template` SET `ScriptName` = 'mob_yauj_brood' WHERE `entry` = 15621;
UPDATE `creature_template` SET `ScriptName` = 'boss_kri' WHERE `entry` = 15511;
UPDATE `creature_template` SET `ScriptName` = 'boss_sartura' WHERE `entry` = 15516;
UPDATE `creature_template` SET `ScriptName` = 'mob_sartura_royal_guard' WHERE `entry` = 15984;
UPDATE `creature_template` SET `ScriptName` = 'boss_fankriss' WHERE `entry` = 15510;
UPDATE `creature_template` SET `ScriptName` = 'mob_spawn_of_fankriss' WHERE `entry` = 15630;
UPDATE `creature_template` SET `ScriptName` = 'boss_viscidus' WHERE `entry` = 15299;
UPDATE `creature_template` SET `ScriptName` = 'boss_glob_of_viscidus' WHERE `entry` = 15667;
UPDATE `creature_template` SET `ScriptName` = 'boss_huhuran' WHERE `entry` = 15509;
UPDATE `creature_template` SET `ScriptName` = 'boss_veklor' WHERE `entry` = 15276;
UPDATE `creature_template` SET `ScriptName` = 'boss_veknilash' WHERE `entry` = 15275;
UPDATE `creature_template` SET `ScriptName` = 'boss_ouro' WHERE `entry` = 15517;
UPDATE `creature_template` SET `ScriptName` = 'boss_eye_of_cthun' WHERE `entry` = 15589;
UPDATE `creature_template` SET `ScriptName` = 'mob_claw_tentacle' WHERE `entry` = 15725;
UPDATE `creature_template` SET `ScriptName` = 'mob_eye_tentacle' WHERE `entry` = 15726;
UPDATE `creature_template` SET `ScriptName` = 'boss_cthun' WHERE `entry` = 15727;
UPDATE `creature_template` SET `ScriptName` = 'mob_giant_claw_tentacle' WHERE `entry` = 15728;
UPDATE `creature_template` SET `ScriptName` = 'mob_giant_eye_tentacle' WHERE `entry` = 15334;
UPDATE `creature_template` SET `ScriptName` = 'mob_giant_flesh_tentacle' WHERE `entry` = 15802;
UPDATE `creature_template` SET `ScriptName` = 'mob_spawn_of_fankriss' WHERE `entry` = 15630;


/* TEROKKAR FOREST */
UPDATE `creature_template` SET `ScriptName` = 'mobs_gordunni_ogre' WHERE `entry` IN (22143, 22144, 22148, 23022);
UPDATE `creature_template` SET `ScriptName` = 'mob_infested_root_walker' WHERE `entry` = 22095;
UPDATE `creature_template` SET `ScriptName` = 'mob_netherweb_victim' WHERE `entry` = 22355;
UPDATE `creature_template` SET `ScriptName` = 'mob_rotting_forest_rager' WHERE `entry` = 22307;
UPDATE `creature_template` SET `ScriptName` = 'npc_skyguard_handler_irena' WHERE `entry` = 23413;

/* THOUSAND NEEDLES */


/* THUNDER BLUFF */
UPDATE `creature_template` SET `ScriptName` = 'npc_cairne_bloodhoof' WHERE `entry` = 3057;

/* TIRISFAL GLADES */


/* ULDAMAN */
UPDATE `creature_template` SET `ScriptName` = 'boss_ironaya' WHERE `entry` = 7228;
UPDATE `creature_template` SET `ScriptName` = 'mob_uldaman' WHERE `entry` IN (4847,4852,4853,4854,4860,4861,6910,7011,7012,7022,7030,7078,7291);
UPDATE `creature_template` SET `ScriptName` = 'mob_jadespine_basilisk' WHERE `entry` = 4863;
UPDATE `creature_template` SET `ScriptName` = 'npc_lore_keeper_of_norgannon' WHERE `entry` = 7172;

/* UN'GORO CRATER */


/* UNDERCITY */
UPDATE `creature_template` SET `ScriptName` = 'npc_parqual_fintallas' WHERE `entry` = 4488;


/* WAILING CAVERNS */
UPDATE `creature_template` SET `ScriptName` = 'boss_wailing_caverns' WHERE `entry` =3654;

/* WESTERN PLAGUELANDS */
UPDATE `creature_template` SET `ScriptName` = 'npcs_dithers_and_arbington' WHERE `entry` IN (11056, 11057);

/* WESTFALL */


/* WETLANDS */


/* WINTERSPRING */
UPDATE `creature_template` SET `ScriptName` = 'npc_lorax' WHERE `entry` = 10918;
UPDATE `creature_template` SET `ScriptName` = 'npc_rivern_frostwind' WHERE `entry` = 10618;
UPDATE `creature_template` SET `ScriptName` = 'npc_witch_doctor_mauari' WHERE `entry` = 10307;

/* ZANGARMARSH */
UPDATE `creature_template` SET `ScriptName` = 'npcs_ashyen_and_keleth' WHERE `entry` IN (17900, 17901);
UPDATE `creature_template` SET `ScriptName` = 'npc_elder_kuruti' WHERE `entry` = 18197;
UPDATE `creature_template` SET `ScriptName` = 'npc_mortog_steamhead' WHERE `entry` = 23373;

/* ZUL'FARRAK */


/* ZUL'GURUB */
UPDATE `instance_template` SET `script` = 'instance_zulgurub' WHERE `map` = 309;
UPDATE `creature_template` SET `ScriptName` = 'boss_jeklik' WHERE `entry` = 14517;
UPDATE `creature_template` SET `ScriptName` = 'boss_venoxis' WHERE `entry` = 14507;
UPDATE `creature_template` SET `ScriptName` = 'boss_marli' WHERE `entry` = 14510;
UPDATE `creature_template` SET `ScriptName` = 'boss_mandokir' WHERE `entry` = 11382;
UPDATE `creature_template` SET `ScriptName` = 'boss_gahzranka' WHERE `entry` = 15114;
UPDATE `creature_template` SET `ScriptName` = 'boss_jindo' WHERE `entry` = 11380;
UPDATE `creature_template` SET `ScriptName` = 'boss_hakkar' WHERE `entry` = 14834;
UPDATE `creature_template` SET `ScriptName` = 'boss_thekal' WHERE `entry` = 14509;
UPDATE `creature_template` SET `ScriptName` = 'boss_arlokk' WHERE `entry` = 14515;
UPDATE `creature_template` SET `ScriptName` = 'mob_zealot_lorkhan' WHERE `entry` = 11347;
UPDATE `creature_template` SET `ScriptName` = 'mob_zealot_zath' WHERE `entry` = 11348;
UPDATE `creature_template` SET `ScriptName` = 'mob_healing_ward' WHERE `entry` = 14987;
UPDATE `creature_template` SET `ScriptName` = 'mob_spawn_of_marli' WHERE `entry` = 15041;
UPDATE `creature_template` SET `ScriptName` = 'mob_batrider' WHERE `entry` = 14965;
UPDATE `creature_template` SET `ScriptName` = 'mob_shade_of_jindo' WHERE `entry` = 14986;
UPDATE `creature_template` SET `ScriptName` = 'boss_grilek' WHERE `entry` = 15082;
UPDATE `creature_template` SET `ScriptName` = 'boss_hazzarah' WHERE `entry` = 15083;
UPDATE `creature_template` SET `ScriptName` = 'boss_renataki' WHERE `entry` = 15084;
UPDATE `creature_template` SET `ScriptName` = 'boss_wushoolay' WHERE `entry` = 15085;
UPDATE `creature_template` SET `Scriptname` = 'mobs_zulgurub' WHERE `entry` IN  (11340 ,11352 , 11350, 11830, 11372, 11351, 14750, 14883);


/* EOF */
