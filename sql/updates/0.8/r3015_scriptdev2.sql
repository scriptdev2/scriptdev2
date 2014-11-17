DELETE FROM script_texts WHERE entry BETWEEN -1001136 AND -1001125;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001125,'Beginning the distillation in 5 seconds.',0,0,0,0,'tipsy mcmanus SAY_DISTILLATION_START'),
(-1001126,'Add another orange! Quickly!',0,0,0,25,'tipsy mcmanus SAY_ADD_ORANGE'),
(-1001127,'Add bananas!',0,0,0,25,'tipsy mcmanus SAY_ADD_BANANAS'),
(-1001128,'Put a papaya in the still!',0,0,0,25,'tipsy mcmanus SAY_ADD_PAPAYA'),
(-1001129,'The still needs heat! Light the brazier!',0,0,0,5,'tipsy mcmanus SAY_LIGHT_BRAZIER'),
(-1001130,'Pressure\'s too high! Open the pressure valve!',0,0,0,5,'tipsy mcmanus SAY_OPEN_VALVE'),
(-1001131,'Good job! Keep your eyes open, now.',0,0,0,4,'tipsy mcmanus SAY_ACTION_COMPLETE_1'),
(-1001132,'Nicely handled! Stay on your toes!',0,0,0,4,'tipsy mcmanus SAY_ACTION_COMPLETE_2'),
(-1001133,'Well done! Be ready for anything!',0,0,0,4,'tipsy mcmanus SAY_ACTION_COMPLETE_3'),
(-1001134,'That\'ll do. Never know what it\'ll need next...',0,0,0,4,'tipsy mcmanus SAY_ACTION_COMPLETE_4'),
(-1001135,'It\'s no good! I\'m shutting it down...',0,0,0,0,'tipsy mcmanus SAY_DISTILLATION_FAIL'),
(-1001136,'We\'ve done it! Come get the cask.',0,0,0,0,'tipsy mcmanus SAY_DISTILLATION_COMPLETE');
DELETE FROM gossip_texts WHERE entry=-3000114;
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3000114,'I\'m ready to start the distillation, uh, Tipsy.','tipsy mcmanus GOSSIP_ITEM_START_DISTILLATION');
