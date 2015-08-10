DELETE FROM script_waypoint WHERE entry=9537;
INSERT INTO script_waypoint VALUES
(9537, 0, 854.9774, -150.3077, -49.671, 0, ''),
(9537, 1, 855.8161, -149.763, -49.671, 0, 'YELL_HURLEY_SPAWN'),
(9537, 2, 882.6298, -148.1658, -49.75969, 0, ''),
(9537, 3, 883.9604, -148.0866, -49.76004, 0, ''),
(9537, 4, 896.846, -147.319, -49.75753, 0, ''),
(9537, 5, 896.846, -147.319, -49.75753, 0, '');

DELETE FROM script_texts WHERE entry IN (-1230041,-1230042);
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1230041,'Get away from those kegs!',0,1,0,0,'Hurley Blackbreath YELL_HURLEY_SPAWN'),
(-1230042,'You\'ll pay for that!',0,0,0,0,'Hurley Blackbreath SAY_HURLEY_AGGRO');