DELETE FROM script_texts WHERE entry IN (-1603227,-1603228,-1603231,-1603267);
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1603227,'Bad news sire.',15538,0,0,1,'yogg SAY_GARONA_1'),
(-1603228,'Gul\'dan is bringing up his warlocks by nightfall. Until then, the Blackrock clan will be trying to take the Eastern Wall.',15540,0,0,1,'yogg SAY_GARONA_3'),
(-1603231,'We will hold until the reinforcements come. As long as men with stout hearts are manning the walls and throne Stormwind will hold.',15585,0,0,1,'yogg SAY_KING_LLANE'),
(-1603267,'The clans are united under Blackhand in this assault. They will stand together until Stormwind has fallen.',15539,0,0,0,'yogg SAY_GARONA_2');
UPDATE script_texts SET emote=1 WHERE entry IN (-1603222,-1603223,-1603224,-1603225);
