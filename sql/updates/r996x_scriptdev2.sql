DELETE FROM script_texts WHERE entry BETWEEN -1230047 AND -1230049;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1230047,'I\m out of here!',0,0,0,0,'rocknot SAY_ROCKNOT_DESPAWN'),
(-1230048,'HALT! There were...reports...of a...disturbance.',0,1,0,0,'Fireguard Destroyer YELL_PATROL_1'),
(-1230049,'We...are...investigating.',0,1,0,0,'Fireguard Destroyer YELL_PATROL_2');