DELETE FROM script_texts WHERE entry BETWEEN -1650064 AND -1650052;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1650052,'Prepare yourselves!',16135,1,0,0,'eadric SAY_AGGRO'),
(-1650053,'Hammer of the Righteous!',16136,1,0,0,'eadric SAY_HAMMER'),
(-1650054,'You... You need more practice.',16137,1,0,0,'eadric SAY_KILL_1'),
(-1650055,'Nay! Nay! And I say yet again nay! Not good enough!',16138,1,0,0,'eadric SAY_KILL_2'),
(-1650056,'I yield! I submit. Excellent work. May I run away now?',16139,1,0,0,'eadric SAY_DEFEAT'),
(-1650057,'%s begins to radiate light. Shield your eyes!',0,3,0,0,'eadric EMOTE_RADIATE'),
(-1650058,'%s targets $N with the Hammer of the Righteous!',0,3,0,0,'eadric EMOTE_HAMMER'),

(-1650059,'Well then, let us begin.',16247,1,0,0,'paletress SAY_AGGRO'),
(-1650060,'Take this time to consider your past deeds.',16248,1,0,0,'paletress SAY_MEMORY'),
(-1650061,'Even the darkest memory fades when confronted.',16249,1,0,0,'paletress SAY_MEMORY_DIES'),
(-1650062,'Take your rest.',16250,1,0,0,'paletress SAY_KILL_1'),
(-1650063,'Be at ease.',16251,1,0,0,'paletress SAY_KILL_2'),
(-1650064,'Excellent work!',16252,1,0,0,'paletress SAY_DEFEAT');
