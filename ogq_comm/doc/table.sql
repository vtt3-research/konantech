
DROP TABLE OGQ_JOB_TB;

CREATE TABLE OGQ_JOB_TB (
  idx NUMERIC NOT NULL,
  requestId CHARACTER VARYING(100) NOT NULL,
  videoUrl CHARACTER VARYING(512) NOT NULL,
  callbackUrl CHARACTER VARYING(512) NOT NULL,
  createDate TIMESTAMP(6) WITHOUT TIME ZONE,
  updateDate TIMESTAMP(6) WITHOUT TIME ZONE,
  status NUMERIC NOT NULL,
  videoid NUMERIC NOT NULL,
  isUsed NUMERIC NOT NULL
);


insert into ogq_job_tb values(
1,'0234567890',
'https://s3.ap-northeast-2.amazonaws.com/awss3free/test/using_ffmpeg_decoder_2.mp4',
'http://183.110.246.21:8080/ogq/response',
now(),now(),0,0,1
)