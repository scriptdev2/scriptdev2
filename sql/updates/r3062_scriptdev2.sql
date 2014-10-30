DELETE FROM script_texts WHERE entry IN (-1595003,-1595004,-1595005,-1595006,-1595007,-1595008);
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1595003,'Scourge forces have been spotted near the Festival Lane Gate!',0,6,0,0,'lordaeron crier SAY_SCOURGE_FESTIVAL_LANE'),
(-1595004,'Scourge forces have been spotted near the King\'s Square fountain!',0,6,0,0,'lordaeron crier SAY_SCOURGE_KINGS_SQUARE'),
(-1595005,'Scourge forces have been spotted near the Market Row Gate!',0,6,0,0,'lordaeron crier SAY_SCOURGE_MARKET_ROW'),
(-1595006,'Scourge forces have been spotted near the Town Hall!',0,6,0,0,'lordaeron crier SAY_SCOURGE_TOWN_HALL'),
(-1595007,'Scourge forces have been spotted near the Elder\'s Square Gate!',0,6,0,0,'lordaeron crier SAY_SCOURGE_ELDERS_SQUARE'),
(-1595008,'Champions, meet me at the Town Hall at once. We must take the fight to Mal\'Ganis.',14297,6,0,0,'arthas SAY_MEET_TOWN_HALL');
DELETE FROM gossip_texts WHERE entry=-3595009;
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3595009,'We\'re only doing what is best for Lordaeron, your Highness.','arthas GOSSIP_ITEM_TOWN_HALL');
