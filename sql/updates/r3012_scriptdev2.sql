DELETE FROM script_texts WHERE entry BETWEEN -1001116 AND -1001106;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001106,'Don\'t forget to get my bell out of the chest here. And remember, if do happen to wander off, just ring it and I\'ll find you again.',0,0,0,1,'shay leafrunner SAY_ESCORT_START'),
(-1001107,'Are we taking the scenic route?',0,0,0,0,'shay leafrunner SAY_WANDER_1'),
(-1001108,'Oh, what a beautiful flower over there...',0,0,0,0,'shay leafrunner SAY_WANDER_2'),
(-1001109,'Are you sure this is the right way? Maybe we should go this way instead...',0,0,0,0,'shay leafrunner SAY_WANDER_3'),
(-1001110,'Hmmm, I wonder what\'s over this way?',0,0,0,0,'shay leafrunner SAY_WANDER_4'),
(-1001111,'This is quite an adventure!',0,0,0,0,'shay leafrunner SAY_WANDER_DONE_1'),
(-1001112,'Oh, I wandered off again. I\'m sorry.',0,0,0,0,'shay leafrunner SAY_WANDER_DONE_2'),
(-1001113,'The bell again, such a sweet sound.',0,0,0,0,'shay leafrunner SAY_WANDER_DONE_3'),
(-1001114,'%s begins to wander off.',0,2,0,0,'shay leafrunner EMOTE_WANDER'),
(-1001115,'Oh, here you are, Rockbiter! I\'m sorry, I know I\'m not supposed to wander off.',0,0,0,1,'shay leafrunner SAY_EVENT_COMPLETE_1'),
(-1001116,'I\'m so glad yer back Shay. Please, don\'t ever run off like that again! What would I tell yer parents if I lost ya?',0,0,0,1,'rockbiter SAY_EVENT_COMPLETE_2');
