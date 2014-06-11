DELETE FROM script_texts WHERE entry BETWEEN -1001124 AND -1001117;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001117,'AHAHAHAHA... you\'ll join us soon enough!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_1'),
(-1001118,'I don\'t want to leave! I want to stay here!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_2'),
(-1001119,'I must get further underground to where he is. I must jump!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_3'),
(-1001120,'I won\'t leave!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_4'),
(-1001121,'I\'ll never return. The whole reason for my existence awaits below!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_5'),
(-1001122,'I\'m coming, master!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_6'),
(-1001123,'My life for you!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_7'),
(-1001124,'NO! You\'re wrong! The voices in my head are beautiful!',0,1,0,0,'saronite mine slave SAY_MINER_SUICIDE_8');
DELETE FROM gossip_texts WHERE entry=-3000113;
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3000113,'Go on, you\'re free. Get out of here!','saronite mine slave GOSSIP_ITEM_SLAVE_FREE');
