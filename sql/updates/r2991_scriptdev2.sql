DELETE FROM script_texts WHERE entry BETWEEN -1000980 AND -1000977;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000977,'Oh, it\'s on now! But you thought I\'d be alone too, huh?!',0,0,0,0,'tapoke slim jahn SAY_AGGRO'),
(-1000978,'Okay, okay! No need to get all violent. I\'ll talk. I\'ll talk!',0,0,0,20,'tapoke slim jahn SAY_DEFEAT'),
(-1000979,'Whoa! This is way more than what I bargained for, you\'re on your own, Slim!',0,0,0,0,'slim\'s friend SAY_FRIEND_DEFEAT'),
(-1000980,'I have a few notes from the job back at my place. I\'ll get them and then meet you back in the inn.',0,0,0,1,'tapoke slim jahn SAY_NOTES');
