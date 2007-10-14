/*  */

/* UNKNOWN LOCATION */


/* WORLD BOSS */
UPDATE `creature_template` SET `ScriptName` = 'boss_ysondre' WHERE `entry` = 14887;
UPDATE `creature_template` SET `ScriptName` = 'boss_emeriss' WHERE `entry` = 14889;
UPDATE `creature_template` SET `ScriptName` = 'boss_taerar' WHERE `entry` = 14890;
UPDATE `creature_template` SET `ScriptName` = 'boss_shade_of_taerar' WHERE `entry` = 15302;
UPDATE `creature_template` SET `ScriptName` = 'boss_teremus_the_devourer' WHERE `entry` = 7846;
UPDATE `creature_template` SET `ScriptName` = 'boss_kruul' WHERE `entry` = 18338;
UPDATE `creature_template` SET `ScriptName` = 'boss_azuregos' WHERE `entry` = 6109;
UPDATE `creature_template` SET `ScriptName` = 'mob_dementeddruids' WHERE `entry` = 15260;

/* WORLD CREATURE  These are creatures to be found in more than one spesific zone */
UPDATE `creature_template` SET `ScriptName` = 'lumberjack' WHERE `entry` IN (11328, 11260);
UPDATE `creature_template` SET `ScriptName` = 'miner' WHERE `entry` IN (80, 1236, 3578, 40, 1358, 1360, 5996, 1426, 1094, 674);
/* Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (14881);

/* GO */
UPDATE `gameobject_template` SET `ScriptName`='go_orb_of_command' WHERE `entry`='179879';


/* GUARD */
UPDATE `creature_template` SET `ScriptName` = 'guard_orgrimmar' WHERE `entry` = 3296;
UPDATE `creature_template` SET `ScriptName` = 'guard_stormwind' WHERE `entry` IN (68, 1976);
UPDATE `creature_template` SET `ScriptName` = 'guard_contested' WHERE `entry` IN (9460, 4624, 3502, 11190, 15184);
UPDATE `creature_template` SET `ScriptName` = 'guard_elwynnforest' WHERE `entry` = 1423;
UPDATE `creature_template` SET `ScriptName` = 'guard_darnassus' WHERE `entry` = 4262;
UPDATE `creature_template` SET `ScriptName` = 'guard_teldrassil' WHERE `entry` = 3571;
UPDATE `creature_template` SET `ScriptName` = 'guard_ironforge' WHERE `entry` = 5595;
UPDATE `creature_template` SET `ScriptName` = 'guard_dunmorogh' WHERE `entry` IN (727, 13076);
UPDATE `creature_template` SET `ScriptName` = 'guard_undercity' WHERE `entry` = 5624;
UPDATE `creature_template` SET `ScriptName` = 'guard_bluffwatcher' WHERE `entry` = 3084;
UPDATE `creature_template` SET `ScriptName` = 'guard_razorhill' WHERE `entry` = 5953;
UPDATE `creature_template` SET `ScriptName` = 'guard_mulgore' WHERE `entry` IN (3210, 3211, 3212, 3213, 3214, 3215, 3217, 3218, 3219, 3220, 3221, 3222, 3223, 3224);
UPDATE `creature_template` SET `ScriptName` = 'guard_dunmorogh' WHERE `entry` IN (727, 13076);
UPDATE `creature_template` SET `ScriptName` = 'guard_tirisfal' WHERE `entry` IN (1735, 1738, 2210, 1736, 1739, 1737, 1744, 1745, 5725, 1743, 2209, 1746, 1742);
UPDATE `creature_template` SET `scriptName` = 'guard_silvermoon' WHERE `entry` = 16222;
UPDATE `creature_template` SET `scriptName` = 'guard_exodar' WHERE `entry` = 16733;
UPDATE `creature_template` SET `ScriptName` = 'guard_shattrath' WHERE `entry` = 19687;

/* HONOR */
UPDATE `creature_template` SET `ScriptName` = 'honor_vendor' WHERE `entry` IN (40100,40101,40102,40103,40104,40105,40106,40107,40108,40109,40110,40111,40112,40113);
UPDATE `creature_template` SET `ScriptName` = 'honor_vendor' WHERE `entry` IN (40200,40201,40202,40203,40204,40205,40206,40207,40208,40209,40210,40211,40212,40213);

/* ITEM */
UPDATE `item_template` SET `ScriptName` = 'purification_mixture' WHERE `entry`= 23268;
UPDATE `item_template` SET `ScriptName` = 'nether_wraith_beacon' WHERE `entry` = 31742;
UPDATE `item_template` SET `ScriptName` = 'area_52_special' WHERE `entry` = 28132;

/* NPC */
UPDATE `creature_template` SET `ScriptName` = 'marshal_mcbride' WHERE `entry` = 197;
UPDATE `creature_template` SET `ScriptName` = 'silva_filnaveth' WHERE `entry` = 11800;
UPDATE `creature_template` SET `ScriptName` = 'skorn_whitecloud' WHERE `entry` = 3052;
UPDATE `creature_template` SET `ScriptName` = 'bunthen_plainswind' WHERE `entry` = 11798;
UPDATE `creature_template` SET `ScriptName` = 'npc_guardian' WHERE `entry`='5764';

/* SERVERS */
UPDATE `creature_template` SET `ScriptName` = 'battlemaster' WHERE `entry` IN (12198,14982,857,12197,7427,347,15007,2804);
UPDATE `creature_template` SET `ScriptName` = 'travelmaster' WHERE `entry` IN (9564,12136,3149,12137,9566,3150,9559,9558);

/*  */
/* ZONE */
/* */

/* ALTERAC MOUNTAINS */
/* Crushridge Mage, Argus Shadow Mage, Dalaran Summoner, Archmage Ansirem Runeweaver <Kirin Tor> */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (2255, 2318, 2358, 2543);

/* ALTERAC VALLEY */
/* Wildpaw Mystic */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11838);

/* ARATHI HIGHLANDS */
/* Feeboz, Boulderfist Shaman, Boulderfist Magus, Syndicate Magus, Dark Iron Shadowcaster, Witherbark Axe Thrower, Plains Creeper, Giant Plains Creeper */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4063, 2570, 2567, 2591, 2577, 2554, 2563, 2565);

/* ASHENVALE */
/* Shadethicket Raincaller, Forsaken Seeker, Dark Strand Cultist, Dark Strand Adept */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (3783, 3732, 3725, 3728);


/*  */
/* AUCHINDOUN */
/*  */

/* BONE WASTES (area around entrance to the four instances) */

/* MANA TOMBS */

/* AUCHENAI CRYPTS */

/* SETHEKK HALLS */

/* SHADOW LABYRINTH */



/* AZSHARA */
/* Draconic Magelord, Draconic Mageweaver, Archmage Xylem */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6129, 6131, 8379);
/* Spitelashes for polymorph quest */
UPDATE `creature_template` SET `ScriptName` = 'mobs_spitelashes' WHERE entry IN (6190,6193,6194,6195,6196,7885,7886,12204,12205);

/* AZUREMYST ISLE */
UPDATE `creature_template` set `ScriptName` = 'injured_draenei' where `entry` = 16971;

/* BADLANDS */
/* Dustbelcher Ogre Mage, Dustbelcher Mystic, Shadowmage Vivian Lagrave <Kargath Expeditionary Force> */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (2720, 2907, 9078);

/* BARRENS */
/* Kolkar Stormer, Razormane Geomancer, Razormane Seer, Razormane Mystic, Razormane Hunter, Razormane Warfrenzy, Razormane Water Seeker, Silithid Creeper, Elder Mystic Razorsnout, Horde Axe Thrower */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (3273, 3269, 3458, 3271, 3265, 3459, 3267, 3250, 3270, 9458);
/* Beaten corpse */
UPDATE creature_template SET ScriptName='npc_beaten_corpse' WHERE entry=10668;

/* BLACK TEMPLE */
UPDATE `instance_template` SET `script` = 'instance_black_temple' WHERE `map`= 564;
/* BT Trash Mobs */
UPDATE `creature_template` SET `scriptname`='mob_blacktemple' WHERE `entry` IN (22844,22845,22846,22849,22853,22855,22869,22873,22875,22876,22877,22878,22880,22881,22882,22883,22884,22945,22953,22954,23337,23339);

/* BLACKFATHOM DEPTHS */
/* Blindlight Oracle, Blindlight Muckdweller, Blindlight Murloc, Twilight Shadowmage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4820, 4819, 4818, 4813);

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

/* Hedrum the Creeper, Dark Keeper Zimrel */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (9032, 9441);
/* Anvilrage military */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (8890, 8891, 8892, 8893, 8894, 8895, 8898);
/* Cave Creeper */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (8933);

/* BLACKROCK SPIRE */
UPDATE `creature_template` SET `ScriptName` = 'mob_chromatic_elite_guard' WHERE `entry` = 10814;
/* Blackhand Dreadweaver,Blackhand Summoner,Blackhand Veteran,Blackhand Elite,Blackhand Assassin,Blackhand Iron Guard,Chromatic Whelp,Chromatic Dragonspawn,Rookery Whelp,Rookery Guardian,Rage Talon Captain,Rage Talon Fire Tongue,Rage Talon Flamescale */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (9817, 9818, 9819, 10317, 10318, 10319, 10442, 10447, 10161, 10258, 10371, 10372, 10083);
/* Spirestone Battle Mage, Spirestone Mystic, Smolderthorn Mystic, Smolderthorn Axe Thrower, Bloodaxe Summoner <Bloodaxe Legion>, Spire Spider, Spire Spiderling */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (9197, 9198, 9239, 9267, 9717, 10374, 10375);

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
/* Jed Runewatcher,Solakar Flamewreath,Goraluk Anvilcrack */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry`IN (10509,10264,10899);

/* BLACKWING LAIR */
UPDATE `instance_template` SET `script` = 'instance_blackwing_lair' WHERE `map`= 469;
UPDATE `creature_template` SET `ScriptName` = 'boss_razorgore' WHERE `entry` = 12435;
UPDATE `creature_template` SET `ScriptName` = 'boss_vaelastrasz' WHERE `entry` = 13020;
UPDATE `creature_template` SET `ScriptName` = 'boss_broodlord' WHERE `entry` = 12017;
UPDATE `creature_template` SET `ScriptName` = 'boss_firemaw' WHERE `entry` = 11983;
UPDATE `creature_template` SET `ScriptName` = 'boss_ebonroc' WHERE `entry` = 14601;
UPDATE `creature_template` SET `ScriptName` = 'boss_flamegor' WHERE `entry` = 11981;
UPDATE `creature_template` SET `ScriptName` = 'boss_chromaggus' WHERE `entry` = 14020;
UPDATE `creature_template` SET `ScriptName` = 'boss_victor_nefarius' WHERE `entry` = 10162;
UPDATE `creature_template` SET `ScriptName` = 'boss_nefarian' WHERE `entry` = 11583;
/* Blackwing Mage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (12420);

/* BLASTED LANDS */
/* Dreadmaul Ogre Mage, Bloodmage Drazial, Bloodmage Lynnore, Archmage Allistarj */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (5975, 7505, 7506, 7666);

/* BURNING STEPPES */
/* Blackrock Sorcerer, Blackrock Warlock, Scalding Broodling, Thaurissan Agent, Firegut Ogre Mage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7026, 7028, 7048, 7038, 7034);


/*  */
/* CAVERNS OF TIME */
/*  */

/* MT. HYJAL */
UPDATE `instance_template` SET `script` = 'instance_hyjal' WHERE `map`= 534;

/* OLD HILLSBRAD */

/* THE DARK PORTAL */



/*  */
/* COILFANG RESERVOIR */
/*  */

/* THE SLAVE PENS */
UPDATE creature_template SET ScriptName='boss_rokmar_the_crackler' WHERE entry=17991;
UPDATE creature_template SET ScriptName='boss_warlord_kalithresh' WHERE entry=17798;

/* THE UNDERBOG */
UPDATE `creature_template` SET `ScriptName` = 'boss_hungarfen' WHERE `entry` = 17770;
UPDATE `creature_template` SET `ScriptName` = 'mob_underbog_mushroom' WHERE `entry` = 17990;

/* THE STEAMVAULT */

/* SERPENTSHRINE CAVERN */
UPDATE `instance_template` SET `script` = 'instance_serpent_shrine' WHERE `map`= 548;



/* DARKSHORE */
UPDATE `creature_template` SET `ScriptName` = 'mob_lunaclaw' WHERE `entry` = 12138;
/* Delmanis the Hated, Greymist Oracle,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (3662, 2207);

/* DEADMINES */
/* Defias Squallshaper, Defias Magician, Defias Conjurer, Defias Overseer, Defias Watchman, Defias Wizard, Defias Evoker, Defias Pirate, Defias Taskmaster, Defias Miner, Defias Strip Miner, Defias Henchman,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1732, 1726, 619, 634, 1725, 4418, 1729, 657, 4417, 598, 4416, 594);

/* DEADWIND PASS */
/* Deadwind Ogre Mage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7379);

/* DESOLACE */
/* Burning Blade Shadowmage, Burning Blade Summoner, Mage Hunter */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4667, 4668, 4681);

/* DIRE MAUL */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11492, 14325, 14324, 14321, 14326, 14323, 13280, 14122, 11488, 11496, 14396, 11501, 14327, 14506, 11487, 11486, 14354, 11498, 11489, 11490, 11444, 11450, 11441, 11457, 13197);
/* Gordok Mage-Lord, Highborne Summoner */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11444, 11466);

/* DUN MOROGH */
UPDATE `creature_template` SET `ScriptName` = 'npc_narm_faulk' WHERE `entry`= 6177;
/* Frostmane Novice, Frostmane Seer, Rockjaw Skullthumper, Rockjaw Bonesnapper, Rockjaw Ambusher, Rockjaw Backbreaker,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (946, 1397, 1115, 1117, 1116, 1118);

/* DUROTAR */
/* Razormane Scout, Razormane Dustrunner, Razormane Quilboar, Razormane Battleguard, Vile Familiar */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (3112, 3113, 3111, 3114, 3101);

/* DUSKWOOD */
/* Eliza, Nightbane Shadow Weaver, Skeletal Mage, Defias Night Runner, Defias Night Blade, Skeletal Fiend, Skeletal Healer, Skeletal Mage, Skeletal Raider, Skeletal Warder, Skeletal Warrior, Venom Web Spider, Pygmy Venom Web Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (314, 533, 203, 215, 909, 531, 787, 203, 1110, 785, 48, 217, 539);

/* DUSTWALLOW MARSH */
/* Mirefin Murloc, Darkmist Spider, Withervine Creeper, Darkfang Creeper, Darkfang Spider, Giant Darkfang Spider, Archmage Tervosh */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4359, 4376, 4382, 4412, 4413, 4415, 4967);
/* Lady Jaina Proudmoore */
UPDATE creature_template SET ScriptName='npc_lady_jaina_proudmoore' WHERE entry=4968;

/* EASTERN PLAGUELANDS */
UPDATE `creature_template` SET `ScriptName` = 'mobs_ghoul_flayer' WHERE `entry` IN (8530, 8531, 8532);
UPDATE `creature_template` SET `ScriptName` = 'npc_darrowshire_spirit' WHERE `entry` = 11064;
/* Crimson Bodyguard, Crimson Courier, Crypt Walker, Hate Shrieker, Cursed Mage, Shadowmage, Dark Summoner, Scarlet Archmage, Archmage Angela Dosantos */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (13118, 12337, 8556, 8541, 8524, 8550, 8551, 9451, 16116);

/* ELWYNN FOREST */
/* Hogger, Defias Rogue Wizard, Defias Enchanter, Defias Bodyguard, Defias Bandit, Defias Cutpurse, Murloc Lurker, Murloc Forager, Forest Spider, Mine Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (448, 474, 910, 6866, 116, 94, 732, 46, 30, 43);
UPDATE `creature_template` SET `ScriptName` = 'npc_henze_faulk' WHERE `entry`= 6172;

/* EVERSONG WOODS */
UPDATE `creature_template` SET `ScriptName` = 'mobs_mana_tapped' WHERE `entry` IN (15273,15274,15294,15298,15367);

/* FELWOOD */
/* Timbermaw Mystic */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11552);

/* FERALAS */
/* Gordunni Ogre Mage, Gordunni Mage-Lord, Woodpaw Mystic, Gordok Ogre-Mage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (5237, 5239, 5254, 11443);

/* GHOSTLANDS */


/* GNOMEREGAN */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6206, 6208, 6209, 6215, 6219, 6229, 6228, 6235, 7361, 6407, 6220, 6218, 7603, 6223, 6222, 6234, 6233, 6226, 6227, 6225, 6230, 7079);

/* GRUUL'S LAIR */
UPDATE `instance_template` SET `script` = 'instance_gruuls_lair' WHERE `map`= 565;
/* Gruul the Dragonkiller */
UPDATE `creature_template` SET `ScriptName` = 'boss_gruul' WHERE `entry`= 19044;


/*  */
/* HELLFIRE CITADEL */
/*  */

/* BLOOD FURNACE */
/* The Maker */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (17381);
/* Laughing Skull Legionnaire, Laughing Skull Warden, Laughing Skull Rogue, Hellfire Imp, Shadowmoon Channeler, Fel Orc Neophyte, Shadowmoon Technician, Shadowmoon Adept, Hellfire Familiar, Felguard Brute, Shadowmoon Summoner, Shadowmoon Warlock, Keli'dan the Breaker, Broggok */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (17626, 17624, 17491, 17477, 17653, 17429, 17414, 17397, 19016, 18894, 17395, 17371, 17377, 17380);

/* HELLFIRE RAMPARTS */
/* Vazruden, Omor the Unscarred, Watchkeeper Gargolmar */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (17537, 17308, 17306);
UPDATE `creature_template` SET `ScriptName` = 'boss_omor_the_unscarred' WHERE `entry` = 17308;
UPDATE `creature_template` SET `ScriptName` = 'boss_watchkeeper_gargolmar' WHERE `entry` = 17306;
/* Hellfire Watcher <Watchkeeper's Subordinate>, Bonechewer Hungerer, Bonechewer Beastmaster, Bonechewer Ravener, Bonechewer Ripper, Bonechewer Destroyer, Bleeding Hollow Archer, Bleeding Hollow Darkcaster, Hellfire Sentry <Herald's Sentry>, Bleeding Hollow Scryer */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (17309, 17259, 17455, 17264, 17281, 17271, 17270, 17269, 17517, 17478);

/* SHATTERED HALLS */
/* Warchief Kargath Bladefist */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (16808);
/* Shattered Hand Centurion, Shadowmoon Darkcaster, Shattered Hand Champion, Sharpshooter Guard, Shattered Hand Assassin, Warbringer O'mrogg, Grand Warlock Nethekurse, Shattered Hand Sharpshooter, Shattered Hand Legionnaire, Shattered Hand Reaver, Shattered Hand Brawler */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (17465, 17694, 17671, 17622, 17695, 16809, 16807, 16704, 16700, 16699, 16593);

/* MAGTHERIDON'S LAIR */
UPDATE `instance_template` SET `script` = 'instance_magtheridons_lair' WHERE `map`= 544;



/* HELLFIRE PENINSULA */
UPDATE `creature_template` SET `ScriptName` = 'boss_doomlord_kazzak' WHERE `entry` = 18728;
UPDATE `creature_template` SET `ScriptName` = 'mobs_bonechewer_orc' WHERE `entry` IN (16876, 16925, 18952, 19701);

/* HILLSBRAD FOOTHILLS */
/* Syndicate Shadow Mage, Elder Moss Creeper, Giant Moss Creeper(also AM), Forest Moss Creeper, Writhing Mage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (2244, 2348, 2349, 2350, 7075);

/* HINTERLANDS */
/* Vilebranch Axe Thrower, Mystic Yayo'jin <Reagent Vendor> */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (2639, 14739);

/* IRONFORGE */


/* KARAZHAN */
UPDATE `instance_template` SET `script` = 'instance_karazhan' WHERE `map`= 532;
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

UPDATE `creature_template` SET `ScriptName` = 'mob_homunculus' WHERE `entry` = 16539;
UPDATE `creature_template` SET `ScriptName` = 'mob_kilrek' WHERE `entry` = 17229;
UPDATE `creature_template` SET `ScriptName` = 'netherspite_infernal' WHERE `entry` = 17646;


/* LOCH MODAN */
/* Mo'grosh Mystic, Tunnel Rat Geomancer, Magosh <Stonesplinter Tribal Shaman>, Stonesplinter Scout, Stonesplinter Seer, Stonesplinter Geomancer, Stonesplinter Skullthumper, Stonesplinter Bonesnapper, Stonesplinter Shaman, Stonesplinter Digger,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1183, 1174, 1399, 1162, 1166, 1165, 1163, 1164, 1197, 1167);

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
UPDATE `instance_template` SET `script` = 'instance_molten_core' WHERE `map`= 409;
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
UPDATE `creature_template` SET `ScriptName` = 'mob_firelord' WHERE `entry`=11668;
UPDATE `creature_template` SET `ScriptName` = 'mob_ancient_core_hound' WHERE `entry` = 11673;
UPDATE `creature_template` SET `ScriptName` = 'mob_core_rager' WHERE `entry` = 11672;
UPDATE `creature_template` SET `ScriptName` = 'mob_firesworn' WHERE `entry` = 12099;
UPDATE `creature_template` SET `ScriptName` = 'mob_firewalker' WHERE `entry` = 11666;
UPDATE `creature_template` SET `ScriptName` = 'mob_flame_guard' WHERE `entry` = 11667;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker' WHERE `entry` = 11661;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_elite' WHERE `entry` = 11664;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_healer' WHERE `entry` = 11663;
UPDATE `creature_template` SET `ScriptName` = 'mob_flamewaker_protector' WHERE `entry` = 12119;
UPDATE `creature_template` SET `ScriptName` = 'mob_lavaspawn' WHERE `entry` = 12265;
UPDATE `creature_template` SET `ScriptName` = 'mob_molten_destroyer' WHERE `entry` = 11659;
UPDATE `creature_template` SET `ScriptName` = 'mob_molten_giant' WHERE `entry` = 11658;


/* MOONGLADE */
UPDATE `creature_template` SET `ScriptName` = 'npc_great_bear_spirit' WHERE `entry` = 11956;

/* MULGORE */
/* Bristleback Shaman, Bristleback Interloper, Bristleback Quilboar, Bristleback Battleboar, Bristleback Interloper,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (2953, 3232, 2952, 2954, 3232);

/* NAGRAND */
UPDATE `creature_template` SET `ScriptName` = 'mob_gurok_the_usurper' WHERE `entry` = 18182;
UPDATE `creature_template` SET `ScriptName` = 'mob_shattered_rumbler' WHERE `entry` = 17157;
UPDATE `creature_template` SET `ScriptName` = 'mobs_kilsorrow_agent' WHERE `entry` IN (17146, 17147, 17148, 18397, 18658);
UPDATE `creature_template` SET `ScriptName` = 'npc_altruis_the_sufferer' WHERE `entry` = 18417;

/* NAXXRAMAS */
UPDATE `instance_template` SET `script` = 'instance_naxxramas' WHERE `map`= 533;
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
/* Dread Creeper, Archmage Tarsis Kir-Moldir */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (15974, 16381);

/* NETHERSTORM */


/* ONYXIA'S LAIR */
UPDATE `creature_template` SET `ScriptName` = 'boss_onyxia' WHERE `entry` = 10184;
/* Onyxian Whelp */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11262);

/* ORGRIMMAR */


/* RAGEFIRE CHASM */


/* RAZORFEN DOWNS */
UPDATE `creature_template` SET `ScriptName` = 'boss_amnennar_the_coldbringer' WHERE `entry` = 7358;
/* Mordresh Fire Eye, Plaguemaw the Rotting, Tuten\'kash, Ragglesnout, Glutton */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7357, 7356, 7355, 7354, 8567);
/* Skeletal Frostweaver, Skeletal Shadowcaster, Skeletal Summoner,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7341, 7340, 7342);

/* REDRIDGE MOUNTAINS */
/* Blackrock Shadowcaster, Murloc Minor Tidecaller, Murloc Nightcrawler, Murloc Tidecaller, Murloc Scout, Murloc Shorestriker, Shadowhide Darkweaver, Rabid Shadowhide Gnoll, Shadowhide Gnoll, Shadowhide Brute, Shadowhide Warrior, Shadowhide Assassin, Redridge Mystic, Redridge Poacher, Redridge Thrasher, Redridge Basher, Black Dragon Whelp, Tarantula, Greater Tarantula, Blackrock Summoner */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (436, 548, 544, 545, 578, 1083, 422, 429, 434, 433, 432, 568, 579, 430, 424, 712, 446, 441, 442, 505, 4463);

/* RUINS OF AHN'QIRAJ */
UPDATE `instance_template` SET `script` = 'instance_ruins_of_ahnqiraj' WHERE `map`= 509;


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
/* Fallen Champion, Scarlet Chaplain, Scarlet Adept, Scarlet Wizard,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6488, 4299, 4296, 4300);

/* SCHOLOMANCE */
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
/* Darkmaster Gandling, Kirtonos the Herald, Rattlegore, Marduk Blackpool, Risen Guard, Risen Bonewarder, Risen Lackey, Risen Aberration, Risen Warrior, Risen Protector, Risen Construct, Risen Guardian, Diseased Ghoul, Ragged Ghoul, Spectral Projection, Spectral Teacher, Necrofiend, Scholomance Neophyte, Scholomance Acolyte, Scholomance Occultist */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1853, 10506, 11622, 10433, 10489, 10491, 10482, 10485, 10486, 10487, 10488, 11598, 10495, 10497, 11263, 10500, 11551, 10470, 10471, 10472);
/* Scholomance Student, Scholomance Necromancer, Scholomance Adept, Scholomance Handler, Splintered Skeleton, Skulking Corpse, Unstable Corpse, Reanimated Corpse, Aspect of Banality, Aspect of Corruption, Aspect of Malice, Aspect of Shadow, Blood Steward of Kirtonos, Spectral Tutor, Dark Shade, Corrupted Spirit, Malicious Spirit, Banal Spirit, Scholomance Dark Summoner */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (10475, 10477, 10469, 11257, 10478, 10479, 10480, 10481, 14518, 14519, 14520, 14521, 14861, 10498, 11284, 14512, 14513, 14514, 11582);

/* SEARING GORGE */
/* Lothos Riftwalker */
UPDATE `creature_template` SET `ScriptName`= 'npc_lothos_riftwalker' WHERE `entry` = 14387;
/* Glassweb Spider, Searing Lava Spider, Greater Lava Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (5856, 5857, 5858);

/* SHADOWFANG KEEP */
/* Archmage Arugal */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4275);

/* SHADOWMOON VALLEY */
UPDATE `creature_template` SET `ScriptName`='neltharaku' WHERE (`entry`='21657');


/* SILITHUS */
/* Orgrimmar Legion Axe Thrower, Stormwind Archmage */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (15617, 15859);
UPDATE `creature_template` SET `ScriptName` = 'npcs_rutgar_and_frankal' WHERE `entry` IN (15170, 15171);
UPDATE `creature_template` SET `ScriptName` = 'npcs_captains_blackanvil_and_skullsplit' WHERE `entry` IN (15440, 15612);

/* SILVERMOON */
UPDATE `creature_template` SET `ScriptName` = 'blood_knight_stillblade' WHERE `entry` = 17768;

/* SILVERPINE FOREST */
/* Dalaran Apprentice, Dalaran Conjuror, Dalaran Mage, Dalaran Wizard, Rot Hide Mystic, Rot Hide Gladerunner, Rot Hide Brute, Rot Hide Plague Weaver, Rot Hide Savage, Rot Hide Bruiser, Mist Creeper, Dalaran Apprentice, Dalaran Wizard, Dalaran Protector, Dalaran Warder, Dalaran Mage, Dalaran Conjuror, Lake Creeper, Elder Lake Creeper, Vile Fin Shorecreeper, Nightlash, Archmage Ataeric */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1867, 1915, 1914, 1889, 1773, 1772, 1939, 1940, 1942, 1944, 1781, 1867, 1889, 1912, 1913, 1914, 1915, 1955, 1956, 1957, 1983, 2120);

/* STOCKADES */
/* Defias Captive, Defias Convict, Defias Inmate, Defias Insurgent, Defias Prisoner,  */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1707, 1711, 1708, 1715, 1706);

/* STONETALON MOUNTAINS */
/* Deepmoss Creeper */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (4005);

/* STORMWIND CITY */
UPDATE `creature_template` SET `ScriptName` = 'npc_bartleby' WHERE `entry` = 6090;
UPDATE `creature_template` SET `ScriptName` = 'dashel_stonefist' WHERE `entry` = 4961;

/* STRANGLETHORN VALE */
UPDATE `creature_template` SET `ScriptName` = 'mob_yenniku' WHERE `entry` = 2530;
/* Bloodscalp Shaman, Bloodsail Elder Magus, Bloodsail Mage, Bloodsail Warlock, Bloodscalp Axe Thrower, Skullsplitter Axe Thrower, Bloodscalp Mystic, Skullsplitter Mystic */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (697, 1653, 1562, 1564, 694, 696, 701, 780);

/* STRATHOLME */
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
/* Jarien, Aurius, Ash'ari Crystal, Elder Farwhisper, Mindless Undead */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (16101, 10917, 10415, 15607, 11030);

/* SUNKEN TEMPLE */
/* Zekkis, Kazkaz the Unholy, Spawn of Hakkar, Shade of Eranikus, Jammal'an the Prophet, Ogom the Wretched, Zolo, Gasher, Loro, Hukku, Zul'Lor, Mijan, Morphaz, Weaver, Dreamscythe, Hazzas, Avatar of Hakkar, Atal'alarion <Guardian of the Idol> */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (5400, 5401, 5708, 5709, 5710, 5711, 5712, 5713, 5714, 5715, 5716, 5717, 5719, 5720, 5721, 5722, 8443, 8580);
/* Jade, Murk Slitherer, Murk Spitter, Murk Worm, Saturated Ooze, Fungal Ooze, Cursed Atal'ai, Atal'ai Witch Doctor, Enthralled Atal'ai, Mummified Atal'ai, Unliving Atal'ai, Atal'ai Priest, Atal'ai Corpse Eater, Atal'ai Deathwalker, Atal'ai High Priest, Nightmare Scalebane, Nightmare Wyrmkin, Nightmare Wanderer, Hakkari Frostwing, Nightmare Whelp, Atal'ai Skeleton, Hakkari Sapper, Hakkari Minion, Hakkari Bloodkeeper, Nightmare Suppressor */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1063, 5224, 5225, 5226, 5228, 5235, 5243, 5259, 5261, 5263, 5267, 5269, 5270, 5271, 5273, 5277, 5280, 5283, 5291, 8319, 8324, 8336, 8437, 8438, 8497);

/* SWAMP OF SORROWS */
/* Marsh Murloc, Adolescent Whelp, Dreaming Whelp, Wyrmkin Dreamwalker, Scalebane Captain, Deathstrike Tarantula */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (747, 740, 741, 743, 745, 769);

/* TANARIS */
UPDATE `creature_template` SET `ScriptName` = 'npc_steward_of_time' WHERE `entry` = 20142;
/* Dunemaul Ogre Mage, Wastewander Shadow Mage, Sandfury Axe Thrower */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (5473, 5617, 5646);

/* TELDRASSIL */
/* Gnarlpine Mystic, Gnarlpine Pathfinder, Bloodfeather Sorceress, Webwood Spider, Giant Webwood Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7235, 2012, 2018, 1986, 2001);


/*  */
/* TEMPEST KEEP */
/*  */

/* THE MECHANAR */

/* THE BOTANICA */

/* THE ARCATRAZ */

/* THE EYE */
UPDATE `instance_template` SET `script` = 'instance_the_eye' WHERE `map`= 550;
/* The Eye Trash Mobs */
UPDATE `creature_template` SET `scriptname`='mob_the_eye' WHERE `entry` IN (20031,20032,20033,20034,20035,20039,20041,20043,20046,20052);
UPDATE `creature_template` SET `scriptname`='mob_crystalcore_devastator' WHERE `entry`='20040';
/* VoidReaver and Arcane orb Target */
UPDATE `creature_template` SET `ScriptName` = 'boss_void_reaver' WHERE `entry` = 19516;
UPDATE `creature_template` SET `ScriptName` = 'mob_arcane_orb_target' WHERE `entry` = 19577;


/* TEMPLE OF AHN'QIRAJ */
UPDATE `instance_template` SET `script` = 'instance_temple_of_ahnqiraj' WHERE `map`= 531;
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
UPDATE creature_template SET ScriptName='boss_eye_of_cthun' WHERE entry=15589;
UPDATE creature_template SET ScriptName='mob_claw_tentacle' WHERE entry=15725;
UPDATE creature_template SET ScriptName='mob_eye_tentacle' WHERE entry=15726;
UPDATE creature_template SET ScriptName='boss_cthun' WHERE entry=15727;
UPDATE creature_template SET ScriptName='mob_giant_claw_tentacle' WHERE entry=15728;
UPDATE creature_template SET ScriptName='mob_giant_eye_tentacle' WHERE entry=15334;
UPDATE creature_template SET ScriptName='mob_giant_flesh_tentacle' WHERE entry=15802;


/* TEROKKAR FOREST */
UPDATE `creature_template` SET `ScriptName` = 'mob_infested_root_walker' WHERE `entry` = 22095;
UPDATE `creature_template` SET `ScriptName` = 'mob_netherweb_victim' WHERE `entry` = 22355;
UPDATE `creature_template` SET `ScriptName` = 'mob_rotting_forest_rager' WHERE `entry` = 22307;
UPDATE `creature_template` SET `ScriptName` = 'mobs_shadow_council_covert' WHERE `entry` IN (18716, 18717, 18719);

/* THOUSAND NEEDLES */


/* THUNDER BLUFF */


/* TIRISFAL GLADES */
/* Scarlet Neophyte, Rot Hide Gnoll, Young Night Web Spider, Night Web Spider, Vicious Night Web Spider */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1539, 1674, 1504, 1505, 1555);

/* ULDAMAN */
/* Uldaman Trash Mobs */
UPDATE `creature_template` SET `scriptname`='mob_uldaman' WHERE `entry` IN (4847,4852,4853,4854,4860,4861,6910,7011,7012,7022,7030,7078,7291);
UPDATE `creature_template` SET `scriptname`='mob_jadespine_basilisk' WHERE `entry`='4863';

/* Uldaman Boss Ironaya */
UPDATE `creature_template` SET `scriptname`='boss_ironaya' WHERE `entry`='7228';


/* UN'GORO CRATER */
/* Tar Creeper */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6527);

/* WAILING CAVERNS */
/* Deviate Creeper */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (3632);

/* WESTERN PLAGUELANDS */
/* Skeletal Acolyte, Skeletal Flayer, Skeletal Sorcerer, Scarlet Mage, Araj the Summoner */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1789, 1783, 1784, 1826, 1852);

/* WESTFALL */
/* Defias Pillager, Defias Smuggler, Defias Trapper, Defias Highwayman, Defias Knuckleduster, Defias Pathstalker, Defias Looter, Murloc Oracle, Murloc Hunter, Murloc Warrior, Murloc Tidehunter, Murloc Coastrunner, Riverpaw Scout, Riverpaw Mongrel, Riverpaw Brute, Riverpaw Herbalist, Defias Renegade Mage, Riverpaw Mystic */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (589, 95, 504, 122, 449, 121, 590, 517, 458, 171, 127, 126, 500, 123, 124, 501, 450, 453);

/* WETLANDS */
/* Bluegill Forager, Bluegill Murloc, Bluegill Muckdweller, Bluegill Raider, Bluegill Warrior, Bluegill Oracle, Mosshide Mistweaver, Mosshide Mystic, Mosshide Trapper, Mosshide Mongrel, Mosshide Gnoll, Fen Creeper, Red Whelp, Lost Whelp, Flamesnorting Whelp, Crimson Whelp */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (1026, 1024, 1028, 1418, 1027, 1029, 1009, 1013, 1011, 1008, 1007, 1040, 1042, 1043, 1044, 1069);

/* WINTERSPRING */
/* Cobalt Mageweaver, Chillwind Chimaera */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7437, 7448);

/* ZANGARMARSH */
UPDATE `creature_template` SET `ScriptName` = 'npcs_ashyen_and_keleth' WHERE `entry` IN (17900, 17901);
UPDATE `creature_template` SET `ScriptName` = 'npc_elder_kuruti' WHERE `entry` = 18197;

/* ZUL'FARRAK */


/* ZUL'GURUB */
UPDATE `instance_template` SET `script` = 'instance_zulgurub' WHERE `map`= 309;
/* Gurubashi Axe Thrower */
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11350);



/* EOF */
