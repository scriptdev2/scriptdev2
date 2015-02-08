DELETE FROM script_texts WHERE entry BETWEEN -1650070 AND -1650065;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1650065,'This farce ends here!',16259,1,0,0,'black knight SAY_AGGRO'),
(-1650066,'My rotting flesh was just getting in the way!',16262,1,0,0,'black knight SAY_PHASE_2'),
(-1650067,'I have no need for bones to best you!',16263,1,0,0,'black knight SAY_PHASE_3'),
(-1650068,'A waste of flesh.',16260,1,0,0,'black knight SAY_KILL_1'),
(-1650069,'Pathetic.',16261,1,0,0,'black knight SAY_KILL_2'),
(-1650070,'No! I must not fail... again...',16264,1,0,0,'black knight SAY_DEATH');
