
-- oracle 12c example 1 -----------------------------------------------------------

DROP TABLE t1;
CREATE TABLE t1 (col1 VARCHAR2(100));
INSERT INTO t1 VALUES ( '[ "LIT192", "CS141", "HIS160" ]' );
INSERT INTO t1 VALUES ( '{ "Name": "John" }' );
INSERT INTO t1 VALUES ( '{ "Grade Values" : { A : 4.0, B : 3.0, C : 2.0 } }');
INSERT INTO t1 VALUES ( '{ "isEnrolled" : true }' );
INSERT INTO t1 VALUES ( '{ "isMatriculated" : False }' );
INSERT INTO t1 VALUES (NULL);
INSERT INTO t1 VALUES ('This is not well-formed JSON data');
INSERT INTO t1 VALUES ('[{first:"John"}, {middle:"Mark"}, {last:"Smith"}]');
INSERT INTO t1 VALUES ('{family : {id:10, ages:[40,38,12], address : {street : "10 Main Street"}}}');

SELECT col1 FROM t1 WHERE col1 IS JSON; -- 1,2,3,4,5
SELECT col1 FROM t1 WHERE col1 IS JSON STRICT; -- 1,2,4
SELECT col1 FROM t1 WHERE col1 IS JSON WITH UNIQUE KEYS;
SELECT col1 FROM t1 WHERE JSON_EXISTS(col1, '$[0].first');
SELECT col1 FROM t1 WHERE JSON_EXISTS(col1, '$[1].middle');

-- oracle 12c example 2 -----------------------------------------------------------

DROP TABLE t2;
CREATE TABLE t2 (col1 VARCHAR2(200));
CREATE INDEX t2_ix ON t2(col1) INDEXTYPE IS CTXSYS.CONTEXT PARAMETERS ('SECTION GROUP CTXSYS.JSON_SECTION_GROUP SYNC (ON COMMIT)');
INSERT INTO t2 VALUES ('{family : {id:10, ages:[40,38,12], address : {street : "10 Main Street"}}}');
INSERT INTO t2 VALUES ('{family : {id:11, ages:[42,40,10,5], address : {street : "200 East Street", apt : 20}}}');
INSERT INTO t2 VALUES ('{family : {id:12, ages:[25,23], address : {street : "300 Oak Street", apt : 10}}}');

SELECT col1 FROM t2 WHERE JSON_TEXTCONTAINS(col1, '$', '10');
SELECT col1 FROM t2 where JSON_TEXTCONTAINS(col1, '$.family.id', '11');
SELECT col1 FROM t2 WHERE JSON_TEXTCONTAINS(col1, '$.family.ages', '40');
SELECT col1 FROM t2 WHERE JSON_TEXTCONTAINS(col1, '$.family.address', 'East');


-- posgresql example 1 -----------------------------------------------------------
DROP TABLE t1;
CREATE TABLE t1 (col1 JSON);
INSERT INTO t1 VALUES ( '[{"a":"foo"},{"b":"bar"},{"c":"baz"}]' );
INSERT INTO t1 VALUES ( '{"a": {"b":"foo"}}' );
INSERT INTO t1 VALUES ( '[1,2,3]' );
INSERT INTO t1 VALUES ( '{"a":1,"b":2}' );
INSERT INTO t1 VALUES ( '{"a": {"b":{"c": "foo"}}}' );
INSERT INTO t1 VALUES ( '{"a":[1,2,3],"b":[4,5,6]}' );


SELECT * FROM t WHERE col1->>'Name' = 'John';


DROP TABLE t2;
CREATE TABLE t2 (col1 JSONB);
INSERT INTO t2 VALUES ( '[{"a":"foo"},{"b":"bar"},{"c":"baz"}]' );
INSERT INTO t2 VALUES ( '{"a": {"b":"foo"}}' );
INSERT INTO t2 VALUES ( '[1,2,3]' );
INSERT INTO t2 VALUES ( '{"a":1,"b":2}' );
INSERT INTO t2 VALUES ( '{"a": {"b":{"c": "foo"}}}' );
INSERT INTO t2 VALUES ( '{"a":[1,2,3],"b":[4,5,6]}' );

SELECT * FROM t2 WHERE col1 @> '{"b":2}';
SELECT * FROM t2 WHERE col1::jsonb @> '{"b":2}'::jsonb;
