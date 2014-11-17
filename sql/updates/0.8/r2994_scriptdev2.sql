DELETE FROM script_texts WHERE entry BETWEEN -1000992 AND -1000981;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000981,'It is time. The rite of exorcism will now commence...',0,0,0,0,'anchorite barada SAY_EXORCISM_1'),
(-1000982,'Prepare yourself. Do not allow the ritual to be interrupted or we may lose our patient...',0,0,0,1,'anchorite barada SAY_EXORCISM_2'),
(-1000983,'Keep away. The fool is mine.',0,0,0,0,'colonel jules SAY_EXORCISM_3'),
(-1000984,'Back, foul beings of darkness! You have no power here!',0,0,0,0,'anchorite barada SAY_EXORCISM_4'),
(-1000985,'No! Not yet! This soul is ours!',0,0,0,0,'colonel jules SAY_EXORCISM_5'),
(-1000986,'Back! I cast you back... corrupter of faith! Author of pain! Do not return, or suffer the same fate as you did here today!',0,0,0,2,'anchorite barada SAY_EXORCISM_6'),
(-1000987,'I... must not...falter!',0,0,0,0,'anchorite barada SAY_EXORCISM_RANDOM_1'),
(-1000988,'Be cleansed with Light, human! Let not the demonic corruption overwhelm you.',0,0,0,0,'anchorite barada SAY_EXORCISM_RANDOM_2'),
(-1000989,'Back, foul beings of darkness! You have no power here!',0,0,0,0,'anchorite barada SAY_EXORCISM_RANDOM_3'),
(-1000990,'This is fruitless, draenei! You and your little helper cannot wrest control of this pathetic human. He is mine!',0,0,0,0,'colonel jules SAY_EXORCISM_RANDOM_4'),
(-1000991,'I see your ancestors, Anchorite! They writhe and scream in the darkness... they are with us!',0,0,0,0,'colonel jules SAY_EXORCISM_RANDOM_5'),
(-1000992,'I will tear your soul into morsels and slow roast them over demon fire!',0,0,0,0,'colonel jules SAY_EXORCISM_RANDOM_6');
DELETE FROM gossip_texts WHERE entry=-3000111;
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3000111,'I am ready, Anchorite. Let us begin the exorcism.','anchorite barada GOSSIP_ITEM_EXORCISM');
