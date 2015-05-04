DELETE FROM script_texts WHERE entry BETWEEN -1001190 AND -1001184;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001184,'How did you find me? Did Landgren tell?',14201,0,0,0,'admiral_westwind SAY_AGGRO'),
(-1001185,'You thought I would just let you kill me?',14205,0,0,0,'admiral_westwind SAY_SPHERE'),
(-1001186,'WHAT?! No matter. Even without my sphere, I will crush you! Behold my true identity and despair!',14207,1,0,0,'admiral_westwind SAY_NO_MATTER'),
(-1001187,'Gah! I spent too much time in that weak little shell.',14426,1,0,0,'malganis_icecrown SAY_TRANSFORM'),
(-1001188,'Kirel narak! I am Mal\'Ganis. I AM ETERNAL!',14427,1,0,0,'malganis_icecrown SAY_20_HP'),
(-1001189,'ENOUGH! I waste my time here. I must gather my strength on the homeworld.',14428,1,0,0,'malganis_icecrown SAY_DEFEATED'),
(-1001190,'You\'ll never defeat the Lich King without my forces. I\'ll have my revenge... on him AND you!',14429,1,0,0,'malganis_icecrown SAY_ESCAPE');
