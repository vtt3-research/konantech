
# oracle
CREATE TABLE t (col1 VARCHAR2(100));
INSERT INTO t VALUES ( '[ "LIT192", "CS141", "HIS160" ]' );
INSERT INTO t VALUES ( '{ "Name": "John" }' );
INSERT INTO t VALUES ( '{ "Grade Values" : { A : 4.0, B : 3.0, C : 2.0 } }');
INSERT INTO t VALUES ( '{ "isEnrolled" : true }' );
INSERT INTO t VALUES ( '{ "isMatriculated" : False }' );
INSERT INTO t VALUES (NULL);
INSERT INTO t VALUES ('This is not well-formed JSON data');



# postgresql
CREATE TABLE t (col1 VARCHAR(100));
INSERT INTO t VALUES ( '[ "LIT192", "CS141", "HIS160" ]' );
INSERT INTO t VALUES ( '{ "Name": "John" }' );
INSERT INTO t VALUES ( '{ "Grade Values" : { A : 4.0, B : 3.0, C : 2.0 } }');
INSERT INTO t VALUES ( '{ "isEnrolled" : true }' );
INSERT INTO t VALUES ( '{ "isMatriculated" : False }' );
INSERT INTO t VALUES (NULL);
INSERT INTO t VALUES ('This is not well-formed JSON data');




CREATE TABLE vtt_meta_tb (
idx NUMERIC NOT NULL,
title CHARACTER VARYING(150),
createuser NUMERIC,
modifyuser NUMERIC,
createtime TIMESTAMP(6) WITHOUT TIME ZONE,
modifytime TIMESTAMP(6) WITHOUT TIME ZONE,
content JSONB,
content2 JSON,
delflag BOOLEAN,
CONSTRAINT pk_vtt_meta_tb PRIMARY KEY (idx));

COMMENT ON TABLE vtt_meta_tb IS '샷';
COMMENT ON COLUMN vtt_meta_tb.idx IS '샷 ID';
COMMENT ON COLUMN vtt_meta_tb.title IS '제목';
COMMENT ON COLUMN vtt_meta_tb.createuser IS '생성자';
COMMENT ON COLUMN vtt_meta_tb.modifyuser IS '수정자';
COMMENT ON COLUMN vtt_meta_tb.createtime IS '생성시각';
COMMENT ON COLUMN vtt_meta_tb.modifytime IS '수정시각';
COMMENT ON COLUMN vtt_meta_tb.delflag IS '삭제 여부';
COMMENT ON COLUMN vtt_meta_tb.content IS '내용';
COMMENT ON CONSTRAINT pk_vtt_meta_tb ON vtt_meta_tb IS '샷 기본키';


INSERT INTO VTT_META_TB ( idx, title, createuser, modifyuser, createtime, modifytime, content, content2, delflag ) VALUES (
  1 , '제목1' , 0, 0, NOW(), NOW(), '{"a": 1}' ,'{"a": 1}' , true );


INSERT INTO VTT_META_TB ( idx, title, createuser, modifyuser, createtime, modifytime, content, content2, delflag ) VALUES (
  2 , '제목2' , 0, 0, NOW(), NOW(), '{"a": 2,"b": 2}' ,'{"a": 2,"b": 2}' , true );


INSERT INTO VTT_META_TB ( idx, title, createuser, modifyuser, createtime, modifytime, content, content2, delflag ) VALUES (
  3 , '제목3' , 0, 0, NOW(), NOW(), '{"a": 3,"b": 2}' ,'{"a": 3,"b": 2}' , true );

INSERT INTO VTT_META_TB ( idx, title, createuser, modifyuser, createtime, modifytime, content, content2, delflag ) VALUES (
  4 , '제목4' , 0, 0, NOW(), NOW(), '{"a": 11,"b": 3}' ,'{"a": 11,"b": 3}' , true );

INSERT INTO VTT_META_TB ( idx, title, createuser, modifyuser, createtime, modifytime, content, content2, delflag ) VALUES (
  5 , '제목5' , 0, 0, NOW(), NOW(), '{"a": 1,"b": 2}' , '{"a": 1,"b": 2}' , true );


