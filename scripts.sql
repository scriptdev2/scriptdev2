--
-- guards data.
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
UPDATE `creature_template` SET`ScriptName` = 'guard_mulgore' WHERE `entry` IN (3209,3210,3211,3212,3213,3214,3215,3217,3218,3219,3220,3221,3222,3223,3224);UPDATE `creature_template` SET`ScriptName` = 'guard_dunmorogh' WHERE `entry` = 727 OR `entry` = 13076;
UPDATE `creature_template` SET`ScriptName` = 'guard_tirisfal' WHERE `entry` IN (1735,1738,2210,1736,1739,1737,1744,1745,5725,1743,2209,1746,1742);

--
-- Honor data.
--

UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40100;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40101;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40102;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40103;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40104;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40105;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40106;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40107;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40108;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40109;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40110;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40111;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40112;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40113;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40200;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40201;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40202;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40203;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40204;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40205;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40206;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40207;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40208;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40209;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40210;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40211;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40212;
UPDATE `creature_template` SET`ScriptName` = 'honor_vendor' WHERE `entry` = 40213;

--
-- others data.
--

UPDATE `creature_template` SET`ScriptName` = 'battlemaster' WHERE `entry` IN (12198,14982,857,12197,7427,347,15007,2804);
UPDATE `creature_template` SET`ScriptName` = 'guildmaster' WHERE `entry` IN (4974,5054,4613);
UPDATE `creature_template` SET`ScriptName` = 'travelmaster' WHERE `entry` IN (9564,12136,3149,12137,9566,3150,9559,9558);

--
-- custom data.
--

UPDATE `creature_template` SET`ScriptName` = 'marshal_mcbride' WHERE `entry` = 197;
UPDATE `creature_template` SET`ScriptName` = 'skorn_whitecloud' WHERE `entry` = 3052;
UPDATE `creature_template` SET`ScriptName` = 'kobold' WHERE `entry` = 257;
UPDATE `creature_template` SET`ScriptName` = 'default_caster' WHERE `entry` IN (946, 4063, 1867, 1867, 1915, 1914, 1889, 314, 2570, 2567, 697, 1653, 1562, 1564, 3218, 2591, 2255, 1539, 4299, 4296, 4300, 533, 203 , 2577 , 3273 , 1183 , 436 , 7235 , 2012, 1397 , 1174 , 2018 , 474 , 910 , 589 , 1399 , 3783 , 3732 , 3725 , 3728 , 3662 , 2207 , 7026 , 7028 , 7048 , 7038);
UPDATE `creature_template` SET`ScriptName` = '' WHERE `ScriptName` = 'sc_default';


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
UPDATE `creature_template` SET `spell1` = 116 WHERE `entry` = 474;
UPDATE `creature_template` SET `spell1` = 3140 WHERE `entry` = 910;
UPDATE `creature_template` SET `spell1` = 133 WHERE `entry` = 589;
UPDATE `creature_template` SET `spell1` = 143 WHERE `entry` = 589;
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
