DELETE FROM script_texts WHERE entry BETWEEN -1001012 AND -1001006;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001006,'Thanks for your help. Let\'s get out of here!',0,0,0,1,'skyguard prisoner SAY_ESCORT_START'),
(-1001007,'Let\'s keep moving. I don\'t like this place.',0,0,0,1,'skyguard prisoner SAY_AMBUSH_END'),
(-1001008,'Thanks again. Sergeant Doryn will be glad to hear he has one less scout to replace this week.',0,0,0,1,'skyguard prisoner SAY_ESCORT_COMPLETE'),
(-1001009,'Death to our enemies!',0,0,0,0,'skettis wing guard SAY_AMBUSH_1'),
(-1001010,'No one escapes Skettis!',0,0,0,0,'skettis wing guard SAY_AMBUSH_2'),
(-1001011,'Skettis prevails!',0,0,0,0,'skettis wing guard SAY_AMBUSH_3'),
(-1001012,'You\'ll go nowhere, Skyguard scum!',0,0,0,0,'skettis wing guard SAY_AMBUSH_4');

DELETE FROM script_waypoint WHERE entry=23383;
INSERT INTO script_waypoint VALUES
(23383, 0, -4109.424, 3034.155, 344.168, 5000, 'SAY_ESCORT_START'),
(23383, 1, -4113.265, 3035.989, 344.071, 0, ''),
(23383, 2, -4120.018, 3032.223, 344.074, 0, ''),
(23383, 3, -4124.412, 3026.332, 344.151, 0, ''),
(23383, 4, -4128.823, 3026.645, 344.035, 0, ''),
(23383, 5, -4138.909, 3028.952, 338.920, 0, ''),
(23383, 6, -4152.592, 3031.234, 336.913, 0, ''),
(23383, 7, -4169.812, 3034.305, 342.047, 0, ''),
(23383, 8, -4174.631, 3036.044, 343.457, 0, ''),
(23383, 9, -4174.399, 3044.983, 343.862, 0, ''),
(23383, 10, -4176.635, 3052.014, 344.077, 0, ''),
(23383, 11, -4183.662, 3058.895, 344.150, 0, ''),
(23383, 12, -4182.916, 3065.411, 342.574, 0, ''),
(23383, 13, -4182.055, 3070.558, 337.644, 5000, 'ambush'),
(23383, 14, -4182.055, 3070.558, 337.644, 5000, 'SAY_AMBUSH_END'),
(23383, 15, -4181.256, 3077.131, 331.590, 0, ''),
(23383, 16, -4179.994, 3086.101, 325.571, 0, ''),
(23383, 17, -4178.770, 3090.101, 323.955, 0, ''),
(23383, 18, -4177.965, 3093.867, 323.839, 5000, 'SAY_ESCORT_COMPLETE'),
(23383, 19, -4166.252, 3106.508, 320.961, 0, '');
