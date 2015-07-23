DELETE FROM script_texts WHERE entry BETWEEN -1230040 AND -1230036;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1230036,'I want more ale! Give me more ale!',0,0,0,0,'rocknot SAY_MORE_BEER'),
(-1230037,'You\'ll pay for that!',0,0,0,0,'Grim Patron SAY_PISSED_PATRON_1'),
(-1230038,'No!',0,0,0,0,'Grim Patron SAY_PISSED_PATRON_2'),
(-1230039,'Oh! Now I\'m pissed!',0,0,0,0,'Grim Patron SAY_PISSED_PATRON_3'),
(-1230040,'Violence! Property damage! None shall pass!!',0,1,0,0,'Phalanx YELL_PHALANX_AGGRO');

UPDATE script_texts SET emote = 5 WHERE entry = -1230000;

DELETE FROM script_waypoint WHERE entry=9502;
INSERT INTO script_waypoint VALUES
(9502,0,847.848,-230.067,-43.614,0,''),
(9502,1,868.122,-223.884,-43.695,0,'YELL_PHALANX_AGGRO');