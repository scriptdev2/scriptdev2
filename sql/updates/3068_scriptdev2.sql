DELETE FROM gossip_texts WHERE entry IN (-3650000, -3650001, -3650002);
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3650000,'I am ready.','herald GOSSIP_ITEM_READY'),
(-3650001,'I am ready.  However, I\'d like to skip the pageantry.','herald GOSSIP_ITEM_READY_SKIP_INTRO'),
(-3650002,'I am ready for the next challenge.','herald GOSSIP_ITEM_READY_NEXT_CHALLENGE');
