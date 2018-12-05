# -*- coding: utf-8 -*-
"""
@author: konantech
"""

import re
import time
import pymysql
import YouTube_Handler
import socket
import random
import os
import requests

class KonanCrawler(object):

    def __init__(self):

        self.search_sid = 0
        self.search_url = ''
        self.search_site = ''
        self.search_count = 0
        self.search_type = 1
        self.search_ext = '*'
        self.search_max_size = 100
        self.search_site_user_id = ''
        self.search_site_user_pw = ''

        self.search_handler = None
        self.db_connecter = None
        self.db_cursor = None
        self.local_ip = 'localhost'
        self.public_ip = ''
        self.server_active = 'N'
        self.server_sid = 0
        self.server_next_sid = 0
        self.server_state = 0
        self.ipactive_scripts = ['','','']
        self.next_scripts_no = 0

    def init_db_connection(self):
        try:
            self.db_connecter = pymysql.connect(
                    host='(IP)',
                    user='(USER)',
                    password='(PASSWORD)',
                    db='(DB)',
                    charset='utf8',
                    port=(PORT),
                    autocommit=True)
            self.db_cursor = self.db_connecter.cursor(pymysql.cursors.DictCursor)

            return True
        except:
            return False

    def init(self):
        if self.init_db_connection() == False:
            return False

        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        self.local_ip = s.getsockname()[0]
        s.close()

        self.server_active = 'N'
        self.next_scripts_no = 0

        use_proxy = 'N'
        sql = "select * from server_info where LOCAL_IP = %s or SECONDARY_IP1 = %s or SECONDARY_IP2 = %s"
        self.db_cursor.execute(sql, (self.local_ip, self.local_ip, self.local_ip))
        rows = self.db_cursor.fetchall()
        if len(rows) > 0:
            for row in rows:
                self.server_sid = row['SID']
                self.server_next_sid = row['NEXT_SID']
                self.public_ip = row['SERVER_IP']
                self.server_active = row['SERVER_ACTIVE']

                if row['LOCAL_IP'] == self.local_ip:
                    self.ipactive_scripts[0] = row['IPACTIVE_SCRIPT02']
                    self.ipactive_scripts[1] = row['IPACTIVE_SCRIPT03']
                elif row['SECONDARY_IP1'] == self.local_ip:
                    self.ipactive_scripts[0] = row['IPACTIVE_SCRIPT03']
                    self.ipactive_scripts[1] = row['IPACTIVE_SCRIPT01']
                else:
                    self.ipactive_scripts[0] = row['IPACTIVE_SCRIPT01']
                    self.ipactive_scripts[1] = row['IPACTIVE_SCRIPT02']

                use_proxy = row['USE_PROXY']

                break

        if self.server_active == 'N':
            return False

        sql = "select * from schedule_info where SC_STATE < 2 order by SID"
        self.db_cursor.execute(sql)
        rows = self.db_cursor.fetchall()
        if len(rows) > 0:
            for row in rows:
                self.search_sid  = row['SID']
                self.search_site = row['SC_SITE']
                self.search_url  = row['SC_URL']
                self.search_count= row['SC_COUNT']
                self.search_type = row['SC_TYPE']
                self.search_ext  = row['SC_EXT']
                self.server_state = row['SC_STATE']
                self.search_max_size = row['SC_MAXSIZE'] * 1024 * 1024
                if self.search_max_size == 0:
                    self.search_max_size = 50*1024*1024

                break

            sql = "select * from domain_info where DOMAIN = %s"
            self.db_cursor.execute(sql, (self.search_site))
            rows = self.db_cursor.fetchall()
            if len(rows) > 0:
                for row in rows:
                    self.search_site_user_id = row['USER_ID']
                    self.search_site_user_pw = row['USER_PASS']
                    break

            if bool(re.search('youtube.com', self.search_url.lower())):
                self.search_site = r'https://www.youtube.com'
                self.search_handler = YouTube_Handler.YouTubeHandler(self.search_url)
            else:
                return False

            self.reset_use_proxy(use_proxy)

            return True
        else:
            return False

    def reset_use_proxy(self, use_proxy):
        old_proxy = self.search_handler.search_proxy
        if use_proxy == 'Y':
            if self.search_handler.search_proxy == '':
                sql = "select * from proxy_list where FAIL < 100"
                self.db_cursor.execute(sql)
                rows = self.db_cursor.fetchall()
                if len(rows) > 0:
                    proxy_list = []
                    for row in rows:
                        proxy_list.append(row['PROXY'])

                    random.shuffle(proxy_list)
                    for proxy in proxy_list:
                        try:
                            r = requests.get(self.search_site, proxies={"http": proxy, "https": proxy})
                            if r.status_code == 200:
                                self.search_handler.search_proxy = proxy
                                break
                            else:
                                sql = "update proxy_list set FAIL = FAIL + 1 where PROXY = %s"
                                self.db_cursor.execute(sql, (proxy))
                        except:
                            sql = "update proxy_list set FAIL = FAIL + 1 where PROXY = %s"
                            self.db_cursor.execute(sql, (proxy))
        else:
             self.search_handler.search_proxy = ''

        if old_proxy != self.search_handler.search_proxy:
            self.search_handler.end()
            self.search_handler.start()

    def check_server_active(self):
        isActive = False
        sql = "select * from server_info where LOCAL_IP = %s or SECONDARY_IP1 = %s or SECONDARY_IP2 = %s"
        self.db_cursor.execute(sql, (self.local_ip, self.local_ip, self.local_ip))
        rows = self.db_cursor.fetchall()
        if len(rows) > 0:
            for row in rows:
                if row['SERVER_ACTIVE'] == 'Y':
                    isActive = True
                    self.reset_use_proxy(row['USE_PROXY'])

                break

        return isActive

    def check_schedule_info_state(self):
        scState = 0
        sql = "select * from schedule_info where SID = %s"
        self.db_cursor.execute(sql, (self.search_sid))
        rows = self.db_cursor.fetchall()
        if len(rows) > 0:
            for row in rows:
                scState = row['SC_STATE']
                break

        return scState

    def update_schedule_info_state(self, state):
        if self.search_handler != None:
            sql = "update schedule_info set SC_STATE = %s where SID = %s"
            self.db_cursor.execute(sql, (state, self.search_sid))

    def update_schedule_info_s_count(self):
        if self.search_handler != None:
            sql = "update schedule_info set SC_COUNT = %s where SID = %s"
            self.db_cursor.execute(sql, (len(self.search_handler.search_video_infos), self.search_sid))

    def update_schedule_info_e_count(self):
        if self.search_handler != None:
            sql = "update schedule_info set SC_COUNT_E = %s where SID = %s"
            self.db_cursor.execute(sql, (self.search_handler.search_error, self.search_sid))

    def update_schedule_info_e_count_inc(self):
        if self.search_handler != None:
            sql = "update schedule_info set SC_COUNT_E = SC_COUNT_E + 1 where SID = %s"
            self.db_cursor.execute(sql, (self.search_sid))

    def update_schedule_info_d_count_inc(self):
        if self.search_handler != None:
            sql = "update schedule_info set SC_DOWNLOAD = SC_DOWNLOAD + 1 where SID = %s"
            self.db_cursor.execute(sql, (self.search_sid))

    def check_db_connection(self):
        try:
            if self.db_cursor.connection:
                return True
            else:
                return self.init_db_connection()
        except:
            return False

    def insert_crawler_info(self, video_info):
        try:
            if self.check_db_connection() == False:
                print 'DB CONNECT ERROR'
                return False

            sql = "insert into crawler_info (SID, SEQ, TITLE, CATEGORY, LICENSE, CONTENTS, MAINTYPE, SUBTYPE, VIDEOID, FILENAME, FILE_URL, SERVER_IP, INPUT_DATE, PROXY_SERVER) Values(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, Now(), %s)"
            self.db_cursor.execute(
                sql,
                (
                    self.search_sid,
                    video_info['seq'],
                    video_info['title'].encode("utf-8"),
                    video_info['category'],
                    video_info['license'],
                    video_info['description'].encode("utf-8"),
                    video_info['type'],
                    video_info['subtype'],
                    video_info['videoid'],
                    video_info['filename'],
                    video_info['url'],
                    self.public_ip,
                    self.search_handler.search_proxy
                )
            )
        except:
            print self.search_sid
            print video_info['seq']
            print video_info['title'].encode("utf-8")
            print video_info['category']
            print video_info['license']
            print video_info['description'].encode("utf-8")
            print video_info['type']
            print video_info['subtype']
            print video_info['videoid']
            print video_info['filename']
            print video_info['url']
            print self.public_ip
            print self.search_handler.search_proxy
            print sql

    def update_crawler_issuccess(self, success, seq):
        sql = "update crawler_info set ISSUCCESS = %s, SERVER_IP = %s, PROXY_SERVER = %s where SID = %s and SEQ = %s"
        self.db_cursor.execute(sql, (success, self.public_ip, self.search_handler.search_proxy, self.search_sid, seq))

    def update_schedule_info_complete(self):
        if self.search_handler != None:
            sql = "update schedule_info set SC_STATE = 2, END_DATE = Now() where SID = %s"
            self.db_cursor.execute(sql, (self.search_sid))

    def close(self):
        if self.db_connecter != None:
            self.db_cursor.close()
            self.db_connecter.close()

    def check_next_server(self):
        checkval = False
        for idx in range(self.next_scripts_no, 2):
            if self.ipactive_scripts[idx] != '':
                self.next_scripts_no += 1
                execscript = './{script}'.format(script=self.ipactive_scripts[idx])
                os.system(execscript)
                time.sleep(5)
                checkval = True
            else:
                # 더이상 연결할 네트워크가 없음으로
                # 서버 활성화를 중단하고 다른 서버에서 작업하도록 전달하고 종료

                # 현재 서버 ACTIVE 끄고
                sql = "update server_info set SERVER_ACTIVE = '%s where SID = %s"
                self.db_cursor.execute(sql, ('N',self.server_sid))

                # 다음 서버 ACTIVE 켜고
                sql = "update server_info set SERVER_ACTIVE = '%s where SID = %s"
                self.db_cursor.execute(sql, ('Y',self.server_next_sid))

                self.search_handler.end()
                self.close()

            break
        return checkval

    def run(self):
        if self.init() == True:
            # MULTI NIC 설정 처리

            if self.search_handler.start() == True:
                self.search_handler.search_sid = self.search_sid
                # 상태를 시작으로 변경
                if self.server_state == 0 or self.search_count == 0:

                    #아직 시작 안했음으로 모든 정보를 모은다.
                    self.update_schedule_info_state(1)

                    self.search_handler.site_login(self.search_site_user_id, self.search_site_user_pw)
                    if self.search_handler.build_link() == False:
                        # 링크를 가져오는걸 실패 하였음 일단 종료후 다시 시도
                        self.check_next_server()
                        self.search_handler.end()
                        self.close()
                        return False

                    self.search_handler.get_all_video_info()

                    self.update_schedule_info_s_count()
                    self.update_schedule_info_e_count()

                    # 우선 정보 먼저  디비 기록
                    for video_info in self.search_handler.search_video_infos:
                        self.insert_crawler_info(video_info)
                else:
                    # 여기서 디비에 수집된 정보를 바탕으로 다시 크롤링 다운로드 목록을 만들어준다.
                    # 다운로드 할때 필요할지 모르니 로그인은 진행
                    self.search_handler.site_login(self.search_site_user_id, self.search_site_user_pw)
                    sql = "select * from crawler_info where SID = %s and ISSUCCESS = %s order by SEQ"
                    self.db_cursor.execute(sql, (self.search_sid, 'N'))
                    rows = self.db_cursor.fetchall()
                    if len(rows) > 0:
                        for row in rows:
                            video_info = {
                                            'seq':row['SEQ'],
                                            'title':row['TITLE'],
                                            'description':row['CONTENTS'],
                                            'videoid':row['VIDEOID'],
                                            'type':row['MAINTYPE'],
                                            'subtype':row['SUBTYPE'],
                                            'url':row['FILE_URL'],
                                            'category':row['CATEGORY'],
                                            'license':row['LICENSE'],
                                            'filename':row['FILENAME']
                                         }
                            self.search_handler.search_video_infos.append(video_info)

                # 파일 다운로드 진행
                for video_info in self.search_handler.search_video_infos:
                    if self.check_server_active() == False:
                        self.search_handler.end()
                        self.close()
                        return False

                    if self.check_schedule_info_state() != 1:
                        self.search_handler.end()
                        self.close()
                        return False

                    start_time = time.time()

                    # self.update_crawler_issuccess('G', video_info['seq'])
                    issuccess = 'Y'
                    checkserver = False

                    if video_info['videoid'] != '':
                        try:
                            headers = self.search_handler.download_file_headers(video_info)
                            if bool(re.search('video', headers['content-type'])):
                                contentlength = int(headers['content-length'])

                                if self.search_max_size < contentlength:
                                    issuccess = 'E3'
                                else:
                                    self.search_handler.download_file(video_info)
                            else:
                                # 여기서 파일이 비디오가 아닐경우 차단된것으로 판단
                                # 네트워크 변경 처리
                                checkserver = True
                        except:
                            checkserver = True
                    else:
                        issuccess = 'E0'

                    if checkserver == True:
                        self.check_next_server()
                        self.search_handler.end()
                        self.close()
                        return False

                    self.update_crawler_issuccess(issuccess, video_info['seq'])
                    if issuccess == 'Y':
                        self.update_schedule_info_d_count_inc()
                    else:
                        self.update_schedule_info_e_count_inc()

                    download_time = int(round(time.time()-start_time, 0))
                    if download_time <= 30:
                        time.sleep(random.randint(31-download_time, 41-download_time))

                self.update_schedule_info_complete()
                self.search_handler.end()

        self.close()

if __name__ == '__main__':

    while 1:
    	mycrawler = KonanCrawler()
    	mycrawler.run()

    	time.sleep(30)
