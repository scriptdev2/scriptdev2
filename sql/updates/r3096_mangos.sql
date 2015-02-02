UPDATE instance_template SET ScriptName='instance_razorfen_downs' WHERE map=129;
DELETE FROM scripted_event_id WHERE id=3130;
INSERT INTO scripted_event_id VALUES (3130, 'event_go_tutenkash_gong');
