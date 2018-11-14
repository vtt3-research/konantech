(주)코난테크놀로지 크롤러
=======================

코난테크놀로지 크롤러는 주어진 URL로 부터 수집 가능한 동영상(카테고리, 라이센스 등)의 정보를 수집 할 수 있도록 도와줍니다.

* * *

Pre-Installation
----------------

- Python2.7 [ https://www.python.org/download/releases/2.7/ ]
  - pymysql : for connect DB
  - selenium : framework for web testing
  - pytube : library for downloading YouTube Videos
  - chromedriver : chromedriver for crawling
- Java
- Spring framework
- Tomcat7 [ https://tomcat.apache.org/download-70.cgi ]
- MariaDB [ https://mariadb.org/download/ ] or MySQL [ https://dev.mysql.com/downloads/mysql/ ]

* * *

Installation (Ubuntu)
---------------------

- Crawler installation  
  - Copy the files in the folder(named 'python') to the desired path (A).
  - Register script execution in '/etc/rc.local' file.
``` bash
sudo vi /etc/rc.local

> sh (A)/crawler_start
> # desired path (A)
```
  - Create a writable folder(named 'html') in path (A)


- Table definition
  - use SQL file [ https://github.com/vtt3-research/konantech/blob/master/vtt-crawler/konan_crawler.sql ]

* * *

Post-Installation
-----------------

- Add admin account
  - Adding an account must be done directly from an account with administrator rights, so you must first create an administrator account.
``` SQL
INSERT INTO MEMBER(`ID`, `PASSWORD`, `ISADMIN`) VALUES('(A)', '(B)', 'Y');
-- Enter the administrator ID in (A) and the password in (B).
```

- Add server information
  - You must enter the crawling server information to make the crawling as normal.
``` SQL
INSERT INTO SERVER_INFO('SERVER_NAME', 'SERVER_IP', 'LOCAL_IP', 'NEXT_SID') VALUES('(A)', '(B)', '(C)', '(D)');
-- Enter the SERVER_NAME ID in (A), the SERVER_IP in (B), the LOCAL_IP in (C), and the NEXT_SID in (D).
```

- Change DB properties
  - (IP) : IP Address of server.
  - (PORT) : The port number that can connect to the DB.
  - (DB) : The name of the DB to connect. (the table must be defined)
  - (USER) : User name that can access DB.
  - (PASSWORD) : Password for user name.
> python code "python\\Konan_Crawler.py" line 45~51
``` python
self.db_connecter = pymysql.connect(host='(IP)',
                                      user='(USER)',
                                      password='(PASSWORD)',
                                      db='(DB)',
                                      charset='utf8',
                                      port=(PORT),
                                      autocommit=True)
```
> spring project "website\\src\\main\\resources\\properties\\\*\\db.pproperties"
```XML
jdbc.driverClassName=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://(IP):(PORT)/(DB)?characterEncoding=utf8&amp;useSSL=false
jdbc.username=(USER)
jdbc.password=(PASSWORD)
jdbc.validationQuery=SELECT 1
```

- Set download folder path
  - Default download folder path : /var/www/html
    - If you want to change the path, edit Site_Handler.py (line 184)
``` python
output_path = '(path)/download/{sid}'.format(sid=self.search_sid)
# Change path to the desired (path)
```
  - Create a writable folder(named 'download') in the download folder path

* * *

Run (Ubuntu)
--------------

- Run crawler
``` bash
cd /etc/rc.d
./rc.local
```
  - If you have more than one crawling server, each crawling server should have a crawler running. You also need to enter additional server information in the database.


- Run spring project

* * *

USAGE (Web)
--------------

1. Login

2. Create a Collector Account

3. Proxy Server Registration (Optional)

4. Schedule registration (스케줄 등록)
  - Enter schedule name
    - Enter an identifiable schedule name
  - Enter Collection URL
    - URL of the searched list in youtube
      - Use when you want to collect multiple videos
      - ex. [ https://www.youtube.com/results?search_query=%EC%BD%94%EB%82%9C%ED%85%8C%ED%81%AC%EB%86%80%EB%A1%9C%EC%A7%80 ]
    - URL of video play status
      - Use when you want to collect only videos that are playing
      - ex. [ https://www.youtube.com/watch?v=unwYxR7HF3Y ]
  - Edit maximum size (최대 크기)
    - If the size of the video to be collected is expected to be large, it should be edited.


5. Schedule inquiry (스케줄 조회)
  - Error information
    - E0 : Failed to get file ID
    - E3 : The file is larger than the capacity defined at acquisition.

* * *

**Copyright (주)코난테크놀로지. All Rights Reserved.**
