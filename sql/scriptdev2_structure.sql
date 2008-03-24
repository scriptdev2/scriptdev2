DROP TABLE IF EXISTS `eventai_scripts`;
CREATE TABLE `eventai_scripts` (
`id` int(11) unsigned NOT NULL COMMENT 'Identifier' AUTO_INCREMENT,
`creature_id` int(11) unsigned NOT NULL default '0' COMMENT 'Creature Template Identifier',

`event_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Event Type',
`event_inverse_phase_mask` int(11) signed NOT NULL default '0' COMMENT 'Mask which phases this event will not trigger in',
`event_chance` int(3) signed NOT NULL default '100',
`event_param1` int(11) signed NOT NULL default '0',
`event_param2` int(11) signed NOT NULL default '0',
`event_param3` int(11) signed NOT NULL default '0',

`action1_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action1_param1` int(11) signed NOT NULL default '0',
`action1_param2` int(11) signed NOT NULL default '0',
`action1_param3` int(11) signed NOT NULL default '0',

`action2_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action2_param1` int(11) signed NOT NULL default '0',
`action2_param2` int(11) signed NOT NULL default '0',
`action2_param3` int(11) signed NOT NULL default '0',

`action3_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action3_param1` int(11) signed NOT NULL default '0',
`action3_param2` int(11) signed NOT NULL default '0',
`action3_param3` int(11) signed NOT NULL default '0',

`comment` varchar(255) NOT NULL default '' COMMENT 'Event Comment',

  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='EventAI Scripts';

DROP TABLE IF EXISTS `localized_texts`;
CREATE TABLE `localized_texts` (
  `id` int(11) unsigned NOT NULL COMMENT 'Identifier' AUTO_INCREMENT,
`locale_0` varchar(255) NOT NULL default '',
`locale_1` varchar(255) NOT NULL default '',
`locale_2` varchar(255) NOT NULL default '',
`locale_3` varchar(255) NOT NULL default '',
`locale_4` varchar(255) NOT NULL default '',
`locale_5` varchar(255) NOT NULL default '',
`locale_6` varchar(255) NOT NULL default '',
`locale_7` varchar(255) NOT NULL default '',

`comment` varchar(255) NOT NULL default '' COMMENT 'Text Comment',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Localized Text';

DROP TABLE IF EXISTS `eventai_summons`;
CREATE TABLE `eventai_summons` (
`id` int(11) unsigned NOT NULL COMMENT 'Location Identifier' AUTO_INCREMENT,
`position_x` float NOT NULL default '0',
`position_y` float NOT NULL default '0',
`position_z` float NOT NULL default '0',
`orientation` float NOT NULL default '0',
`spawntimesecs` int(11) unsigned NOT NULL default '120',
PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='EventAI Summoning Locations';