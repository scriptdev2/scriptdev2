/* THIS FILE CONTAINS ADDITIONAL SQL REQUIRED FOR SCRIPTS TO WORK PROPERLY */
/* CAN BE INCLUDED IN ANY DATABASE IF SCRIPTDEV SUPPORT IS WANTED BY DEFAULT */
/* It's recommended to run this SQL after any regular database update */


/* */
/* AREATRIGGER_TEMPLATE */
/* */


/* */
/* CREATURE_TEMPLATE */
/* */
/* UPDATE  `creature_template` SET `faction` = 0, `npcflag` = `npcflag`|0 WHERE `entry`= xxxxx ; */

/* Lothos Riftwaker */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 14387;
/* Darrowshire Spirit */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 11064;
/* Skorn Whitecloud */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 3052;
/* Ashyen and Keleth */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` IN (17900,17901);
/* Great Bear Spirit */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 11956;
/* Captain Blackanvil and Captain Skullsplit */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` IN (15440, 15612);
/* Elder Kuruti */
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 18197;

/* mob_broggok_poisoncloud */
UPDATE `creature_template` SET `minlevel` = 63, `maxlevel` = 63 WHERE `entry` = 17662;
/* Arcane Orb Target */
UPDATE `creature_template` SET `modelid_A` = 16946, `modelid_H` = 16946, `minlevel` = 70, `maxlevel` = 70 WHERE `entry` = 19577;

/* Beaten Corpse*/
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` = 10668;


/* */
/* CREATURE_MOVEMENT */
/* */


/* */
/* CREATURE */
/* */
/* DELETE FROM `creature` WHERE `id` = xxxxx;   delete any spawned creature */
/* DELETE FROM `creature` WHERE `map` = xxx AND `id` = xxxxx;   delete any spawned creature in the specified map */
/* Use DELETE with caution! Note that most of the deletions should be related to creatures that are summoned by scripts, creatures not present in world by default */

/* Nefarian */
DELETE FROM `creature` WHERE `id` = 11583;
/* Darrowshire Spirit */
/* DELETE FROM `creature` WHERE `id` = 11064; */
/* Undead Postman */
/* DELETE FROM `creature` WHERE `id` = 11142; */
/* Darkmaster Gandling */
DELETE FROM `creature` WHERE `id` = 1853;


/* */
/* QUEST_TEMPLATE */
/* */
/* UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 0, `ReqCreatureOrGOId2` = 0, `ReqCreatureOrGOId3` = 0, `ReqCreatureOrGOId4` = 0, `ReqCreatureOrGOCount1` = 0, `ReqCreatureOrGOCount2` = 0, `ReqCreatureOrGOCount3` = 0, `ReqCreatureOrGOCount4`=  0, `ReqSpellCast1` = 0, `ReqSpellCast2` = 0, `ReqSpellCast3` = 0, `ReqSpellCast4` = 0 WHERE `entry`= xxxxx;*/

/* Thirst Unending */
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 15274, `ReqCreatureOrGOId2` = 0, `ReqCreatureOrGOId3`= 0, `ReqCreatureOrGOId4`= 0, `ReqCreatureOrGOCount1`= 6, `ReqCreatureOrGOCount2`= 0, `ReqCreatureOrGOCount3`= 0, `ReqCreatureOrGOCount4`= 0, `ReqSpellCast1` = 28734, `ReqSpellCast2` = 0, `ReqSpellCast3` = 0, `ReqSpellCast4` = 0 WHERE `entry`= 8346;
/* Fel Orc Scavengers */
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 21161, `ReqCreatureOrGOId2` = 0, `ReqCreatureOrGOId3`= 0, `ReqCreatureOrGOId4`= 0, `ReqCreatureOrGOCount1`= 20, `ReqCreatureOrGOCount2`= 0, `ReqCreatureOrGOCount3`= 0, `ReqCreatureOrGOCount4`= 0, `ReqSpellCast1` = 0, `ReqSpellCast2` = 0, `ReqSpellCast3` = 0, `ReqSpellCast4` = 0 WHERE `entry`= 10482;
/* Dearest Natalia */
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 15221, `ReqCreatureOrGOId2` = 15222, `ReqCreatureOrGOId3` = 0, `ReqCreatureOrGOId4` = 0, `ReqCreatureOrGOCount1` = 1, `ReqCreatureOrGOCount2` = 1, `ReqCreatureOrGOCount3` = 0, `ReqCreatureOrGOCount4`= 0, `ReqSpellCast1` = 0, `ReqSpellCast2` = 0, `ReqSpellCast3` = 0, `ReqSpellCast4` = 0 WHERE `entry`= 8304;
/* Blessings of the Ancients */
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 17900, `ReqCreatureOrGOId2` = 17901, `ReqCreatureOrGOId3`= 0, `ReqCreatureOrGOId4`= 0, `ReqCreatureOrGOCount1` = 1, `ReqCreatureOrGOCount2` = 1, `ReqCreatureOrGOCount3` = 0, `ReqCreatureOrGOCount4`= 0, `ReqSpellCast1` = 0, `ReqSpellCast2` = 0, `ReqSpellCast3` = 0, `ReqSpellCast4` = 0 WHERE `entry` =9785;
/* Lost in Battle */
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 10668, `ReqCreatureOrGOCount1` = 1 WHERE `entry` = 4921;


/* */
/* ITEM_TEMPLATE */
/* */

/* */
/* SPELL_SCRIPT_TARGET */
/* */

/*Infernal Relay*/
REPLACE INTO `spell_script_target` VALUES (30834, 1, 17646);



/* EOF */
