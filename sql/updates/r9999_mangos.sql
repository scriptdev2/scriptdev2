DELETE FROM creature_movement_template WHERE entry = 8891;
INSERT INTO creature_movement_template (entry,point,position_x,position_y,position_z,waittime,script_id,textid1,textid2,textid3,textid4,textid5,emote,spell,orientation,model1,model2) VALUES
(8891,1,685.89,-279.73,-43.1944,0,889101,0,0,0,0,0,0,0,0.471239,0,0),
(8891,2,685.89,-279.73,-43.1944,10800000,0,0,0,0,0,0,0,0,0.471239,0,0);

DELETE FROM dbscripts_on_creature_movement WHERE id = 889101;
INSERT INTO dbscripts_on_creature_movement (id,delay,command,datalong,datalong2,buddy_entry,search_radius,data_flags,dataint,dataint2,dataint3,dataint4,x,y,z,o,comments) VALUES
(889101,0,25,0,0,0,0,0,0,0,0,0,0,0,0,0,'RUN OFF'),
(889101,1,25,0,0,8891,40,0,0,0,0,0,0,0,0,0,'RUN OFF'),
(889101,2,20,1,5,8891,40,8,0,0,0,0,0,0,0,0,'switch to random around current point'),
(889101,3,20,1,5,0,0,8,0,0,0,0,0,0,0,0,'switch to random around current point');

DELETE FROM scripted_areatrigger WHERE entry = 1786;
INSERT INTO scripted_areatrigger (entry,ScriptName) VALUES
(1786,'at_shadowforge_bridge');