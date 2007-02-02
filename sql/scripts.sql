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
UPDATE `creature_template` SET`ScriptName` = 'boss_shazzra' WHERE `entry` = 12264;
UPDATE `creature_template` SET`ScriptName` = 'boss_golemagg' WHERE `entry` = 11988;
UPDATE `creature_template` SET`ScriptName` = 'boss_sulfuron' WHERE `entry` = 12098;
UPDATE `creature_template` SET`ScriptName` = 'boss_majordomo' WHERE `entry` = 12018;
UPDATE `creature_template` SET`ScriptName` = 'boss_ragnaros' WHERE `entry` = 11502;

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
UPDATE `creature_template` SET `spell1` = 568 WHERE `entry` = 1784; #Arcane Burst
UPDATE `creature_template` SET `spell2` = 1411 WHERE `entry` = 1784; #Firebolt 5
UPDATE `creature_template` SET `spell1` = 705 WHERE `entry` = 7342; #Shadow Bolt R3
UPDATE `creature_template` SET `spell2` = 1014 WHERE `entry` = 7342; #Curse of Agony R2
UPDATE `creature_template` SET `spell1` = 8699 WHERE `entry` = 785; #Unholy Frenzy
UPDATE `creature_template` SET `spell1` = 7373 WHERE `entry` = 48; #Hamstring R3