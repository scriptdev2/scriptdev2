--
-- area data
--

--
-- boss data
--

UPDATE `creature_template` SET`ScriptName` = 'boss_onyxia' WHERE `entry` = 10184;

--
-- creature data
--
UPDATE `creature_template` SET`ScriptName` = '' WHERE `ScriptName` = 'sc_default';

-- Generic Casters
UPDATE `creature_template` SET`ScriptName` = 'generic_caster' WHERE `entry` IN (946, 4063, 1867, 1867, 1915, 1914, 1889, 314, 2570, 2567, 697, 1653, 1562, 1564, 3218, 2591, 2255, 1539, 4299, 4296, 4300, 533, 203 , 2577 , 3273 , 1183 , 436 , 7235 , 2012, 1397 , 1174 , 2018 , 474 , 910 , 589 , 1399 , 3783 , 3732 , 3725 , 3728 , 3662 , 2207 , 7026 , 7028 , 7048 , 7038); -- Base
UPDATE `creature_template` SET`ScriptName` = 'generic_caster' WHERE `entry` IN (2953, 3232, 2952, 2954, 1732, 1726, 619, 634, 1725, 4418, 474, 910, 95, 589); -- Revision 9

-- Generic Melee
UPDATE `creature_template` SET`ScriptName` = 'generic_melee' WHERE `entry` IN (3232, 2952, 2954, 1707, 1711, 1708, 1715, 1706, 1729, 657, 4417, 598, 4416, 594, 6866, 215, 909, 116, 504, 94, 122, 449, 121, 590); -- Revision 9

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