DELETE FROM script_texts WHERE entry BETWEEN -1531046 AND -1531041;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1531041,'%s begins to slow!',0,2,0,0,'viscidus EMOTE_SLOW'),
(-1531042,'%s is freezing up!',0,2,0,0,'viscidus EMOTE_FREEZE'),
(-1531043,'%s is frozen solid!',0,2,0,0,'viscidus EMOTE_FROZEN'),
(-1531044,'%s begins to crack!',0,2,0,0,'viscidus EMOTE_CRACK'),
(-1531045,'%s looks ready to shatter!',0,2,0,0,'viscidus EMOTE_SHATTER'),
(-1531046,'%s explodes!',0,2,0,0,'viscidus EMOTE_EXPLODE');
