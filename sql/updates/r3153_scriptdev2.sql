DELETE FROM script_texts WHERE entry IN (-1609081,-1609082,-1609083,-1609084,-1609085,-1609086,-1609087,-1609088,-1609287,-1609288,-1609289);
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1609081,'%s throws rotten apple on $N.',0,2,0,0,'city guard EMOTE_APPLE'),
(-1609082,'%s throws rotten banana on $N.',0,2,0,0,'city guard EMOTE_BANANA'),
(-1609083,'%s spits on $N.',0,2,0,0,'city guard EMOTE_SPIT'),
(-1609084,'Monster!',0,0,0,14,'city guard SAY_RANDOM_1'),
(-1609085,'Murderer!',0,0,0,14,'city guard SAY_RANDOM_2'),
(-1609086,'GET A ROPE!',0,0,0,25,'city guard SAY_RANDOM_3'),
(-1609087,'How dare you set foot in our city!',0,0,0,25,'city guard SAY_RANDOM_4'),
(-1609088,'You disgust me.',0,0,0,14,'city guard SAY_RANDOM_5'),
(-1609287,'Looks like we\'re going to have ourselves an execution.',0,0,0,25,'city guard SAY_RANDOM_6'),
(-1609288,'Traitorous dog.',0,0,0,14,'city guard SAY_RANDOM_7'),
(-1609289,'My family was wiped out by the Scourge! MONSTER!',0,0,0,25,'city guard SAY_RANDOM_8');
