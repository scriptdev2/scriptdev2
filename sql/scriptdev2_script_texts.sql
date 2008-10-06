-- 
-- scriptdev2_script_texts.sql Contains data for table `script_texts` mainly used in C++ parts
-- valied entries for table are between -100000 and -199999
-- 

TRUNCATE `script_texts`;

-- 
-- First entries are reserved for special use, do not add regular text here.
-- 

INSERT INTO `script_texts` (`entry`,`content_default`,`sound`,`type`,`language`,`comment`) VALUES
(-100000,'<ScriptDev2 Text Entry Missing!>',0,0,0,'DEFAULT_TEXT');

-- 
-- Normal text entries. Say/Yell/Whisper/Emote for any regular creature.
-- 



-- 
-- Below just for beautiful view in table, run at own desire
-- 

-- ALTER TABLE `script_texts` ORDER BY `entry` desc;
