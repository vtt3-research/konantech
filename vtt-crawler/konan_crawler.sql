-- 테이블 순서는 관계를 고려하여 한 번에 실행해도 에러가 발생하지 않게 정렬되었습니다.

-- SCHEDULE_INFO Table Create SQL
CREATE TABLE SCHEDULE_INFO
(
    `SID`         INT             NOT NULL    AUTO_INCREMENT COMMENT '고유번호', 
    `S_NAME`      VARCHAR(45)     NOT NULL    COMMENT '스케줄 등록명', 
    `S_SITE`      VARCHAR(45)     NOT NULL    COMMENT '수집사이트', 
    `S_URL`       VARCHAR(200)    NOT NULL    COMMENT '수집위치', 
    `S_TYPE`      INT             NOT NULL    COMMENT '수집형태(동영상 1, 이미지 2, 텍스트 3)', 
    `S_EXT`       VARCHAR(45)     NULL        COMMENT '수집 확장자', 
    `S_MAXSIZE`   INT             NULL        COMMENT '수집파일 최대크기', 
    `S_STATE`     INT             NOT NULL    COMMENT '수집상태(대기 0, 수집중 1, 완료 2, 오류 3)', 
    `S_COUNT`     INT             NOT NULL    COMMENT '수집수', 
    `USER_ID`     VARCHAR(20)     NOT NULL    COMMENT '수집자 아이디', 
    `DELETE_YN`   VARCHAR(2)      NULL        COMMENT '삭제유무(Y or N)', 
    `INPUT_DATE`  DATETIME        NOT NULL    COMMENT '등록일', 
    `END_DATE`    DATETIME        NULL        COMMENT '완료일', 
    PRIMARY KEY (SID)
);

ALTER TABLE SCHEDULE_INFO COMMENT '크롤러 스케줄';


-- MEMBER Table Create SQL
CREATE TABLE MEMBER
(
    `ID`        VARCHAR(20)    NOT NULL    COMMENT '아이디', 
    `PASSWORD`  VARCHAR(20)    NOT NULL    COMMENT '패스워드', 
    `ISADMIN`   VARCHAR(2)     NOT NULL    COMMENT '관리자유무(Y or N)', 
    PRIMARY KEY (ID)
);

ALTER TABLE MEMBER COMMENT '회원정보';


-- DOMAIN_INFO Table Create SQL
CREATE TABLE DOMAIN_INFO
(
    `DOMAIN`      VARCHAR(45)    NOT NULL    COMMENT '도메인', 
    `USER_ID`     VARCHAR(45)    NOT NULL    COMMENT '로그인 아이디', 
    `USER_PASS`   VARCHAR(45)    NOT NULL    COMMENT '로그인 패스워드', 
    `INPUT_DATE`  DATETIME       NOT NULL    COMMENT '등록일', 
    `INPUT_ID`    VARCHAR(20)    NOT NULL    COMMENT '등록자', 
    PRIMARY KEY (DOMAIN)
);

ALTER TABLE DOMAIN_INFO COMMENT '도메인 로그인 정보';


-- CRAWLER_INFO Table Create SQL
CREATE TABLE CRAWLER_INFO
(
    `CID`         INT             NOT NULL    AUTO_INCREMENT COMMENT '크롤링 고유번호', 
    `SEQ`         INT             NOT NULL    COMMENT '크롤링 순번', 
    `SID`         INT             NOT NULL    COMMENT '스케줄 고유번호', 
    `TITLE`       VARCHAR(100)    NULL        COMMENT '등록된 이름', 
    `CATEGORY`    VARCHAR(100)    NULL        COMMENT '카테고리', 
    `LICENSE`     VARCHAR(100)    NULL        COMMENT '라이센스', 
    `CONTENTS`    TEXT            NULL        COMMENT '내용', 
    `FILENAME`    VARCHAR(100)    NULL        COMMENT '파일명', 
    `INPUT_DATE`  DATETIME        NOT NULL    COMMENT '등록일', 
    PRIMARY KEY (CID, SEQ)
);

ALTER TABLE CRAWLER_INFO COMMENT '크롤링된 정보';

ALTER TABLE CRAWLER_INFO ADD CONSTRAINT FK_CRAWLER_INFO_SID_SCHEDULE_INFO_SID FOREIGN KEY (SID) REFERENCES SCHEDULE_INFO (SID)  ON DELETE RESTRICT ON UPDATE RESTRICT;
