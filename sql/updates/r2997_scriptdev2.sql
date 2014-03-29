DELETE FROM script_texts WHERE entry BETWEEN -1001005 AND -1001001;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1001001,'We must leave before more are alerted.',0,0,0,0,'kurenai captive SAY_KUR_START'),
(-1001002,'It\'s an ambush! Defend yourself!',0,0,0,0,'kurenai captive SAY_KUR_AMBUSH_1'),
(-1001003,'We are surrounded!',0,0,0,0,'kurenai captive SAY_KUR_AMBUSH_2'),
(-1001004,'Up ahead is the road to Telaar. We will split up when we reach the fork as they will surely send more Murkblood after us. Hopefully one of us makes it back to Telaar alive.',0,0,0,1,'kurenai captive SAY_KUR_COMPLETE_1'),
(-1001005,'Farewell, stranger. Your heroics will be remembered by my people. Now, hurry to Telaar!',0,0,0,1,'kurenai captive SAY_KUR_COMPLETE_2');

DELETE FROM script_waypoint WHERE entry=18209;
INSERT INTO script_waypoint VALUES
(18209, 0, -1518.092407, 8465.188477, -4.102, 0, ''),
(18209, 1, -1516.741699, 8472.000977, -4.101, 0, ''),
(18209, 2, -1516.330444, 8473.119141, -4.102, 0, ''),
(18209, 3, -1514.117310, 8476.740234, -4.100, 0, ''),
(18209, 4, -1512.199951, 8481.147461, -4.015, 0, ''),
(18209, 5, -1514.709839, 8488.281250, -3.544, 0, ''),
(18209, 6, -1516.556274, 8495.236328, -2.463, 0, ''),
(18209, 7, -1515.730957, 8506.528320, -0.609, 7000, 'SAY_KUR_AMBUSH'),
(18209, 8, -1505.038940, 8513.247070, 0.672, 0, ''),
(18209, 9, -1476.161133, 8496.066406, 2.157, 0, ''),
(18209, 10, -1464.450684, 8492.601563, 3.529, 0, ''),
(18209, 11, -1457.568359, 8492.183594, 4.449, 0, ''),
(18209, 12, -1444.100342, 8499.031250, 6.177, 0, ''),
(18209, 13, -1426.472168, 8510.116211, 7.686, 0, ''),
(18209, 14, -1403.685303, 8524.146484, 9.680, 0, ''),
(18209, 15, -1384.890503, 8542.014648, 11.180, 0, ''),
(18209, 16, -1385.107422, 8547.194336, 11.297, 5000, 'SAY_KUR_COMPLETE'),
(18209, 17, -1387.814453, 8556.652344, 11.735, 0, ''),
(18209, 18, -1397.817749, 8574.999023, 13.204, 0, ''),
(18209, 19, -1411.961304, 8598.225586, 14.990, 0, '');
