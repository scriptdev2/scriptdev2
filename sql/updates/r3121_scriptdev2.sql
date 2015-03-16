DELETE FROM gossip_texts WHERE entry IN (-3649014,-3649015,-3649016,-3649017);
INSERT INTO gossip_texts (entry,content_default,comment) VALUES
(-3649014,'We\'re ready. This time things will be different.','barrett GOSSIP_ITEM_BEAST_WIPE_START'),
(-3649015,'Now.','barrett GOSSIP_ITEM_JARAXXUS_WIPE_START'),
(-3649016,'We\'ll just have to improve our teamwork to match the two of them.','barrett GOSSIP_ITEM_TWINS_WIPE_INIT'),
(-3649017,'Just bring them out again, then watch.','barrett GOSSIP_ITEM_TWINS_WIPE_START');
