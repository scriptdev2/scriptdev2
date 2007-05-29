--
-- area data
--

--
-- boss data
--

UPDATE `creature_template` SET`ScriptName` = 'boss_onyxia' WHERE `entry` = 10184;

UPDATE `creature_template` SET`ScriptName` = 'boss_lucifron' WHERE `entry` = 12118;
UPDATE `creature_template` SET`ScriptName` = 'boss_magmadar' WHERE `entry` = 11982;
UPDATE `creature_template` SET`ScriptName` = 'boss_gehennas' WHERE `entry` = 12259;
UPDATE `creature_template` SET`ScriptName` = 'boss_garr' WHERE `entry` = 12057;
UPDATE `creature_template` SET`ScriptName` = 'boss_baron_geddon' WHERE `entry` = 12056;
UPDATE `creature_template` SET`ScriptName` = 'boss_shazzrah' WHERE `entry` = 12264;
UPDATE `creature_template` SET`ScriptName` = 'boss_golemagg' WHERE `entry` = 11988;
UPDATE `creature_template` SET`ScriptName` = 'boss_sulfuron' WHERE `entry` = 12098;
UPDATE `creature_template` SET`ScriptName` = 'boss_majordomo' WHERE `entry` = 12018;
UPDATE `creature_template` SET`ScriptName` = 'boss_ragnaros' WHERE `entry` = 11502;

-- Instances

-- Scarlet Monastery
-- 2953 - Fallen Champion
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6488);
UPDATE `creature_template` SET `spell1` = 19644  WHERE `entry` = 6488;
UPDATE `creature_template` SET `spell2` = 21949 WHERE `entry` = 6488;
UPDATE `creature_template` SET `spell3` = 19642 WHERE `entry` = 6488;

--
-- creature data
--
UPDATE `creature_template` SET`ScriptName` = '' WHERE `ScriptName` = 'sc_default';
UPDATE `creature_template` SET`ScriptName` = 'lumberjack' WHERE `entry` IN (11328,11260);
UPDATE `creature_template` SET`ScriptName` = 'miner' WHERE `entry` IN (80,1236,3578,40,1358,1360,5996,1426,1094,674);

-- Generic Scripts
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (946, 4063, 1867, 1867, 1915, 1914, 1889, 314, 2570, 2567, 697, 1653, 1562, 1564, 3218, 2591, 2255, 1539, 4299, 4296, 4300, 533, 203 , 2577 , 3273 , 1183 , 436 , 7235 , 2012, 1397 , 1174 , 2018 , 474 , 910 , 589 , 1399 , 3783 , 3732 , 3725 , 3728 , 3662 , 2207 , 7026 , 7028 , 7048 , 7038); -- Base

-- Revision 9
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (2953, 3232, 2952, 2954, 1732, 1726, 619, 634, 1725, 4418, 474, 910, 95, 589);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (3232, 2952, 2954, 1707, 1711, 1708, 1715, 1706, 1729, 657, 4417, 598, 4416, 594, 6866, 215, 909, 116, 504, 94, 122, 449, 121, 590);

-- Revision 11
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (4820, 1026, 517, 458, 548);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (4819, 4818, 4359, 1024, 1028, 1418, 1027, 1029, 747, 171, 544, 545, 578, 127, 126, 732, 46, 1083, 422);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (1009, 1013, 429, 1773);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (1011, 1008, 1007, 434, 433, 432, 568, 579, 448, 500, 123, 124, 501, 1674, 1772, 1939, 1940, 1942, 1944);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (3112, 3113, 3269, 3458, 3271, 3265);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (3111, 3114, 3459, 3267);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (430, 424, 1162, 1166, 1165);
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (712, 446, 1115, 1117, 1116, 1118, 1163, 1164, 1197, 1167);

-- Revision 13
UPDATE `creature_template` SET`ScriptName` = 'generic_creature' WHERE `entry` IN (1789, 4473, 10391, 531, 1783, 7341, 10390, 787, 203, 1110, 7340, 1784, 7342, 785, 48);


-- Creature specific
UPDATE `creature_template` SET`ScriptName` = '' WHERE `entry` = 257;

--
-- custom data
--

--
-- go data
--

--
-- guard data
--

UPDATE `creature_template` SET`ScriptName` = 'guard_orgrimmar' WHERE `entry` = 3296;
UPDATE `creature_template` SET`ScriptName` = 'guard_stormwind' WHERE `entry` IN (68,1976);
UPDATE `creature_template` SET`ScriptName` = 'guard_contested' WHERE `entry` IN (9460,4624,3502,11190,15184);
UPDATE `creature_template` SET`ScriptName` = 'guard_elwynnforest' WHERE `entry` = 1423;
UPDATE `creature_template` SET`ScriptName` = 'guard_darnassus' WHERE `entry` = 4262;
UPDATE `creature_template` SET`ScriptName` = 'guard_teldrassil' WHERE `entry` = 3571;
UPDATE `creature_template` SET`ScriptName` = 'guard_ironforge' WHERE `entry` = 5595;
UPDATE `creature_template` SET`ScriptName` = 'guard_dunmorogh' WHERE `entry` IN (727,13076);
UPDATE `creature_template` SET`ScriptName` = 'guard_undercity' WHERE `entry` = 5624;
UPDATE `creature_template` SET`ScriptName` = 'guard_bluffwatcher' WHERE `entry` = 3084;
UPDATE `creature_template` SET`ScriptName` = 'guard_razorhill' WHERE `entry` = 5953;
UPDATE `creature_template` SET`ScriptName` = 'guard_mulgore' WHERE `entry` IN (3210,3211,3212,3213,3214,3215,3217,3218,3219,3220,3221,3222,3223,3224);
UPDATE `creature_template` SET`ScriptName` = 'guard_dunmorogh' WHERE `entry` = 727 OR `entry` = 13076;
UPDATE `creature_template` SET`ScriptName` = 'guard_tirisfal' WHERE `entry` IN (1735,1738,2210,1736,1739,1737,1744,1745,5725,1743,2209,1746,1742);
UPDATE `creature_template` SET `scriptName` = 'guard_silvermoon' WHERE `entry` = 16222;
UPDATE `creature_template` SET `scriptName` = 'guard_exodar' WHERE `entry` = 16733;

--
-- Honor data.
--

UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` IN (40100,40101,40102,40103,40104,40105,40106,40107,40108,40109,40110,40111,40112,40113);
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` IN (40200,40201,40202,40203,40204,40205,40206,40207,40208,40209,40210,40211,40212,40213);


--
-- item data
--

--
-- npc data
--

UPDATE `creature_template` SET`ScriptName` = 'marshal_mcbride' WHERE `entry` = 197;
UPDATE `creature_template` SET`ScriptName` = 'silva_filnaveth' WHERE `entry` = 11800;
UPDATE `creature_template` SET`ScriptName` = 'skorn_whitecloud' WHERE `entry` = 3052;
UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'bunthen_plainswind' WHERE `entry` = 11798;

--
-- servers data
--

UPDATE `creature_template` SET`ScriptName` = 'battlemaster' WHERE `entry` IN (12198,14982,857,12197,7427,347,15007,2804);
UPDATE `creature_template` SET`ScriptName` = 'guildmaster' WHERE `entry` IN (4974,5054,4613);
UPDATE `creature_template` SET`ScriptName` = 'travelmaster' WHERE `entry` IN (9564,12136,3149,12137,9566,3150,9559,9558);


--
-- creature spells update
--
UPDATE `creature_template` SET `spell1` = 116 WHERE `entry` = 946;
UPDATE `creature_template` SET `spell1` = 16415 WHERE `entry` = 4063;
UPDATE `creature_template` SET `spell1` = 205 WHERE `entry` = 1867;
UPDATE `creature_template` SET `spell1` = 705 WHERE `entry` = 1915;
UPDATE `creature_template` SET `spell1` = 145 WHERE `entry` = 1914;
UPDATE `creature_template` SET `spell2` = 134 WHERE `entry` = 1914;
UPDATE `creature_template` SET `spell1` = 837 WHERE `entry` = 1889;
UPDATE `creature_template` SET `spell2` = 122 WHERE `entry` = 1889;
UPDATE `creature_template` SET `spell1` = 8406 WHERE `entry` = 314;
UPDATE `creature_template` SET `spell2` = 865 WHERE `entry` = 314;
UPDATE `creature_template` SET `spell1` = 6041 WHERE `entry` = 2570;
UPDATE `creature_template` SET `spell2` = 6364 WHERE `entry` = 2570;
UPDATE `creature_template` SET `spell1` = 8407 WHERE `entry` = 2567;
UPDATE `creature_template` SET `spell2` = 12486 WHERE `entry` = 2567;
UPDATE `creature_template` SET `spell1` = 6041 WHERE `entry` = 697;
UPDATE `creature_template` SET `spell1` = 8402 WHERE `entry` = 1653;
UPDATE `creature_template` SET `spell2` = 8423 WHERE `entry` = 1653;
UPDATE `creature_template` SET `spell1` = 8402 WHERE `entry` = 1562;
UPDATE `creature_template` SET `spell1` = 7641 WHERE `entry` = 1564;
UPDATE `creature_template` SET `spell2` = 11707 WHERE `entry` = 1564;
UPDATE `creature_template` SET `spell1` = 1106 WHERE `entry` = 3218;
UPDATE `creature_template` SET `spell1` = 837 WHERE `entry` = 2591;
UPDATE `creature_template` SET `spell1` = 837 WHERE `entry` = 2255;
UPDATE `creature_template` SET `spell1` = 205 WHERE `entry` = 1539;
UPDATE `creature_template` SET `spell1` = 15264 WHERE `entry` = 4299;
UPDATE `creature_template` SET `spell2` = 6060 WHERE `entry` = 4299;
UPDATE `creature_template` SET `spell3` = 988 WHERE `entry` = 4299;
UPDATE `creature_template` SET `spell1` = 984 WHERE `entry` = 4296;
UPDATE `creature_template` SET `spell2` = 1026 WHERE `entry` = 4296;
UPDATE `creature_template` SET `spell1` = 8438 WHERE `entry` = 4300;
UPDATE `creature_template` SET `spell2` = 2601 WHERE `entry` = 4300;
UPDATE `creature_template` SET `spell1` = 1088 WHERE `entry` = 533;
UPDATE `creature_template` SET `spell2` = 992 WHERE `entry` = 533;
UPDATE `creature_template` SET `spell1` = 7322 WHERE `entry` = 203;
UPDATE `creature_template` SET `spell1` = 1106 WHERE `entry` = 2577;
UPDATE `creature_template` SET `spell2` = 2941 WHERE `entry` = 2577;
UPDATE `creature_template` SET `spell1` = 548 WHERE `entry` = 3273;
UPDATE `creature_template` SET `spell2` = 6535 WHERE `entry` = 3273;
UPDATE `creature_template` SET `spell1` = 529 WHERE `entry` = 1183;
UPDATE `creature_template` SET `spell1` = 992 WHERE `entry` = 436;
UPDATE `creature_template` SET `spell1` = 705 WHERE `entry` = 436;
UPDATE `creature_template` SET `spell1` = 5177 WHERE `entry` = 7235;
UPDATE `creature_template` SET `spell1` = 5177 WHERE `entry` = 2012;
UPDATE `creature_template` SET `spell1` = 403 WHERE `entry` = 1397;
UPDATE `creature_template` SET `spell1` = 770 WHERE `entry` = 1397;
UPDATE `creature_template` SET `spell1` = 2136 WHERE `entry` = 1174;
UPDATE `creature_template` SET `spell1` = 143 WHERE `entry` = 2018;
UPDATE `creature_template` SET `spell1` =  915 WHERE `entry` =  1399;
UPDATE `creature_template` SET `spell2` =  2606 WHERE `entry` =  1399;
UPDATE `creature_template` SET `spell1` =  915 WHERE `entry` =  3783;
UPDATE `creature_template` SET `spell2` =  8293 WHERE `entry` =  3783;
UPDATE `creature_template` SET `spell1` =  984 WHERE `entry` =  3732;
UPDATE `creature_template` SET `spell1` =  13480 WHERE `entry` =  3725;
UPDATE `creature_template` SET `spell1` =  13480 WHERE `entry` =  3728;
UPDATE `creature_template` SET `spell1` =  9672 WHERE `entry` =  3662;
UPDATE `creature_template` SET `spell2` =  7101 WHERE `entry` =  3662;
UPDATE `creature_template` SET `spell1` =  18089 WHERE `entry` =  2207;
UPDATE `creature_template` SET `spell1` =  8402 WHERE `entry` =  7026;
UPDATE `creature_template` SET `spell2` =  10215 WHERE `entry` =  7026;
UPDATE `creature_template` SET `spell1` =  11659 WHERE `entry` =  7028;
UPDATE `creature_template` SET `spell1` =  8402 WHERE `entry` =  7048;
UPDATE `creature_template` SET `spell1` =  7918 WHERE `entry` =  7038;
UPDATE `creature_template` SET `spell2` =  6685 WHERE `entry` =  7038;

-- added revision 9
UPDATE `creature_template` SET `spell1` = 12166 WHERE `entry` = 3232;
UPDATE `creature_template` SET `spell1` = 403 WHERE `entry` = 2953;#Lightning Bolt
UPDATE `creature_template` SET `spell1` = 3385 WHERE `entry` = 2954;#Boar Charge
UPDATE `creature_template` SET `spell1` = 15657 WHERE `entry` = 1707;#Backstab
UPDATE `creature_template` SET `spell2` = 17230 WHERE `entry` = 1707;#Infected Wound
UPDATE `creature_template` SET `spell4` = 16509 WHERE `entry` = 1707;#Rend
UPDATE `creature_template` SET `spell1` = 6253 WHERE `entry` = 1711; #Backhand
UPDATE `creature_template` SET `spell2` = 16244 WHERE `entry` = 1711; #Demoralizing Shout
UPDATE `creature_template` SET `spell3` = 8242 WHERE `entry` = 1711; #Shield Slam
UPDATE `creature_template` SET `spell4` = 16509 WHERE `entry` = 1711; #Rend
UPDATE `creature_template` SET `spell1` = 6547 WHERE `entry` = 1708; #Rend R3
UPDATE `creature_template` SET `spell2` = 2590 WHERE `entry` = 1708; #Backstab R3
UPDATE `creature_template` SET `spell3` = 6253 WHERE `entry` = 1708; #Backhand
UPDATE `creature_template` SET `spell1` = 11554 WHERE `entry` = 1715; #Demoralizing Shout R3
UPDATE `creature_template` SET `spell2` = 8242 WHERE `entry` = 1715; #Shield Slam
UPDATE `creature_template` SET `spell3` = 6253 WHERE `entry` = 1715; #Backhand
UPDATE `creature_template` SET `spell1` = 1768 WHERE `entry` = 1706; #Kick R3
UPDATE `creature_template` SET `spell2` = 17230 WHERE `entry` = 1706; #Infected Wound
UPDATE `creature_template` SET `spell3` = 16509 WHERE `entry` = 1706; #Rend
UPDATE `creature_template` SET `spell1` = 5115 WHERE `entry` = 1729; #Battle Command
UPDATE `creature_template` SET `spell1` = 9915 WHERE `entry` = 1732; #Frost Nove R3
UPDATE `creature_template` SET `spell1` = 143 WHERE `entry` = 1726; #Fireball R2
-- UPDATE `creature_template` SET `spell2` = 5110 WHERE `entry` = 1726; #Summon: Living Flame (NYI)
UPDATE `creature_template` SET `spell1` = 143 WHERE `entry` = 619; #Fireball R2
-- UPDATE `creature_template` SET `spell2` = 5172 WHERE `entry` = 619; #Bloodsail Companion (NYI)
UPDATE `creature_template` SET `spell1` = 6660 WHERE `entry` = 657; #Shoot
-- UPDATE `creature_template` SET `spell2` = 5172 WHERE `entry` = 657; #Bloodsail Companion (NYI)
UPDATE `creature_template` SET `spell1` = 6660 WHERE `entry` = 4417; #Shoot
UPDATE `creature_template` SET `spell2` = 6685 WHERE `entry` = 4417; #Piercing Shot
UPDATE `creature_template` SET `spell1` = 6685 WHERE `entry` = 598; #Piercing Shot
UPDATE `creature_template` SET `spell1` = 6016 WHERE `entry` = 4416; #Pierce Armor
UPDATE `creature_template` SET `spell1` = 5115 WHERE `entry` = 594; #Battle Command
UPDATE `creature_template` SET `spell2` = 6435 WHERE `entry` = 594; #Smite Slam
UPDATE `creature_template` SET `spell1` = 9915 WHERE `entry` = 634; #Frost Nove R3
UPDATE `creature_template` SET `spell1` = 143 WHERE `entry` = 1725; #Fireball R2
UPDATE `creature_template` SET `spell1` = 205 WHERE `entry` = 1725; #Frostbolt R2
UPDATE `creature_template` SET `spell1` = 133 WHERE `entry` = 4418; #Fireball
UPDATE `creature_template` SET `spell2` = 205 WHERE `entry` = 4418; #Frostbolt R2
UPDATE `creature_template` SET `spell3` = 113 WHERE `entry` = 4418; #Chains of Ice
UPDATE `creature_template` SET `spell1` = 6136 WHERE `entry` = 474; #Chilled
UPDATE `creature_template` SET `spell2` = 116 WHERE `entry` = 474; #Frostbolt
UPDATE `creature_template` SET `spell2` = 676 WHERE `entry` = 6866; #Disarm
UPDATE `creature_template` SET `spell3` = 53 WHERE `entry` = 6866; #Backstab
UPDATE `creature_template` SET `spell1` = 3140 WHERE `entry` = 910; #Fireball R4
UPDATE `creature_template` SET `spell2` = 12486 WHERE `entry` = 910; #Chilled R3
UPDATE `creature_template` SET `spell3` = 3443 WHERE `entry` = 910; #Enchanted Quickeness
UPDATE `creature_template` SET `spell1` = 744 WHERE `entry` = 909; #Poison
UPDATE `creature_template` SET `spell2` = 7992 WHERE `entry` = 909; #Slowing Poison
UPDATE `creature_template` SET `spell3` = 2590 WHERE `entry` = 909; #Backstab R3
UPDATE `creature_template` SET `spell1` = 8646 WHERE `entry` = 116; #Snap Kick
UPDATE `creature_template` SET `spell1` = 53 WHERE `entry` = 504; #Backstab
UPDATE `creature_template` SET `spell1` = 2764 WHERE `entry` = 95; #Throw
UPDATE `creature_template` SET `spell1` = 53 WHERE `entry` = 94; #Backstab
UPDATE `creature_template` SET `spell2` = 133 WHERE `entry` = 589; #Fireball
UPDATE `creature_template` SET `spell1` = 168 WHERE `entry` = 589; #Frost Armor
UPDATE `creature_template` SET `spell2` = 53 WHERE `entry` = 122; #Backstab
UPDATE `creature_template` SET `spell2` = 1671 WHERE `entry` = 449; #Shield Bash R2
UPDATE `creature_template` SET `spell1` = 71 WHERE `entry` = 449; #Defensive Stance
UPDATE `creature_template` SET `spell2` = 6554 WHERE `entry` = 121; #Pummel R2
UPDATE `creature_template` SET `spell2` = 53 WHERE `entry` = 590; #Backstab

-- Added revision 11
UPDATE `creature_template` SET `spell1` = 8382 WHERE `entry` = 4819; #Leech Poison
-- UPDATE `creature_template` SET `spell3` = 1604 WHERE `entry` = 4819; #Dazed (NYI)
UPDATE `creature_template` SET `spell2` = 8733 WHERE `entry` = 4819; #Blessing of Blackfathom
UPDATE `creature_template` SET `spell1` = 6145 WHERE `entry` = 4818; #Eagle Eye
UPDATE `creature_template` SET `spell2` = 71 WHERE `entry` = 4818; #Defensive Stance
-- UPDATE `creature_template` SET `spell4` = 1604 WHERE `entry` = 4818; #Dazed (NYI)
UPDATE `creature_template` SET `spell3` = 22691 WHERE `entry` = 4818; #Disarm
UPDATE `creature_template` SET `spell1` = 14109 WHERE `entry` = 4820; #Lightning Bolt
UPDATE `creature_template` SET `spell2` = 3358 WHERE `entry` = 4820; #Leech Poison
UPDATE `creature_template` SET `spell3` = 8733 WHERE `entry` = 4820; #Blessing of Blackfathom
UPDATE `creature_template` SET `spell4` = 12024 WHERE `entry` = 4820; #Net
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 4359; #Poisonous Stab
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 1024; #Poisonous Stab
UPDATE `creature_template` SET `spell1` = 1777 WHERE `entry` = 1028; #Gouge R2
UPDATE `creature_template` SET `spell2` = 6533 WHERE `entry` = 1028; #Net
UPDATE `creature_template` SET `spell1` = 10177 WHERE `entry` = 1418; #Frost Ward R4
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 1418; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 744 WHERE `entry` = 1026; #Poison
UPDATE `creature_template` SET `spell2` = 1707 WHERE `entry` = 1026; #Mind Bomb
UPDATE `creature_template` SET `spell1` = 7372 WHERE `entry` = 1027; #Hamstring R2
UPDATE `creature_template` SET `spell2` = 2457 WHERE `entry` = 1027; #Battle Stance
UPDATE `creature_template` SET `spell1` = 15869 WHERE `entry` = 1029; #Superior Healing Ward
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 747; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 747; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 171; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 171; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 544; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 544; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 545; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 545; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 578; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 578; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 744 WHERE `entry` = 127; #Poison
UPDATE `creature_template` SET `spell2` = 865 WHERE `entry` = 127; #Frost Nova R2
UPDATE `creature_template` SET `spell1` = 13519 WHERE `entry` = 517; #Holy Smite
UPDATE `creature_template` SET `spell2` = 6074 WHERE `entry` = 517; #Renew R2
UPDATE `creature_template` SET `spell1` = 10277 WHERE `entry` = 458; #Throw
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 458; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 7357 WHERE `entry` = 126; #Poisonous Stab
-- UPDATE `creature_template` SET `spell2` = 15656 WHERE `entry` = 126; #Teleport to Player (NYI)
UPDATE `creature_template` SET `spell1` = 205 WHERE `entry` = 548; #Frostbolt R2
UPDATE `creature_template` SET `spell2` = 331 WHERE `entry` = 548; #Healing Wave
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 732; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 2589 WHERE `entry` = 732; #Backstab R2
UPDATE `creature_template` SET `spell1` = 3368 WHERE `entry` = 46; #Drink Minor Potion
UPDATE `creature_template` SET `spell1` = 6268 WHERE `entry` = 1083; #Rushing Charge
UPDATE `creature_template` SET `spell1` = 3393 WHERE `entry` = 422; #Consume Flesh
UPDATE `creature_template` SET `spell1` = 3288 WHERE `entry` = 1011; #Moss Hide
UPDATE `creature_template` SET `spell1` = 8016 WHERE `entry` = 1008; #Spirit Decay
UPDATE `creature_template` SET `spell1` = 8016 WHERE `entry` = 1007; #Spirit Decay
UPDATE `creature_template` SET `spell1` = 205 WHERE `entry` = 1009; #Frosbolt R2
UPDATE `creature_template` SET `spell1` = 547 WHERE `entry` = 1013; #Healing Wave R3
UPDATE `creature_template` SET `spell2` = 548 WHERE `entry` = 1013; #Lightning Bolt R3
UPDATE `creature_template` SET `spell1` = 744 WHERE `entry` = 434; #Poison
UPDATE `creature_template` SET `spell1` = 3150 WHERE `entry` = 433; #Rabies
UPDATE `creature_template` SET `spell1` = 6205 WHERE `entry` = 432; #Curse of Weakness
UPDATE `creature_template` SET `spell1` = 705 WHERE `entry` = 429; #Shadow Bolt R3
UPDATE `creature_template` SET `spell1` = 71 WHERE `entry` = 568; #Defensive Stance
UPDATE `creature_template` SET `spell2` = 8380 WHERE `entry` = 568; #Sunder Armor R3
UPDATE `creature_template` SET `spell3` = 8629 WHERE `entry` = 568; #Gouge R3
UPDATE `creature_template` SET `spell1` = 744 WHERE `entry` = 579; #Poison
UPDATE `creature_template` SET `spell1` = 6730 WHERE `entry` = 448; #Head Butt
UPDATE `creature_template` SET `spell2` = 6016 WHERE `entry` = 448; #Pierce Armor
UPDATE `creature_template` SET `spell1` = 6730 WHERE `entry` = 500; #Head Butt
UPDATE `creature_template` SET `spell2` = 6016 WHERE `entry` = 500; #Pierce Armor
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 123; #Dazed (NYI)
UPDATE `creature_template` SET `spell2` = 8016 WHERE `entry` = 123; #Spirit Decay
UPDATE `creature_template` SET `spell1` = 1160 WHERE `entry` = 124; #Demoralizing Shout
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 501; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 3368 WHERE `entry` = 501; #Drink Minor Potion
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1674; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1772; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1773; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 695 WHERE `entry` = 1773; #Shadow Bolt R2
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1939; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1940; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1942; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 3237 WHERE `entry` = 1944; #Curse of Thule
UPDATE `creature_template` SET `spell1` = 2480 WHERE `entry` = 3112; #Shoot Bow
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 3112; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 5280 WHERE `entry` = 3111; #Razor Mane
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 3114; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 3248 WHERE `entry` = 3114; #Improved Blocking
UPDATE `creature_template` SET `spell1` = 6950 WHERE `entry` = 3113; #Faerie Fire
UPDATE `creature_template` SET `spell2` = 774 WHERE `entry` = 3113; #Rejuvenation
UPDATE `creature_template` SET `spell1` = 548 WHERE `entry` = 3269; #Lightning Bolt R3
UPDATE `creature_template` SET `spell2` = 8045 WHERE `entry` = 3269; #Earth Shock R3
UPDATE `creature_template` SET `spell3` = 8052 WHERE `entry` = 3269; #Flame Shock R2
UPDATE `creature_template` SET `spell1` = 594 WHERE `entry` = 3458; #Shadow Word: Pain R2
UPDATE `creature_template` SET `spell2` = 8092 WHERE `entry` = 3458; #Mind Blast
UPDATE `creature_template` SET `spell3` = 2052 WHERE `entry` = 3458; #Lesser Heal R2
UPDATE `creature_template` SET `spell1` = 594 WHERE `entry` = 3271; #Shadow Word: Pain R2
UPDATE `creature_template` SET `spell2` = 598 WHERE `entry` = 3271; #Smite R3
UPDATE `creature_template` SET `spell3` = 8102 WHERE `entry` = 3271; #Mind Blast R2
UPDATE `creature_template` SET `spell4` = 6074 WHERE `entry` = 3271; #Renew R2
UPDATE `creature_template` SET `spell1` = 1978 WHERE `entry` = 3265; #Serpent Sting
UPDATE `creature_template` SET `spell2` = 3044 WHERE `entry` = 3265; #Arcane Shot
UPDATE `creature_template` SET `spell1` = 13446 WHERE `entry` = 3459; #Strike
UPDATE `creature_template` SET `spell2` = 12323 WHERE `entry` = 3459; #Piercing Howl
UPDATE `creature_template` SET `spell3` = 13443 WHERE `entry` = 3459; #Rend
UPDATE `creature_template` SET `spell1` = 22356 WHERE `entry` = 3267; #Slow
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 3267; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 3427 WHERE `entry` = 712; #Infected Wound
UPDATE `creature_template` SET `spell1` = 5164 WHERE `entry` = 446; #Knockdown
UPDATE `creature_template` SET `spell1` = 331 WHERE `entry` = 430; #Healing Wave
UPDATE `creature_template` SET `spell2` = 548 WHERE `entry` = 430; #Lightning Bolt R3
UPDATE `creature_template` SET `spell1` = 7919 WHERE `entry` = 424; #Shoot Crossbow
UPDATE `creature_template` SET `spell1` = 3148 WHERE `entry` = 1115; #Head Crack
UPDATE `creature_template` SET `spell1` = 5164 WHERE `entry` = 1117; #Knockdown
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 1116; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 53 WHERE `entry` = 1116; #Backstab
-- UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 1118; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 5164 WHERE `entry` = 1118; #Knockdown
UPDATE `creature_template` SET `spell1` = 10277 WHERE `entry` = 1162; #Throw
UPDATE `creature_template` SET `spell1` = 529 WHERE `entry` = 1166; #Lightning Bolt R2
UPDATE `creature_template` SET `spell1` = 1776 WHERE `entry` = 1163; #Gouge
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 1163; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 11976 WHERE `entry` = 1164; #Strike
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 1164; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 331 WHERE `entry` = 1197; #Healing Wave
-- UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 1197; #Dazed (NYI)
UPDATE `creature_template` SET `spell1` = 2121 WHERE `entry` = 1165; #Flamestrike R2
UPDATE `creature_template` SET `spell1` = 7405 WHERE `entry` = 1167; #Sunder Armor R2
UPDATE `creature_template` SET `spell2` = 71 WHERE `entry` = 1167; #Defensive Stance

-- Added revision 13
UPDATE `creature_template` SET `spell1` = 11660 WHERE `entry` = 1789; #Shadow Bolt R8
UPDATE `creature_template` SET `spell2` = 17926 WHERE `entry` = 1789; #Death Coil R3
UPDATE `creature_template` SET `spell3` = 6215 WHERE `entry` = 1789; #Curse of Agony R6
UPDATE `creature_template` SET `spell4` = 11672 WHERE `entry` = 1789; #Corruption R6
UPDATE `creature_template` SET `spell1` = 348 WHERE `entry` = 4473; #Immolate
UPDATE `creature_template` SET `spell2` = 686 WHERE `entry` = 4473; #Shadow Bolt
UPDATE `creature_template` SET `spell1` = 15580 WHERE `entry` = 10391; #Strike R5
UPDATE `creature_template` SET `spell2` = 15613 WHERE `entry` = 10391; #Cleave R3
UPDATE `creature_template` SET `spell3` = 23600 WHERE `entry` = 10391; #Piercing Howl
UPDATE `creature_template` SET `spell4` = 16406 WHERE `entry` = 10391; #Rend R6
UPDATE `creature_template` SET `spell1` = 3416 WHERE `entry` = 531; #Fiend Fury
UPDATE `creature_template` SET `spell1` = 7369 WHERE `entry` = 1783; #Cleave R2
UPDATE `creature_template` SET `spell1` = 838 WHERE `entry` = 7341; #Frostbolt R3
UPDATE `creature_template` SET `spell1` = 15580 WHERE `entry` = 10390; #Strike R5
UPDATE `creature_template` SET `spell2` = 15613 WHERE `entry` = 10390; #Cleave R3
UPDATE `creature_template` SET `spell3` = 16406 WHERE `entry` = 10390; #Rend R6
UPDATE `creature_template` SET `spell1` = 9734 WHERE `entry` = 787; #Holy Smite
UPDATE `creature_template` SET `spell2` = 6074 WHERE `entry` = 787; #Renew R2
UPDATE `creature_template` SET `spell1` = 7322 WHERE `entry` = 203; #Frostbolt R4
UPDATE `creature_template` SET `spell2` = 12486 WHERE `entry` = 203; #Chilled R3
UPDATE `creature_template` SET `spell1` = 7992 WHERE `entry` = 1110; #Slowing Poison
UPDATE `creature_template` SET `spell1` = 17312 WHERE `entry` = 7340; #Mind Flay R3
UPDATE `creature_template` SET `spell2` = 992 WHERE `entry` = 7340; #Shadow Word: Pain R4
UPDATE `creature_template` SET `spell3` = 984 WHERE `entry` = 7340; #Mind Blast R3
UPDATE `creature_template` SET `spell1` = 22940 WHERE `entry` = 1784; #Arcane Blast
UPDATE `creature_template` SET `spell2` = 1411 WHERE `entry` = 1784; #Firebolt 5
UPDATE `creature_template` SET `spell1` = 705 WHERE `entry` = 7342; #Shadow Bolt R3
UPDATE `creature_template` SET `spell2` = 1014 WHERE `entry` = 7342; #Curse of Agony R2
UPDATE `creature_template` SET `spell1` = 8699 WHERE `entry` = 785; #Unholy Frenzy
UPDATE `creature_template` SET `spell1` = 7373 WHERE `entry` = 48; #Hamstring R3

-- Updates past revison 16

-- Instances

-- Scarlet Monastery
-- 2953 - Fallen Champion
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6488);
UPDATE `creature_template` SET `spell1` = 19644  WHERE `entry` = 6488;
UPDATE `creature_template` SET `spell2` = 21949 WHERE `entry` = 6488;
UPDATE `creature_template` SET `spell3` = 19642 WHERE `entry` = 6488;

-- Creature scripts
UPDATE `creature_template` SET `ScriptName` = 'scarlet_torturer' WHERE `entry` IN (4306);

-- Boss Scripts
UPDATE `creature_template` SET `ScriptName` = 'boss_arcanist_doan' WHERE `entry` IN (6487);
UPDATE `creature_template` SET `ScriptName` = 'boss_azshir_the_sleepless' WHERE `entry` IN (6490);
UPDATE `creature_template` SET `ScriptName` = 'boss_bloodmage_thalnos' WHERE `entry` IN (4543);
UPDATE `creature_template` SET `ScriptName` = 'boss_herod' WHERE `entry` IN (3975);
UPDATE `creature_template` SET `ScriptName` = 'boss_high_inquisitor_fairbanks' WHERE `entry` IN (4542);
UPDATE `creature_template` SET `ScriptName` = 'boss_high_inquisitor_whitemane' WHERE `entry` IN (3977);
UPDATE `creature_template` SET `ScriptName` = 'boss_houndmaster_loksey' WHERE `entry` IN (3974);
UPDATE `creature_template` SET `ScriptName` = 'boss_interrogator_vishas' WHERE `entry` IN (3983);
UPDATE `creature_template` SET `ScriptName` = 'boss_scarlet_commander_mograine' WHERE `entry` IN (3976);
UPDATE `creature_template` SET `ScriptName` = 'boss_scorn' WHERE `entry` IN (14693);

-- Instances

-- Razorfen Downs by NuRRi
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (7358, 7357, 7356, 7355, 7354, 8567);

-- 7358 - Amnennar the Coldbringer
-- spell_list "10179 22645 13009 12556"
-- Frostbolt, Frost Nova, Amnennar's Wrath, Frost Armor
UPDATE `creature_template` SET `spell1` = 10179 WHERE `entry` = 7358;
UPDATE `creature_template` SET `spell2` = 22645 WHERE `entry` = 7358;
UPDATE `creature_template` SET `spell3` = 13009 WHERE `entry` = 7358;
UPDATE `creature_template` SET `spell4` = 12556 WHERE `entry` = 7358;

-- 7357 - Mordresh Fire Eye
-- spell_list "10148 12470"
-- Fireball, Firenova
UPDATE `creature_template` SET `spell1` = 10148 WHERE `entry` = 7357;
UPDATE `creature_template` SET `spell2` = 12470 WHERE `entry` = 7357;

-- 7356 - Plaguemaw The Rotting
-- spell_list "12947 12946"
-- Withered Touch, Putrid Stench
UPDATE `creature_template` SET `spell1` = 12947 WHERE `entry` = 7356;
UPDATE `creature_template` SET `spell2` = 12946 WHERE `entry` = 7356;

-- 7355 - TutenKash
-- spell_list "12255 12252 12254"
-- Curse of Tuten'kash, Web Spray, Virulent Poison Proc
UPDATE `creature_template` SET `spell1` = 12255 WHERE `entry` = 7355;
UPDATE `creature_template` SET `spell2` = 12252 WHERE `entry` = 7355;
UPDATE `creature_template` SET `spell3` = 12254 WHERE `entry` = 7355;


-- 7354 - Ragglesnout
-- spell_list "10892 11659"
-- Shadow Word: Pain, Shadow Bolt
UPDATE `creature_template` SET `spell1` = 10892 WHERE `entry` = 7354;
UPDATE `creature_template` SET `spell2` = 11659 WHERE `entry` = 7354;

-- 8567 - Glutton
-- spell_list "16345"
-- Disease Cloud
UPDATE `creature_template` SET `spell1` = 16345 WHERE `entry` = 8567;

-- Instances

-- Maraudon by NuRRi
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (12225, 12236, 12237, 12201, 12258, 13601, 13282);

-- 12225 - Celebras the Cursed
-- spell_list "21667 21331 21793"
-- Wrath, Entangling Roots, Twisted Tranquility
UPDATE `creature_template` SET `spell1` = 21667 WHERE `entry` = 12225;
UPDATE `creature_template` SET `spell2` = 21331 WHERE `entry` = 12225;
UPDATE `creature_template` SET `spell3` = 21793 WHERE `entry` = 12225;

-- 12236 - Lord Vyletongue
-- spell_list "21080 8817"
-- Putrid Breath, Smoke Bomb
UPDATE `creature_template` SET `spell1` = 21080 WHERE `entry` = 12236;
UPDATE `creature_template` SET `spell2` = 8817 WHERE `entry` = 12236;

-- 12237 - Meshlok The Harvester
-- spell_list "24375 15580"
-- War Stomp, Strike
UPDATE `creature_template` SET `spell1` = 24375 WHERE `entry` = 12237;
UPDATE `creature_template` SET `spell2` = 15580 WHERE `entry` = 12237;

-- 12201 - PrincessTheradras
-- spell_list "21909 21832 19128 21869"
-- Dust Field, Boulder, Knockdown, Repulsive Gaze
UPDATE `creature_template` SET `spell1` = 21909 WHERE `entry` = 12201;
UPDATE `creature_template` SET `spell2` = 21832 WHERE `entry` = 12201;
UPDATE `creature_template` SET `spell3` = 19128 WHERE `entry` = 12201;
UPDATE `creature_template` SET `spell4` = 21869 WHERE `entry` = 12201;

-- 12258 - Razorlash
-- spell_list "21911 15584 21749"
-- Puncture, Cleave, Thorn Volley
UPDATE `creature_template` SET `spell1` = 21911 WHERE `entry` = 12258;
UPDATE `creature_template` SET `spell2` = 15584 WHERE `entry` = 12258;
UPDATE `creature_template` SET `spell3` = 21749 WHERE `entry` = 12258;

-- 13601 - Tinkerer Gizlock
-- spell_list "21833 22334"
-- Goblin Dragon Gun, Bomb
UPDATE `creature_template` SET `spell1` = 21833 WHERE `entry` = 13601;
UPDATE `creature_template` SET `spell2` = 22334 WHERE `entry` = 13601;

-- 13282 - Noxxion
-- spell_list "21687 21547 21707"
-- Toxic Volley, Spore Cloud, Summon Spawn of Noxxion
UPDATE `creature_template` SET `spell1` = 21687 WHERE `entry` = 13282;
UPDATE `creature_template` SET `spell2` = 21547 WHERE `entry` = 13282;
UPDATE `creature_template` SET `spell3` = 21707 WHERE `entry` = 13282;

-- Instances

-- Gnomeregan
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (6206, 6208, 6209, 6215, 6219, 6229, 6228, 6235, 7361, 6407, 6220, 6218, 7603, 6223, 6222, 6234, 6233, 6226, 6227, 6225, 6230, 7079);

-- 6206 - Caverndeep Burrower
-- spell_list "9770 16145 1604 9776"
-- Radiation(9770), Sunder Armor(16145), Dazed(1604), Irradiated(9776)
UPDATE `creature_template` SET `spell1` = 9770 WHERE `entry` = 6206;
UPDATE `creature_template` SET `spell2` = 16145 WHERE `entry` = 6206;
UPDATE `creature_template` SET `spell3` = 1604 WHERE `entry` = 6206;
UPDATE `creature_template` SET `spell4` = 9776 WHERE `entry` = 6206;

-- 6208 - Caverndeep Invader
-- spell_list "9770 1604 12540"
-- Radiation(9770), Dazed(1604), Gouge(12540)
UPDATE `creature_template` SET `spell1` = 9770 WHERE `entry` = 6208;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6208;
UPDATE `creature_template` SET `spell3` = 12540 WHERE `entry` = 6208;

-- 6209 - Caverndeep Looter
-- spell_list "10851"
-- Grab Weapon
UPDATE `creature_template` SET `spell1` = 10851 WHERE `entry` = 6209;

-- 6215 - Chomper
-- spell_list "1604 6409 3420"
-- Dazed(1604), Cheap Shot(6409), Crippling Poison(3420)
-- Aura/Buff: Kirtonos Gargoyle Passive(18116)->Will be scripted
UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 6215;
UPDATE `creature_template` SET `spell2` = 6409 WHERE `entry` = 6215;
UPDATE `creature_template` SET `spell3` = 3420 WHERE `entry` = 6215;


-- 6219 - Corrosive Lurker
-- spell_list "9459 1604 10341 11638"
-- Corrosive Ooze(9459), Dazed(1604), Radiation Cloud(10341), Radiation Poisoning(11638)
UPDATE `creature_template` SET `spell1` = 9459 WHERE `entry` = 6219;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6219;
UPDATE `creature_template` SET `spell3` = 10341 WHERE `entry` = 6219;
UPDATE `creature_template` SET `spell4` = 11638 WHERE `entry` = 6219;

-- 6229 - Crowd Pummeler 9-60
-- spell_list "16169 5568 10887"
-- Arcing Smash(16169), Trample(5568), Crowd Pummel(10887)
-- Aura/Buff: Kirtonos Gargoyle Passive(18116)
UPDATE `creature_template` SET `spell1` = 16169 WHERE `entry` = 6229;
UPDATE `creature_template` SET `spell2` = 5568 WHERE `entry` = 6229;
UPDATE `creature_template` SET `spell3` = 10887 WHERE `entry` = 6229;

-- 6228 - Dark Iron Ambassador
-- spell_list "3053 16412"
-- FireShield Effect II(3052), Fireball(16412)
UPDATE `creature_template` SET `spell1` = 3053 WHERE `entry` = 6228;
UPDATE `creature_template` SET `spell2` = 16412 WHERE `entry` = 6228;

-- 6235 - Electrocutioner 6000
-- spell_list "11082 11085 11084"
-- Megavolt(11082), Chain Bolt(11085), Shock(11084)
-- Aura/Buff: Kirtonos Gargoyle Passive(18116) Will be scripted
UPDATE `creature_template` SET `spell1` = 11082 WHERE `entry` = 6235;
UPDATE `creature_template` SET `spell2` = 11085 WHERE `entry` = 6235;
UPDATE `creature_template` SET `spell3` = 11084 WHERE `entry` = 6235;

-- 7361 - Grubbis
-- spell_list "1604"
-- Dazed
UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 7361;

-- 6407 - Holdout Technician
-- spell_list "6660 8858"
-- Shoot(6660), Bomb(8858)
UPDATE `creature_template` SET `spell1` = 6660 WHERE `entry` = 6407;
UPDATE `creature_template` SET `spell2` = 8858 WHERE `entry` = 6407;

-- 6220 - Irradiated Horror
-- spell_list "8211 10341 1604 9459"
-- Chain Burn Rank 1(8211), Radiation Cloud(10341), Dazed(1604), Corrosive Ooze(9459)
UPDATE `creature_template` SET `spell1` = 8211 WHERE `entry` = 6220;
UPDATE `creature_template` SET `spell2` = 10341 WHERE `entry` = 6220;
UPDATE `creature_template` SET `spell3` = 1604 WHERE `entry` = 6220;
UPDATE `creature_template` SET `spell4` = 9459 WHERE `entry` = 6220;

-- 6218 - Irradiated Slime
-- spell_list "10341 1604 9459"
-- Radiation Cloud(10341), Dazed(1604), Corrosive Ooze(9459)
UPDATE `creature_template` SET `spell1` = 10341 WHERE `entry` = 6218;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6218;
UPDATE `creature_template` SET `spell3` = 9459 WHERE `entry` = 6218;

-- 7603 - Leprous Assistant
-- spell_list "1604 11264 12024"
-- Dazed(1604), Ice Blast(11264), Net(12024)
UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 7603;
UPDATE `creature_template` SET `spell2` = 11264 WHERE `entry` = 7603;
UPDATE `creature_template` SET `spell3` = 12024 WHERE `entry` = 7603;

-- 6223 - Leprous Defender
-- spell_list "6660 2643 5116"
-- Shoot(6660), Multi-Shot Rank 1(2643), Concussive Shot (5116)
UPDATE `creature_template` SET `spell1` = 6660 WHERE `entry` = 6223;
UPDATE `creature_template` SET `spell2` = 2643 WHERE `entry` = 6223;
UPDATE `creature_template` SET `spell3` = 5116 WHERE `entry` = 6223;

-- 6222 - Leprous Technician
-- spell_list "13398 1604 12024"
-- Throw Wrench(13398), Dazed(1604), Net(12024)
UPDATE `creature_template` SET `spell1` = 13398 WHERE `entry` = 6222;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6222;
UPDATE `creature_template` SET `spell3` = 12024 WHERE `entry` = 6222;

-- 6234 - Mechanized Guardian
-- spell_list "11820 1604"
-- Electrified Net(11820), Dazed(1604)
UPDATE `creature_template` SET `spell1` = 11820 WHERE `entry` = 6234;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6234;

-- 6233 - Mechanized Sentry
-- spell_list "1604"
-- Dazed(1604)
UPDATE `creature_template` SET `spell1` = 1604 WHERE `entry` = 6233;

-- 6226 - Mechano Flamewalker
-- spell_list "11306 10733"
-- Fire Nova Rank 4(11306), Flame Spray(10733)
UPDATE `creature_template` SET `spell1` = 11306 WHERE `entry` = 6226;
UPDATE `creature_template` SET `spell2` = 10733 WHERE `entry` = 6226;

-- 6227 - Mechano Flamewalker
-- spell_list "22519 11264"
-- Ice Nova(22519), Ice Blast(11264)
UPDATE `creature_template` SET `spell1` = 22519 WHERE `entry` = 6227;
UPDATE `creature_template` SET `spell2` = 11264 WHERE `entry` = 6227;

-- 6225 - Mechano Tank
-- spell_list "10346 1604 17174"
-- Machine Gun(10346), Dazed(1604), Concussive Shot(17174)
UPDATE `creature_template` SET `spell1` = 10346 WHERE `entry` = 6225;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6225;
UPDATE `creature_template` SET `spell3` = 17174 WHERE `entry` = 6225;

-- 6230 - Peacekeeper Security Suit
-- spell_list "6533 1604 11820 11084"
-- Net(6533), Dazed(1604), Electrified Net(11820), Shock(11084)
UPDATE `creature_template` SET `spell1` = 6533 WHERE `entry` = 6230;
UPDATE `creature_template` SET `spell2` = 1604 WHERE `entry` = 6230;
UPDATE `creature_template` SET `spell3` = 11820 WHERE `entry` = 6230;
UPDATE `creature_template` SET `spell4` = 11084 WHERE `entry` = 6230;

-- 7079 - Viscous Fallout
-- spell_list "10341 1604"
-- Radiation Cloud(10341), Dazed(1604)
UPDATE `creature_template` SET `spell1` = 10341 WHERE `entry` = 7079;
UPDATE `creature_template` SET `spell2` = 9459 WHERE `entry` = 7079;

-- Instances

-- Dire Maul
UPDATE `creature_template` SET `ScriptName` = 'generic_creature' WHERE `entry` IN (11492, 14325, 14324, 14321, 14326, 14323, 13280, 14122, 11488, 11496, 14396, 11501, 14327, 14506, 11487, 11486, 14354, 11498, 11489, 11490, 11444, 11450, 11441, 11457, 13197);

-- 11492 - Alzzin The Wildshaper 
-- Spell list - 22689(Mangle), 22662(Wither), 19319(Vicious Bite)
UPDATE `creature_template` SET `spell1` = 22689 WHERE `entry` = 11492;
UPDATE `creature_template` SET `spell2` = 22662 WHERE `entry` = 11492;
UPDATE `creature_template` SET `spell3` = 19319 WHERE `entry` = 11492;

-- 14325 - Captain Kromcrush
-- Spell list - 15708(Mortal Strike), 23511(Demoralizing Shout)
UPDATE `creature_template` SET `spell1` = 15708 WHERE `entry` = 14325;
UPDATE `creature_template` SET `spell2` = 23511 WHERE `entry` = 14325;

-- 14324 - Cho Rush The Observer
-- Spell list - 10947(Mind Blast), 10151(Fireball)
UPDATE `creature_template` SET `spell1` = 10947 WHERE `entry` = 14324;
UPDATE `creature_template` SET `spell2` = 10151 WHERE `entry` = 14324;

-- 14321 - Guard Fengus
-- Spell list - 15580(Strike), 15655(Shield Bash), 22572(Bruising Blow), 20691(Cleave)
UPDATE `creature_template` SET `spell1` = 15580 WHERE `entry` = 14321;
UPDATE `creature_template` SET `spell2` = 15655 WHERE `entry` = 14321;
UPDATE `creature_template` SET `spell3` = 22572 WHERE `entry` = 14321;
UPDATE `creature_template` SET `spell4` = 20691 WHERE `entry` = 14321;

-- 14326 - Guard Mol Dar
-- Spell list - 15580(Strike), 15655(Shield Bash), 20691(Cleave)
UPDATE `creature_template` SET `spell1` = 15580 WHERE `entry` = 14326;
UPDATE `creature_template` SET `spell2` = 15655 WHERE `entry` = 14326;
UPDATE `creature_template` SET `spell3` = 20691 WHERE `entry` = 14326;

-- 14323 - Guard Slip Kik
-- Spell list - 15580(Strike), 17307(Knockout), 20691(Cleave)
UPDATE `creature_template` SET `spell1` = 15580 WHERE `entry` = 14323;
UPDATE `creature_template` SET `spell2` = 17307 WHERE `entry` = 14323;
UPDATE `creature_template` SET `spell3` = 20691 WHERE `entry` = 14323;

-- 13280 - Hydrospawn
-- Spell list - 22419(Riptide), 22420(Submersion), 22421(Massive Geyser)
UPDATE `creature_template` SET `spell1` = 22419 WHERE `entry` = 13280;
UPDATE `creature_template` SET `spell2` = 22420 WHERE `entry` = 13280;
UPDATE `creature_template` SET `spell3` = 22421 WHERE `entry` = 13280;

-- 14122 - Massive Geyser
-- Spell list - 22422(Water)
UPDATE `creature_template` SET `spell1` = 22422 WHERE `entry` = 14122;

-- 11488 - Illyanna Ravenoak
-- Spell list - 5116(Concussive Shot), 20904(Aimed Shot [Rank 6]), 14290(Multi Shot [Rank 4]), 14295(Volley)
-- Note: Give her bounding radius >10 x <30 so she can shoot
UPDATE `creature_template` SET `spell1` = 5116 WHERE `entry` = 11488;
UPDATE `creature_template` SET `spell2` = 20904 WHERE `entry` = 11488;
UPDATE `creature_template` SET `spell3` = 14290 WHERE `entry` = 11488;
UPDATE `creature_template` SET `spell4` = 14295 WHERE `entry` = 11488;

-- 11496 - Immolthar
-- Spell list - 16128(Infected Bite), 15550(Trample), 22899(Eye of Immolthar)
UPDATE `creature_template` SET `spell1` = 16128 WHERE `entry` = 11496;
UPDATE `creature_template` SET `spell2` = 15550 WHERE `entry` = 11496;
UPDATE `creature_template` SET `spell3` = 22899 WHERE `entry` = 11496;

-- 14396 - Eye of Immolthar
-- Spell list - 22909(Eye of Immolthar[Debuff])
UPDATE `creature_template` SET `spell1` = 22909 WHERE `entry` = 14396;

-- 11501 - King Gordok
-- Spell list - 15708(War Stomp), 24375(Mortal Strike)
UPDATE `creature_template` SET `spell1` = 15708 WHERE `entry` = 11501;
UPDATE `creature_template` SET `spell2` = 24375 WHERE `entry` = 11501;

-- 14327 - Lethtendris
-- Spell list - 11668(Immolate), 14887(Shadow Bolt Volley)
UPDATE `creature_template` SET `spell1` = 11668 WHERE `entry` = 14327;
UPDATE `creature_template` SET `spell2` = 14887 WHERE `entry` = 14327;

-- 14506 - Lord Hel Nurath
-- Spell list - 10984(Shadow Word: Pain)
UPDATE `creature_template` SET `spell1` = 10984 WHERE `entry` = 14506;

-- 11487 - Magister Kalendris
--  spell_list "10894 10947 18807"
--  Shadow Word: Pain, Mind Blast, Mind Flay
UPDATE `creature_template` SET `spell1` = 10894 WHERE `entry` = 11487;
UPDATE `creature_template` SET `spell2` = 10947 WHERE `entry` = 11487;
UPDATE `creature_template` SET `spell3` = 18807 WHERE `entry` = 11487;

-- 11486 - Prince Tortheldrin
-- spell_list "20691 22920"
-- Cleave, Arcane Blast
UPDATE `creature_template` SET `spell1` = 20691 WHERE `entry` = 11486;
UPDATE `creature_template` SET `spell2` = 22920 WHERE `entry` = 11486;

-- 14354 - Pusillin
-- spell_list "22424 10151 16144"
-- Blast Wave, Fireball, Fire Blast
UPDATE `creature_template` SET `spell1` = 22424 WHERE `entry` = 14354;
UPDATE `creature_template` SET `spell2` = 10151 WHERE `entry` = 14354;
UPDATE `creature_template` SET `spell3` = 16144 WHERE `entry` = 14354;

-- 11498 - Skarr The Unbreakable
-- spell_list "20691 24375"
-- Cleave, Mortal Strike
UPDATE `creature_template` SET `spell1` = 20691 WHERE `entry` = 11498;
UPDATE `creature_template` SET `spell2` = 24375 WHERE `entry` = 11498;

-- 11489 - Tendris Warpwood
-- spell_list "15550 22924 22994"
-- Trample, Grasping Vines, Entangle
UPDATE `creature_template` SET `spell1` = 15550 WHERE `entry` = 11489;
UPDATE `creature_template` SET `spell2` = 22924 WHERE `entry` = 11489;
UPDATE `creature_template` SET `spell3` = 22994 WHERE `entry` = 11489;

-- 11490 - Zevrim Thornhoof
-- spell_list "22478 22651"
-- Intense Pain, Sacrifice
UPDATE `creature_template` SET `spell1` = 22478 WHERE `entry` = 11490;
UPDATE `creature_template` SET `spell2` = 22651 WHERE `entry` = 11490;

-- 11444 - Gordok Mage Lord
-- spell_list "20832 16102 15530 16170"
-- Fire Blast, Flamestrike, Frostbolt, Bloodlust
UPDATE `creature_template` SET `spell1` = 20832 WHERE `entry` = 11444;
UPDATE `creature_template` SET `spell2` = 16102 WHERE `entry` = 11444;
UPDATE `creature_template` SET `spell3` = 15530 WHERE `entry` = 11444;
UPDATE `creature_template` SET `spell4` = 16170 WHERE `entry` = 11444;

-- 11450 - Gordok Reaver
-- spell_list "22572 22916"
-- Bruising Blow, Uppercut
UPDATE `creature_template` SET `spell1` = 22572 WHERE `entry` = 11450;
UPDATE `creature_template` SET `spell2` = 22916 WHERE `entry` = 11450;

-- 11441 - Gordok Brute
-- spell_list "13737 20677 24317"
-- Mortal Strike, Cleave, Sunder Armor
UPDATE `creature_template` SET `spell1` = 13737 WHERE `entry` = 11441;
UPDATE `creature_template` SET `spell2` = 20677 WHERE `entry` = 11441;
UPDATE `creature_template` SET `spell3` = 24317 WHERE `entry` = 11441;

-- 11457 - Wildspawn Hellcaller
-- spell_list "19474" 
-- Rain of Fire
UPDATE `creature_template` SET `spell1` = 19474 WHERE `entry` = 11457;

-- 13197 - Fel Lash
-- spell_list "22460 22272"
-- Arcane Explosion, Arcane Missiles
UPDATE `creature_template` SET `spell1` = 22460 WHERE `entry` = 13197;
UPDATE `creature_template` SET `spell2` = 22272 WHERE `entry` = 13197;

--
-- Scholomance
--

-- Ash\'ari Crystal (id 10415) spells

UPDATE `creature_template` SET `spell1`=512, `spell2`=1010, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10415 ;


-- Aurius (id 10917) spells

UPDATE `creature_template` SET `spell1`=1096, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10917 ;


-- Balnazzar (id 10813) spells

UPDATE `creature_template` SET `spell1`=20603, `spell2`=15398, `spell3`=9256, `spell4`=20741, `scriptname`='generic_creature' WHERE `entry`=10813 ;


-- Baroness Anastari (id 10436) spells

UPDATE `creature_template` SET `spell1`=16565, `spell2`=16867, `spell3`=18327, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10436 ;


-- Bile Slime (id 10697) spells

UPDATE `creature_template` SET `spell1`=16866, `spell2`=1604, `spell3`=0, `spell4`=0,  `scriptname`='generic_creature' WHERE `entry`=10697 ;


-- Bile Spewer (id 10416) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=16866, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10416 ;


-- Black Guard Sentry (id 10394) spells

UPDATE `creature_template` SET `spell1`=17439, `spell2`=15584, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10394 ;


-- Black Guard Swordsmith (id 11121) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=5884, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11121 ;


-- Broken Cadaver (id 10383) spells

UPDATE `creature_template` SET `spell1`=16140, `spell2`=1604, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10383 ;


-- Cannibal Ghoul (id 8530) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=3589, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=8530 ;


-- Cannon Master Willey (id 10997) spells

UPDATE `creature_template` SET `spell1`=23382, `spell2`=15615, `spell3`=16496, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10997 ;


-- Crimson Battle Mage (id 10425) spells

UPDATE `creature_template` SET `spell1`=13020, `spell2`=23413, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10425 ;


-- Archivist Galford (id 10811) spells

UPDATE `creature_template` SET `spell1`=17293, `spell2`=23462, `spell3`=17274, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10811 ;


-- Crimson Bodyguard (id 13118) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=15749, `spell3`=11972, `spell4`=13534,  `scriptname`='generic_creature' WHERE `entry`=13118 ;


-- Crimson Conjuror (id 10419) spells

UPDATE `creature_template` SET `spell1`=11831, `spell2`=23412, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10419 ;


-- Crimson Courier (id 12337) spells

UPDATE `creature_template` SET `spell1`=5101, `spell2`=17143, `spell3`=13005, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12337 ;


-- Crimson Defender (id 10421) spells

UPDATE `creature_template` SET `spell1`=13005, `spell2`=1604, `spell3`=20005, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10421 ;


-- Crimson Gallant (id 10424) spells

UPDATE `creature_template` SET `spell1`=36647, `spell2`=17143, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10424 ;


-- Crimson Guardsman (id 10418) spells

UPDATE `creature_template` SET `spell1`=15749, `spell2`=33871, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10418 ;


-- Crimson Hammersmith (id 11120) spells

UPDATE `creature_template` SET `spell1`=17445, `spell2`=1604, `spell3`=39077, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11120 ;


-- Crimson Initiate (id 10420) spells

UPDATE `creature_template` SET `spell1`=17287, `spell2`=1604, `spell3`=22645, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10420 ;


-- Crimson Inquisitor (id 10426) spells

UPDATE `creature_template` SET `spell1`=22645, `spell2`=17165, `spell3`=22947, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10426 ;

-- Crimson Monk (id 11043) spells

UPDATE `creature_template` SET `spell1`=36033, `spell2`=17145, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11043 ;


-- Crimson Priest (id 10423) spells

UPDATE `creature_template` SET `spell1`=36176, `spell2`=36947, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10423 ;


-- Crimson Rifleman (id 11054) spells

UPDATE `creature_template` SET `spell1`=38094, `spell2`=1604, `spell3`=6788, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11054 ;


-- Crimson Sorcerer (id 10422) spells

UPDATE `creature_template` SET `spell1`=15451, `spell2`=16927, `spell3`=16144, `spell4`=15534, `scriptname`='generic_creature' WHERE `entry`=10422 ;


-- Crypt Beast (id 10413) spells

UPDATE `creature_template` SET `spell1`=22412, `spell2`=4962, `spell3`=1604, `spell4`=3589, `scriptname`='generic_creature' WHERE `entry`=10413 ;


-- Crypt Crawler (id 10412) spells

UPDATE `creature_template` SET `spell1`=15471, `spell2`=1604, `spell3`=3589, `spell4`=16430, `scriptname`='generic_creature' WHERE `entry`=10412 ;


-- Crypt Walker (id 8556) spells

UPDATE `creature_template` SET `spell1`=16401, `spell2`=12023, `spell3`=1604, `spell4`=20812, `scriptname`='generic_creature' WHERE `entry`=8556 ;


-- Doomguard (id 11859) spells

UPDATE `creature_template` SET `spell1`=4629, `spell2`=20549, `spell3`=20812, `spell4`=21949, `scriptname`='generic_creature' WHERE `entry`=11859 ;


-- Elder Farwhisper (id 15607) spells
UPDATE `creature_template` SET `spell1`=1604, `spell2`=15530, `spell3`=15531, `spell4`=16458, `scriptname`='generic_creature' WHERE `entry`=15607 ;


-- Flametongue Totem IV (id 10557) spells

UPDATE `creature_template` SET `spell1`=16388, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10557 ;


-- Fleshflayer Ghoul (id 10407) spells

UPDATE `creature_template` SET `spell1`=18200, `spell2`=1604, `spell3`=9791, `spell4`=16458, `scriptname`='generic_creature' WHERE `entry`=10407 ;


-- Fras Siabi (id 11058) spells

UPDATE `creature_template` SET `spell1`=7964, `spell2`=20712, `spell3`=1604, `spell4`=23511, `scriptname`='generic_creature' WHERE `entry`=11058 ;


-- Freed Soul (id 11136) spells

UPDATE `creature_template` SET `spell1`=16249, `spell2`=16143, `spell3`=16449, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11136 ;


-- Ghostly Citizen (id 10385) spells

UPDATE `creature_template` SET `spell1`=22687, `spell2`=1604, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10385 ;


-- Ghoul Ravener (id 10406) spells

UPDATE `creature_template` SET `spell1`=12538, `spell2`=15608, `spell3`=16172, `spell4`=1604, `scriptname`='generic_creature' WHERE `entry`=10406 ;


-- Grand Crusader Dathrohan (id 10812) spells

UPDATE `creature_template` SET `spell1`=17286, `spell2`=1604, `spell3`=20830, `spell4`=0, `scriptname`='' WHERE `entry`=10812 ;


-- Hate Shrieker (id 8541) spells
UPDATE `creature_template` SET `spell1`=3589, `spell2`=16867, `spell3`=1604, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=8541 ;


-- Jarien (id 16101) spells

UPDATE `creature_template` SET `spell1`=17685, `spell2`=18663, `spell3`=19643, `spell4`=20812, `scriptname`='generic_creature' WHERE `entry`=16101 ;


-- Magistrate Barthilas (id 10435) spells

UPDATE `creature_template` SET `spell1`=16793, `spell2`=10887, `spell3`=14099, `spell4`=1604, `scriptname`='generic_creature' WHERE `entry`=10435 ;


-- Maleki the Pallid (id 10438) spells

UPDATE `creature_template` SET `spell1`=15850, `spell2`=16249, `spell3`=20743, `spell4`=16869, `scriptname`='generic_creature' WHERE `entry`=10438 ;


-- Malor the Zealous (id 11032) spells

UPDATE `creature_template` SET `spell1`=12734, `spell2`=16172, `spell3`=1604, `spell4`=6788, `scriptname`='generic_creature' WHERE `entry`=11032 ;


-- Mangled Cadaver (id 10382) spells

UPDATE `creature_template` SET `spell1`=16143, `spell2`=1604, `spell3`=15043, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10382 ;


-- Mindless Skeleton (id 11197) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=6788, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11197 ;


-- Mindless Undead (id 11030) spells

UPDATE `creature_template` SET `spell1`=1604, `spell2`=6788, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11030 ;


-- Ramstein the Gorger (id 10439) spells

UPDATE `creature_template` SET `spell1`=5568, `spell2`=17307, `spell3`=1604, `spell4`=6788, `scriptname`='generic_creature' WHERE `entry`=10439 ;

--
-- Molten Core
--

-- Molten Giant (id 11658) spells

UPDATE `creature_template` SET `spell1`=19813, `spell2`=4283, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11658 ;


-- Firelord (id 11668) spells

UPDATE `creature_template` SET `spell1`=19393, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='mob_firelord' WHERE `entry`=11668 ;


-- Lava Surger (id 12101) spells

UPDATE `creature_template` SET `spell1`=25787, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12101 ;


-- Ancient Core Hound (id 11673) spells

UPDATE `creature_template` SET `spell1`=19364, `spell2`=19365, `spell3`=19369, `spell4`=19367, `scriptname`='generic_creature' WHERE `entry`=11673 ;


-- Molten Destroyer (id 11659) spells

UPDATE `creature_template` SET `spell1`=4283, `spell2`=19813, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11659 ;


-- Son of Flame (id 12143) spells

UPDATE `creature_template` SET `spell1`=20228, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12143 ;


-- Flame Imp (id 11669) spells

UPDATE `creature_template` SET `spell1`=20203, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11669 ;


-- Flamewaker Healer (id 11663) spells

UPDATE `creature_template` SET `spell1`=24668, `spell2`=23952, `spell3`=10960, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11663 ;


-- Flamewaker Priest (id 11662) spells

UPDATE `creature_template` SET `spell1`=23952, `spell2`=20294, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11662 ;


-- Flamewaker Protector (id 12119) spells

UPDATE `creature_template` SET `spell1`=20677, `spell2`=20740, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12119 ;


-- Flamewaker Elite (id 11664) spells

UPDATE `creature_template` SET `spell1`=20623, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11664 ;


-- Core Hound (id 11671) spells
UPDATE `creature_template` SET `spell1`=19778, `spell2`=19771, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11671 ;


-- Flamewaker (id 11661) spells

UPDATE `creature_template` SET `spell1`=21081, `spell2`=19730, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11661 ;


-- Flameguard (id 11667) spells

UPDATE `creature_template` SET `spell1`=19626, `spell2`=19630, `spell3`=19631, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11667 ;


-- Magmakin (id 12806) spells

UPDATE `creature_template` SET `spell1`=19798, `spell2`=19496, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12806 ;


-- Firewalker (id 11666) spells

UPDATE `creature_template` SET `spell1`=19635, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11666 ;


-- Lava Reaver (id 12100) spells

UPDATE `creature_template` SET `spell1`=19642, `spell2`=19644, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12100 ;


-- Lava Elemental (id 12076) spells

UPDATE `creature_template` SET `spell1`=19641, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12076 ;


-- Firesworn (id 12099) spells

UPDATE `creature_template` SET `spell1`=20294, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12099 ;


-- Lava Spawn (id 12265) spells

UPDATE `creature_template` SET `spell1`=18108, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=12265 ;


-- Core Rager (id 11672) spells

UPDATE `creature_template` SET `spell1`=22689, `spell2`=19319, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11672 ;

--
-- UBRS
--


-- Blackhand Incarcerator (id 10316) spells

UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='' WHERE `entry`=10316 ;


-- Jed Runewatcher (id 10509) spells

UPDATE `creature_template` SET `spell1`=35178, `spell2`=15749, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10509 ;


-- Blackhand Veteran (id 9819) spells

UPDATE `creature_template` SET `spell1`=35178, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=9819 ;


-- Blackhand Iron Guard (id 10319) spells

UPDATE `creature_template` SET `spell1`=30688, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10319 ;


-- Blackhand Dreadweaver (id 9817) spells

UPDATE `creature_template` SET `spell1`=20816, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=9817 ;


-- Blackhand Summoner (id 9818) spells

UPDATE `creature_template` SET `spell1`=32192, `spell2`=18108, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=9818 ;

-- Pyroguard Emberseer (id 9816) spells

UPDATE `creature_template` SET `spell1`=23462, `spell2`=23341, `spell3`=17274, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=9816 ;


-- Rookery Guardian (id 10258) spells

UPDATE `creature_template` SET `spell1`=24317, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10258 ;


-- Rookery Whelp (id 10161) spells

UPDATE `creature_template` SET `spell1`=145, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10161 ;


-- Rookery Hatcher (id 10683) spells

UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='' WHERE `entry`=10683 ;


-- Solakar Flamewreath (id 10264) spells

UPDATE `creature_template` SET `spell1`=28725, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10264 ;


-- Goraluk Anvilcrack (id 10899) spells

UPDATE `creature_template` SET `spell1`=12461, `spell2`=22591, `spell3`=16172, `spell4`=24317, `scriptname`='generic_creature' WHERE `entry`=10899 ;


-- Blackhand Elite (id 10317) spells

UPDATE `creature_template` SET `spell1`=12461, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10317 ;


-- Blackhand Thug (id 10762) spells

UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='' WHERE `entry`=10762 ;


-- Blackhand Dragon Handler (id 10742) spells

UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='' WHERE `entry`=10742 ;


-- Chromatic Dragonspawn (id 10447) spells

UPDATE `creature_template` SET `spell1`=30014, `spell2`=32370, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10447 ;


-- Chromatic Whelp (id 10442) spells

UPDATE `creature_template` SET `spell1`=22414, `spell2`=23512, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10442 ;


-- Warchief Rend Blackhand (id 10429) spells

UPDATE `creature_template` SET `spell1`=20691, `spell2`=15589, `spell3`=23931, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10429 ;


-- Gyth (id 10339) spells

UPDATE `creature_template` SET `spell1`=20667, `spell2`=20712, `spell3`=18763, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10339 ;


-- Blackhand Assassin (id 10318) spells

UPDATE `creature_template` SET `spell1`=11286, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10318 ;


-- Rage Talon Fire Tongue (id 10372) spells

UPDATE `creature_template` SET `spell1`=23462, `spell2`=25668, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10372 ;


-- Rage Talon Captain (id 10371) spells

UPDATE `creature_template` SET `spell1`=28168, `spell2`=27580, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10371 ;


-- Rage Talon Flamescale (id 10083) spells

UPDATE `creature_template` SET `spell1`=23462, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10083 ;


-- Lord Victor Nefarius (id 10162) spells

UPDATE `creature_template` SET `spell1`=20741, `spell2`=24668, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10162 ;


-- The Beast (id 10430) spells

UPDATE `creature_template` SET `spell1`=17883, `spell2`=16785, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10430 ;


-- General Drakkisath (id 10363) spells

UPDATE `creature_template` SET `spell1`=23462, `spell2`=20691, `spell3`=23023, `spell4`=23931, `scriptname`='generic_creature' WHERE `entry`=10363 ;


-- Chromatic Elite Guard (id 10814) spells

UPDATE `creature_template` SET `spell1`=20276, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10814 ;


--
-- Scholomance
--


-- Risen Guardian (id 11598) spells

UPDATE `creature_template` SET `spell1`=13496, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11598 ;


-- Risen Protector (id 10487) spells

UPDATE `creature_template` SET `spell1`=27559, `spell2`=20691, `spell3`=12020, `spell4`=30138, `scriptname`='generic_creature' WHERE `entry`=10487 ;


-- Risen Bonewarder (id 10491) spells

UPDATE `creature_template` SET `spell1`=17715, `spell2`=27559, `spell3`=20276, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10491 ;


-- Scholomance Neophyte (id 10470) spells

UPDATE `creature_template` SET `spell1`=11660, `spell2`=18807, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10470 ;


-- Reanimated Corpse (id 10481) spells

UPDATE `creature_template` SET `spell1`=18270, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10481 ;


-- Marduk Blackpool (id 10433) spells

UPDATE `creature_template` SET `spell1`=18116, `spell2`=17695, `spell3`=20741, `spell4`=15584, `scriptname`='generic_creature' WHERE `entry`=10433 ;


-- Risen Lackey (id 10482) spells

UPDATE `creature_template` SET `spell1`=17472, `spell2`=15584, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10482 ;


-- Doctor Theolen Krastinov (id 11261) spells

UPDATE `creature_template` SET `spell1`=18106, `spell2`=18116, `spell3`=15584, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11261 ;


-- Kirtonos the Herald (id 10506) spells

UPDATE `creature_template` SET `spell1`=18116, `spell2`=20741, `spell3`=18144, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10506 ;


-- Instructor Malicia (id 10505) spells

UPDATE `creature_template` SET `spell1`=11672, `spell2`=12020, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10505 ;


-- Lorekeeper Polkelt (id 10901) spells

UPDATE `creature_template` SET `spell1`=16359, `spell2`=3584, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10901 ;


-- Rattlegore (id 11622) spells

UPDATE `creature_template` SET `spell1`=24375, `spell2`=18813, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11622 ;


-- The Ravenian (id 10507) spells

UPDATE `creature_template` SET `spell1`=15550, `spell2`=20691, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10507 ;


-- Lord Alexei Barov (id 10504) spells

UPDATE `creature_template` SET `spell1`=11668, `spell2`=11700, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10504 ;


-- Jandice Barov (id 10503) spells

UPDATE `creature_template` SET `spell1`=24673, `spell2`=18270, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10503 ;


-- Illusion of Jandice Barov (id 11439) spells

UPDATE `creature_template` SET `spell1`=15584, `spell2`=17472, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11439 ;


-- Lady Illucia Barov (id 10502) spells

UPDATE `creature_template` SET `spell1`=18116, `spell2`=19460, `spell3`=6215, `spell4`=15487, `scriptname`='generic_creature' WHERE `entry`=10502 ;


-- Darkmaster Gandling (id 1853) spells

UPDATE `creature_template` SET `spell1`=18702, `spell2`=10212, `spell3`=18116, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=1853 ;


-- Ras Frostwhisper (id 10508) spells

UPDATE `creature_template` SET `spell1`=16350, `spell2`=21369, `spell3`=8398, `spell4`=18033, `scriptname`='generic_creature' WHERE `entry`=10508 ;


-- Blood Steward of Kirtonos (id 14861) spells
UPDATE `creature_template` SET `spell1`=27224, `spell2`=30900, `spell3`=3609, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14861 ;


-- Spectral Tutor (id 10498) spells

UPDATE `creature_template` SET `spell1`=18278, `spell2`=29405, `spell3`=12020, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10498 ;


-- Diseased Ghoul (id 10495) spells

UPDATE `creature_template` SET `spell1`=24063, `spell2`=18270, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10495 ;


-- Spectral Projection (id 11263) spells

UPDATE `creature_template` SET `spell1`=12020, `spell2`=18278, `spell3`=25304, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11263 ;


-- Necrofiend (id 11551) spells

UPDATE `creature_template` SET `spell1`=16469, `spell2`=25349, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11551 ;


-- Scholomance Necromancer (id 10477) spells

UPDATE `creature_template` SET `spell1`=25304, `spell2`=11660, `spell3`=12020, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10477 ;


-- Scholomance Occultist (id 10472) spells

UPDATE `creature_template` SET `spell1`=25304, `spell2`=12020, `spell3`=10161, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10472 ;


-- Scholomance Student (id 10475) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10475 ;


-- Scholomance Adept (id 10469) spells

UPDATE `creature_template` SET `spell1`=25304, `spell2`=10161, `spell3`=10473, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10469 ;


-- Dark Shade (id 11284) spells

UPDATE `creature_template` SET `spell1`=11660, `spell2`=15571, `spell3`=10161, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11284 ;


-- Unstable Corpse (id 10480) spells

UPDATE `creature_template` SET `spell1`=17689, `spell2`=15571, `spell3`=18270, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10480 ;


-- Ragged Ghoul (id 10497) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=30138, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10497 ;


-- Skulking Corpse (id 10479) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=30138, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10479 ;


-- Malicious Spirit (id 14513) spells

UPDATE `creature_template` SET `spell1`=17504, `spell2`=12867, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14513 ;


-- Aspect of Malice (id 14520) spells

UPDATE `creature_template` SET `spell1`=17504, `spell2`=12867, `spell3`=24673, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14520 ;


-- Aspect of Shadow (id 14521) spells

UPDATE `creature_template` SET `spell1`=30138, `spell2`=11660, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14521 ;


-- Aspect of Banality (id 14518) spells

UPDATE `creature_template` SET `spell1`=11556, `spell2`=8140, `spell3`=23262, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14518 ;

-- Aspect of Corruption (id 14519) spells

UPDATE `creature_template` SET `spell1`=10894, `spell2`=23244, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14519 ;


-- Banal Spirit (id 14514) spells

UPDATE `creature_template` SET `spell1`=11556, `spell2`=8140, `spell3`=15571, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14514 ;


-- Corrupted Spirit (id 14512) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=23244, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=14512 ;


-- Risen Construct (id 10488) spells

UPDATE `creature_template` SET `spell1`=12292, `spell2`=15571, `spell3`=28168, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10488 ;


-- Risen Aberration (id 10485) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=24063, `spell3`=18270, `spell4`=12021, `scriptname`='generic_creature' WHERE `entry`=10485 ;


-- Scholomance Handler (id 11257) spells

UPDATE `creature_template` SET `spell1`=10161, `spell2`=13021, `spell3`=20883, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=11257 ;


-- Splintered Skeleton (id 10478) spells

UPDATE `creature_template` SET `spell1`=15571, `spell2`=0, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10478 ;


-- Risen Warrior (id 10486) spells

UPDATE `creature_template` SET `spell1`=17504, `spell2`=26554, `spell3`=35011, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10486 ;


-- Vectus (id 10432) spells

UPDATE `creature_template` SET `spell1`=19627, `spell2`=18816, `spell3`=13021, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10432 ;


-- Scholomance Acolyte (id 10471) spells

UPDATE `creature_template` SET `spell1`=18807, `spell2`=10876, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10471 ;


-- Spectral Teacher (id 10500) spells

UPDATE `creature_template` SET `spell1`=10876, `spell2`=25304, `spell3`=18647, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10500 ;


-- Risen Guard (id 10489) spells

UPDATE `creature_template` SET `spell1`=29684, `spell2`=25051, `spell3`=0, `spell4`=0, `scriptname`='generic_creature' WHERE `entry`=10489 ;

-- 
-- BLACK WING LAIR
--
-- Razorgore the Untamed 12435
UPDATE `creature_template` SET`ScriptName` = 'boss_razorgore' WHERE `entry` = 12435;

-- Vaelastrasz the corrupt 13020
UPDATE `creature_template` SET`ScriptName` = 'boss_vaelastrasz' WHERE `entry` = 13020;
UPDATE creature_template SET faction=32 WHERE entry=13020;
UPDATE creature_template SET npcflag=65 WHERE entry=13020;

-- Broodlord 12017
UPDATE `creature_template` SET`ScriptName` = 'boss_broodlord' WHERE `entry` = 12017;

-- Firemaw 11983
UPDATE `creature_template` SET`ScriptName` = 'boss_firemaw' WHERE `entry` = 11983;

-- Ebonroc 14601
UPDATE `creature_template` SET`ScriptName` = 'boss_ebonroc' WHERE `entry` = 14601;

-- Flamegor 11981
UPDATE `creature_template` SET`ScriptName` = 'boss_flamegor' WHERE `entry` = 11981;

-- Chromaggus 14020
UPDATE `creature_template` SET`ScriptName` = 'boss_chromaggus' WHERE `entry` = 14020;

-- Victor Nefarius 11583
UPDATE `creature_template` SET`ScriptName` = 'boss_victor_nefarius' WHERE `entry` = 10162;

-- Nefarian 11583
UPDATE `creature_template` SET`ScriptName` = 'boss_nefarian' WHERE `entry` = 11583;
DELETE FROM `creature` WHERE id=11583;

--
-- NAXX
--

-- Anubrekhan 15956
UPDATE `creature_template` SET`ScriptName` = 'boss_anubrekhan' WHERE `entry` = 15956;

-- Faerlina 15953
UPDATE `creature_template` SET`ScriptName` = 'boss_faerlina' WHERE `entry` = 15953;

-- Maexxna 15952
UPDATE `creature_template` SET`ScriptName` = 'boss_maexxna' WHERE `entry` = 15952;

-- Noth 15954
UPDATE `creature_template` SET`ScriptName` = 'boss_noth' WHERE `entry` = 15954;

-- Heigan 15936
UPDATE `creature_template` SET`ScriptName` = 'boss_heigan' WHERE `entry` = 15936;

-- Loatheb 16011
UPDATE `creature_template` SET`ScriptName` = 'boss_loatheb' WHERE `entry` = 16011;

-- Razuvios 16061
UPDATE `creature_template` SET`ScriptName` = 'boss_razuvious' WHERE `entry` = 16061;

-- Gothik 16060
UPDATE `creature_template` SET`ScriptName` = 'boss_gothik' WHERE `entry` = 16060;

-- Highlord Mograine 16062
UPDATE `creature_template` SET`ScriptName` = 'boss_highlord_mograine' WHERE `entry` = 16062;

-- Thane Korth'azz 16064
UPDATE `creature_template` SET`ScriptName` = 'boss_thane_korthazz' WHERE `entry` = 16064;

-- Sir Zeliek 16063
UPDATE `creature_template` SET`ScriptName` = 'boss_sir_zeliek' WHERE `entry` = 16063;

-- Lady Blaumeux 16065
UPDATE `creature_template` SET`ScriptName` = 'boss_lady_blaumeux' WHERE `entry` = 16065;

-- Patchwerk 16028
UPDATE `creature_template` SET`ScriptName` = 'boss_patchwerk' WHERE `entry` = 16028;

-- Grobbulus 15931
UPDATE `creature_template` SET`ScriptName` = 'boss_grobbulus' WHERE `entry` = 15931;

-- Gluth 15932
UPDATE `creature_template` SET`ScriptName` = 'boss_gluth' WHERE `entry` = 15932;

-- Thaddius 15928
UPDATE `creature_template` SET`ScriptName` = 'boss_thaddius' WHERE `entry` = 15928;

-- Stalagg 15929
UPDATE `creature_template` SET`ScriptName` = 'boss_stalagg' WHERE `entry` = 15929;

-- Fugen 15930
UPDATE `creature_template` SET`ScriptName` = 'boss_fugen' WHERE `entry` = 15930;

-- Sapphiron 15989
UPDATE `creature_template` SET`ScriptName` = 'boss_sapphiron' WHERE `entry` = 15989;

-- Kel'Thuzad 15990
UPDATE `creature_template` SET`ScriptName` = 'boss_kelthuzad' WHERE `entry` = 15990;

-- Injured Draenei (Azuremyst isle)
UPDATE `creature_template` set `ScriptName` = 'injured_draenei' where `entry` = 16971;

-- Blackrock Depths
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

-- Lower Blackrock Spire
UPDATE `creature_template` SET `ScriptName` = 'boss_highlord_omokk' WHERE `entry` = 9196;
UPDATE `creature_template` SET `ScriptName` = 'boss_shadow_hunter_voshgajin' WHERE `entry` = 9236;
UPDATE `creature_template` SET `ScriptName` = 'boss_warmaster_voone' WHERE `entry` = 9237;
UPDATE `creature_template` SET `ScriptName` = 'boss_mother_smolderweb' WHERE `entry` = 10596;
UPDATE `creature_template` SET `ScriptName` = 'quartermaster_zigris' WHERE `entry` = 9736;
UPDATE `creature_template` SET `ScriptName` = 'boss_halycon' WHERE `entry` = 10220;
UPDATE `creature_template` SET `ScriptName` = 'boss_overlord_wyrmthalak' WHERE `entry` = 9568;

-- Upper Blackrock Spire
UPDATE `creature_template` SET `ScriptName` = 'boss_the_beast' WHERE `entry` = 10430;
UPDATE `creature_template` SET `ScriptName` = 'boss_drakkisath' WHERE `entry` = 10363;

-- Scholomance
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

-- Maraudon
UPDATE `creature_template` SET `ScriptName` = 'boss_landslide' WHERE `entry` = 12203;
UPDATE `creature_template` SET `ScriptName` = 'boss_lord_vyletongue' WHERE `entry` = 12236;
UPDATE `creature_template` SET `ScriptName` = 'boss_noxxion' WHERE `entry` = 13282;
UPDATE `creature_template` SET `ScriptName` = 'boss_princess_theradras' WHERE `entry` = 12201;
UPDATE `creature_template` SET `ScriptName` = 'boss_rotgrip' WHERE `entry` = 13596;
UPDATE `creature_template` SET `ScriptName` = 'boss_tinkerer_gizlock' WHERE `entry` = 13601;
UPDATE `creature_template` SET `ScriptName` = 'celebras_the_cursed' WHERE `entry` = 12225;
UPDATE `creature_template` SET `ScriptName` = 'meshlok_the_harvester' WHERE `entry` = 12237;
UPDATE `creature_template` SET `ScriptName` = 'razorlash' WHERE `entry` = 12258;

-- Razorfen Downs
UPDATE `creature_template` SET `ScriptName` = 'boss_amnennar_the_coldbringer' WHERE `entry` = 7358;

-- World Bosses
UPDATE `creature_template` SET `ScriptName` = 'boss_ysondre' WHERE `entry` = 14887;
UPDATE `creature_template` SET `ScriptName` = 'boss_emeriss' WHERE `entry` = 14889;
UPDATE `creature_template` SET `ScriptName` = 'boss_taerar' WHERE `entry` = 14890;
UPDATE `creature_template` SET `ScriptName` = 'boss_shade_of_taerar' WHERE `entry` = 15302;
UPDATE `creature_template` SET `ScriptName` = 'boss_teremus_the_devourer' WHERE `entry` = 7846;
UPDATE `creature_template` SET `ScriptName` = 'boss_kruul' WHERE `entry` = 18338;

-- Burning Crusade
UPDATE `creature_template` SET `ScriptName` = 'boss_doomlord_kazzak' WHERE `entry` = 18728;

-- Stratholme
UPDATE `creature_template` SET `ScriptName` = 'boss_fras_siabi' WHERE `entry` = 11058;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 11058;
UPDATE `creature_template` SET `ScriptName` = 'boss_skul' WHERE `entry` = 10393;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10393;

UPDATE `creature_template` SET `ScriptName` = 'boss_hearthsinger_forresten' WHERE `entry` = 10558;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10558;

UPDATE `creature_template` SET `ScriptName` = 'boss_magistrate_barthilas' WHERE `entry` = 10435;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10435;
UPDATE `creature_template` SET `size` = '1.50' WHERE `entry` = 10435;

UPDATE `creature_template` SET `ScriptName` = 'boss_maleki_the_pallid' WHERE `entry` = 10438;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10438;
UPDATE `creature_template` SET `ScriptName` = 'boss_nerubenkan' WHERE `entry` = 10437;
UPDATE `creature_template` SET `speed` = '1.25' WHERE `entry` = 10437;
UPDATE `creature_template` SET `ScriptName` = 'boss_cannon_master_willey' WHERE `entry` = 10997;
UPDATE `creature_template` SET `ScriptName` = 'boss_the_unforgiven' WHERE `entry` = 10516;
UPDATE `creature_template` SET `speed` = '1.20' WHERE `entry` = 10516;
UPDATE `creature_template` SET `ScriptName` = 'boss_baroness_anastari' WHERE `entry` = 10436;
UPDATE `creature_template` SET `speed` = '1.10' WHERE `entry` = 10436;
UPDATE `creature_template` SET `ScriptName` = 'boss_archivist_galford' WHERE `entry` = 10811;
UPDATE `creature_template` SET `speed` = '1.10' WHERE `entry` = 10811;
UPDATE `creature_template` SET `ScriptName` = 'boss_crimson_hammersmith' WHERE `entry` = 11120;
UPDATE `creature_template` SET `size` = '1.50' WHERE `entry` = 11120;
UPDATE `creature_template` SET `ScriptName` = 'boss_ramstein_the_gorger' WHERE `entry` = 10439;
UPDATE `creature_template` SET `speed` = '0.75' WHERE `entry` = 10439;
UPDATE `creature_template` SET `size` = '1.85' WHERE `entry` = 10439;
UPDATE `creature_template` SET `ScriptName` = 'boss_malor_the_zealous' WHERE `entry` = 11032;
UPDATE `creature_template` SET `speed` = '1.10' WHERE `entry` = 11032;
UPDATE `creature_template` SET `ScriptName` = 'boss_black_guard_swordsmith' WHERE `entry` = 11121;
UPDATE `creature_template` SET `size` = '1.50' WHERE `entry` = 11121;
UPDATE `creature_template` SET `ScriptName` = 'boss_timmy_the_cruel' WHERE `entry` = 10808;
UPDATE `creature_template` SET `ScriptName` = 'boss_stonespine' WHERE `entry` = 10809;
UPDATE `creature_template` SET `speed` = '0.90' WHERE `entry` = 10809;
UPDATE `creature_template` SET `ScriptName` = 'boss_postmaster_malown' WHERE `entry` = 11143;
UPDATE `creature_template` SET `ScriptName` = 'boss_baron_rivendare' WHERE `entry` = 10440;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_guardsman' WHERE `entry` = 10418;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10418;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 10418;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_conjuror' WHERE `entry` = 10419;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10419;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 10419;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_initiate' WHERE `entry` = 10420;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10420;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 10420;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_defender' WHERE `entry` = 10421;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10421;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 10421;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_sorcerer' WHERE `entry` = 10422;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10422;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 10422;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_priest' WHERE `entry` = 10423;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10423;
UPDATE `creature_template` SET `size` = '1.00' WHERE `entry` = 10423;
UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_gallant' WHERE `entry` = 10424;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10424;
UPDATE `creature_template` SET `size` = '1.00' WHERE `entry` = 10424;

UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_battle_mage' WHERE `entry` = 10425;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10425;
UPDATE `creature_template` SET `size` = '1.20' WHERE `entry` = 10425;

UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_inquisitor' WHERE `entry` = 10426;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10426;
UPDATE `creature_template` SET `size` = '1.15' WHERE `entry` = 10426;

UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_monk' WHERE `entry` = 11043;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 11043;

UPDATE `creature_template` SET `size` = '0.90' WHERE `entry` = 11043;

UPDATE `creature_template` SET `ScriptName` = 'mob_crimson_rifleman' WHERE `entry` = 11054;
UPDATE `creature_template` SET `speed` = '1.00' WHERE `entry` = 11054;
UPDATE `creature_template` SET `size` = '1.10' WHERE `entry` = 11054;

UPDATE `creature_template` SET `ScriptName` = 'mob_bile_spewer' WHERE `entry` = 10416;
UPDATE `creature_template` SET `speed` = '0.80' WHERE `entry` = 10416;
UPDATE `creature_template` SET `size` = '1.55' WHERE `entry` = 10416;
UPDATE `creature_template` SET `ScriptName` = 'mob_bile_slime' WHERE `entry` = 10697;
UPDATE `creature_template` SET `speed` = '0.65' WHERE `entry` = 10697;
UPDATE `creature_template` SET `ScriptName` = 'mob_black_guard_sentry' WHERE `entry` = 10394;
UPDATE `creature_template` SET `speed` = '1.20' WHERE `entry` = 10394;
UPDATE `creature_template` SET `size` = '1.40' WHERE `entry` = 10394;
UPDATE `creature_template` SET `ScriptName` = 'mob_ghoul_ravener' WHERE `entry` = 10406;
UPDATE `creature_template` SET `speed` = '1.20' WHERE `entry` = 10406;
UPDATE `creature_template` SET `ScriptName` = 'mob_fleshflayer_ghoul' WHERE `entry` = 10407;
UPDATE `creature_template` SET `speed` = '1.20' WHERE `entry` = 10407;
UPDATE `creature_template` SET `ScriptName` = 'mob_plague_ghoul' WHERE `entry` = 10405;
UPDATE `creature_template` SET `speed` = '1.20' WHERE `entry` = 10405;
UPDATE `creature_template` SET `ScriptName` = 'mob_eye_of_naxxramas' WHERE `entry` = 10411;
UPDATE `creature_template` SET `speed` = '0.80' WHERE `entry` = 10411;
UPDATE `creature_template` SET `ScriptName` = 'mob_rockwing_screecher' WHERE `entry` = 10409;
UPDATE `creature_template` SET `speed` = '0.90' WHERE `entry` = 10409;
UPDATE `creature_template` SET `ScriptName` = 'mob_rockwing_gargoyle' WHERE `entry` = 10408;
UPDATE `creature_template` SET `speed` = '0.90' WHERE `entry` = 10408;
UPDATE `creature_template` SET `ScriptName` = 'mob_venom_belcher' WHERE `entry` = 10417;
UPDATE `creature_template` SET `speed` = '0.80' WHERE `entry` = 10417;
UPDATE `creature_template` SET `size` = '1.45' WHERE `entry` = 10417;
UPDATE `creature_template` SET `ScriptName` = 'mob_patchwork_horror' WHERE `entry` = 10414;
UPDATE `creature_template` SET `speed` = '0.80' WHERE `entry` = 10414;
UPDATE `creature_template` SET `ScriptName` = 'mob_wailing_banshee' WHERE `entry` = 10464;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10464;
UPDATE `creature_template` SET `ScriptName` = 'mob_shrieking_banshee' WHERE `entry` = 10463;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10463;
UPDATE `creature_template` SET `ScriptName` = 'mob_vengeful_phantom' WHERE `entry` = 10387;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10387;
UPDATE `creature_template` SET `ScriptName` = 'mob_wrath_phantom' WHERE `entry` = 10389;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10389;
UPDATE `creature_template` SET `ScriptName` = 'mob_spiteful_phantom' WHERE `entry` = 10388;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10388;
UPDATE `creature_template` SET `ScriptName` = 'mob_crypt_beast' WHERE `entry` = 10413;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10413;
UPDATE `creature_template` SET `ScriptName` = 'mob_crypt_crawler' WHERE `entry` = 10412;
UPDATE `creature_template` SET `speed` = '1.15' WHERE `entry` = 10412;
UPDATE `creature_template` SET `ScriptName` = 'mob_undead_scarab' WHERE `entry` = 10876;
UPDATE `creature_template` SET `speed` = '0.50' WHERE `entry` = 10876;
UPDATE `creature_template` SET `size` = '0.30' WHERE `entry` = 10876;
UPDATE `creature_template` SET `ScriptName` = 'mob_skeletal_guardian' WHERE `entry` = 10390;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10390;
UPDATE `creature_template` SET `ScriptName` = 'mob_skeletal_berserker' WHERE `entry` = 10391;
UPDATE `creature_template` SET `speed` = '1.10' WHERE `entry` = 10391;
UPDATE `creature_template` SET `ScriptName` = 'mob_mangled_cadaver' WHERE `entry` = 10382;
UPDATE `creature_template` SET `speed` = '0.70' WHERE `entry` = 10382;
UPDATE `creature_template` SET `ScriptName` = 'mob_ravaged_cadaver' WHERE `entry` = 10381;
UPDATE `creature_template` SET `speed` = '0.70' WHERE `entry` = 10381;
UPDATE `creature_template` SET `ScriptName` = 'mob_broken_cadaver' WHERE `entry` = 10383;
UPDATE `creature_template` SET `speed` = '0.70' WHERE `entry` = 10383;
UPDATE `creature_template` SET `ScriptName` = 'mob_mindless_skeleton' WHERE `entry` = 11197;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_acolyte' WHERE `entry` = 10399;
UPDATE `creature_template` SET `speed` = '1.00' WHERE `entry` = 10399;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_necromancer' WHERE `entry` = 10400;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10400;
UPDATE `creature_template` SET `ScriptName` = 'mob_thuzadin_shadowcaster' WHERE `entry` = 10398;
UPDATE `creature_template` SET `speed` = '1.05' WHERE `entry` = 10398;
UPDATE `creature_template` SET `ScriptName` = 'mob_spectral_citizen' WHERE `entry` = 10384;
UPDATE `creature_template` SET `speed` = '1.00' WHERE `entry` = 10384;
UPDATE `creature_template` SET `ScriptName` = 'mob_ghostly_citizen' WHERE `entry` = 10385;
UPDATE `creature_template` SET `speed` = '1.00' WHERE `entry` = 10385;
UPDATE `creature_template` SET `ScriptName` = 'mob_restless_soul' WHERE `entry` = 11122;
UPDATE `creature_template` SET `speed` = '1.25' WHERE `entry` = 11122;
UPDATE `creature_template` SET `ScriptName` = 'mob_freed_soul' WHERE `entry` = 11136;
UPDATE `creature_template` SET `speed` = '0.55' WHERE `entry` = 11136;