
DROP TABLE OGQ_JOB_TB;

CREATE TABLE OGQ_JOB_TB (
  idx NUMERIC NOT NULL,
  requestId CHARACTER VARYING(100) NOT NULL,
  videoUrl CHARACTER VARYING(512) NOT NULL,
  callbackUrl CHARACTER VARYING(512) NOT NULL,
  resultString TEXT,
  createDate TIMESTAMP(6) WITHOUT TIME ZONE,
  updateDate TIMESTAMP(6) WITHOUT TIME ZONE,
  status NUMERIC NOT NULL,
  videoid NUMERIC NOT NULL,
  isUsed NUMERIC NOT NULL
);


insert into ogq_job_tb values(
11,'129_118_v2 (2).mp4',
'https://s3.ap-northeast-2.amazonaws.com/vtt-ogq/movie_attach/129_118_v2 (2).mp4',
'http://ec2-13-125-78-13.ap-northeast-2.compute.amazonaws.com:8080/callbackReceiver',
'',
now(),now(),0,9,1
)