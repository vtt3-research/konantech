
DELETE from DEMO_COMPJOB_TB where jobname='cataloging';
INSERT INTO DEMO_COMPJOB_TB (jobname, jobtype, jobproperty, startexname, progressexname, endexname,  paramproperty) VALUES (
'cataloging', 'ca',
'{
	"select": [{
		"table": "demo_video_tb",
		"alias": "catalogvideosrc",
		"where": ["idx"],
		"select": "idx as assetid, ''video/''|| assetfilepath || ''/''|| assetfilename as filepath, volumewin, volumeetc "
	},{
		"table": "demo_video_tb",
		"alias": "shotimages",
		"where": ["idx"],
		"select": "''proxyshot/'' || assetfilepath || ''/'' as filepath, volumewin, volumeetc "
	}],
	"status": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "catalogstatus"
	}],
	"start": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "catalogstarttime"
	}],
	"end": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "catalogendtime"
	}]
}', null, null, null,  '{
"paramlist":
	[{
		"type": "mainkey",
		"field": "idx"
	}]
}');



DELETE from DEMO_COMPJOB_TB where jobname='transcoding';
INSERT INTO DEMO_COMPJOB_TB (jobname, jobtype, jobproperty, startexname, progressexname, endexname,  paramproperty) VALUES (
'transcoding', 'tc', '{
	"select": [{
		"table": "demo_video_tb",
		"alias": "convertvideosrc",
		"where": ["idx"],
		"select": "idx as assetid, ''MXF'' as videowrappertype, ''video/''|| assetfilepath || ''/''|| assetfilename as filepath, volumewin, volumeetc "
	},{
		"table": "demo_video_tb",
		"alias": "proxyvideos",
		"where": ["idx"],
		"select": "idx as idx, ''proxyvideo/'' || assetfilepath || ''/'' || objectid || ''.mp4'' as filepath, ''h264'' as mobtype , volumewin, volumeetc "
	}],
	"status": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingstatus"
	}],
	"start": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingstarttime"
	}],
	"end": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingendtime"
	}]
}', null, null, null, '{
"paramlist":
	[{
		"type": "mainkey",
		"field": "idx"
	}]
}');


DELETE from DEMO_COMPJOB_TB where jobname='convert_video';
INSERT INTO DEMO_COMPJOB_TB (jobname, jobtype, jobproperty, startexname, progressexname, endexname,  paramproperty) VALUES (
'convert_video', 'tc_video', '{
	"select": [{
		"table": "demo_video_tb",
		"alias": "convertvideosrc",
		"where": ["idx"],
		"select": "idx as assetid, ''MXF'' as videowrappertype, ''video/''|| assetfilepath || ''/''|| assetfilename as filepath, volumewin, volumeetc "
	},{
		"table": "demo_video_tb",
		"alias": "proxyvideos",
		"where": ["idx"],
		"select": "idx as idx, ''proxyvideo/'' || assetfilepath || ''/'' || objectid || ''.mp4'' as filepath, ''h264'' as mobtype , volumewin, volumeetc "
	}],
	"status": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingstatus"
	}],
	"start": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingstarttime"
	}],
	"end": [{
		"table": "demo_video_tb",
		"where": ["idx"],
		"update": "transcodingendtime"
	}]
}', null, null, null, '{
"paramlist":
	[{
		"type": "mainkey",
		"field": "idx"
	}]
}');




DELETE from DEMO_WORKFLOW_TB;
INSERT INTO DEMO_WORKFLOW_TB (workflowname, assetinfo, tablename, isdelworkflow) VALUES ('ca', null, null, null);
INSERT INTO DEMO_WORKFLOW_TB (workflowname, assetinfo, tablename, isdelworkflow) VALUES ('tc', null, null, null);
INSERT INTO DEMO_WORKFLOW_TB (workflowname, assetinfo, tablename, isdelworkflow) VALUES ('tc_video', null, null, null);

DELETE FROM DEMO_WORKFLOWORDER_TB;
INSERT INTO DEMO_WORKFLOWORDER_TB (workflowname, jobname, subtype, seq, pool, priority, expool) VALUES ('ca', 'cataloging', 0, 0, 0, 0, 0);
INSERT INTO DEMO_WORKFLOWORDER_TB (workflowname, jobname, subtype, seq, pool, priority, expool) VALUES ('tc', 'transcoding', 0, 0, 0, 0, 0);
INSERT INTO DEMO_WORKFLOWORDER_TB (workflowname, jobname, subtype, seq, pool, priority, expool) VALUES ('tc_video', 'convert_video', 0, 0, 0, 0, 0);


-- RESET --
DELETE FROM DEMO_COMPJOBQUEUE_TB;
DELETE FROM DEMO_COMPSERVER_TB;
DELETE FROM DEMO_COMPSERVERJOB_TB;
DELETE FROM DEMO_WORKFLOWHIS_TB;