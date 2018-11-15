# -*- coding: utf-8 -*-
"""
@author: konantech
"""
import os
from collections import OrderedDict
from selenium import webdriver
import time
import re
from pytube import request
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from pytube.compat import install_proxy

class SiteHandler(object):

    def __init__(self, search_url = '' ):

        self.search_sid = 0
        self.search_url_str = search_url
        self.search_error = 0
        self.search_video_infos = []
        self.search_video_links = []
        self.search_proxy = ''
        self.search_startswith_str = ''
        self.site_name = ''
        self.site_main_url = ''
        self.driver = None
        self.driver_option = None
        self.site_login_url = ''
        # 다운로드 갯수 제한
        self.MAX_DOWNLOAD_COUNT = 300

    def init_option(self):
        self.driver_option = webdriver.ChromeOptions()
        prefs = {
            "profile.default_content_setting_values.plugins": 1,
            "profile.content_settings.plugin_whitelist.adobe-flash-player": 1,
            "profile.content_settings.exceptions.plugins.*,*.per_resource.adobe-flash-player": 1,
            "credentials_enable_service": False,
            "profile.password_manager_enabled": False
        }
        self.driver_option.add_argument("--headless");
        self.driver_option.add_argument("--no-sandbox");
        self.driver_option.add_argument("--disable-dev-shm-usage");
        self.driver_option.add_argument("--proxy-server=%s" % self.search_proxy)
        self.driver_option.add_argument("user-agent=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36")
        self.driver_option.add_argument("lang=ko_KR")
        self.driver_option.add_experimental_option("prefs",prefs)

        if self.search_proxy != '':
            install_proxy({"http": self.search_proxy, "https": self.search_proxy})
        else:
            install_proxy({})

    def site_login_process(self, login_id, login_pass):
        return False

    def site_login(self, user_id, user_pass):
        if user_id != '' and user_pass != '':
            self.driver.get(self.site_login_url)
            self.site_login_process(user_id, user_pass)

    def start(self):
        chromepath = os.getcwd() + '/chromedriver'

        self.init_option()
        self.driver = webdriver.Chrome(chromepath, chrome_options = self.driver_option)

        return True

    def end(self):
        if self.driver != None:
            self.driver.quit()
            self.driver = None

    def build_link(self):
        if self.get_video_id(self.search_url_str) == '':
            return self.get_all_video_link_fr_url(self.search_url_str)
        else:
            self.search_video_links.append(self.search_url_str)
            return True

    def build_info(self):
        self.get_all_video_info()

    def get_find_element_str(self, search_url):
        return '//a[@class="link_item"]'

    def find_element_results(self, by, value):
        return self.driver.find_elements(by, value)

    def get_video_id(self, target_url):
        return ''

    def is_add_video_link(self, search_url, href):
        return None

    def auto_scroll_web_page(self, limit=10):
        body = self.driver.find_element_by_tag_name('body')
        while limit:
            body.send_keys(Keys.END)
            limit -= 1;
            time.sleep(1)

    def get_all_video_link_fr_url(self, search_url):

        link_cnt = len(self.search_video_links)
        if link_cnt >= self.MAX_DOWNLOAD_COUNT:
            return True

        self.driver.get(search_url)
        time.sleep(1)

        if bool(re.search('500 Internal Server Error', self.driver.page_source)):
            self.save_get_video_info_page('main_search_page.html', self.driver.page_source)
            print '500 Error : ' + search_url
            return False

        elements = self.find_element_results(By.XPATH, self.get_find_element_str(search_url))
        curr_cnt = len(elements)

        while (link_cnt + curr_cnt) < self.MAX_DOWNLOAD_COUNT:
            self.auto_scroll_web_page(1)

            elements = self.find_element_results(By.XPATH, self.get_find_element_str(search_url))
            check_cnt = len(elements)

            if curr_cnt == check_cnt:
                break
            else:
               curr_cnt = check_cnt

        playlist_link_results = []
        for el in elements:
            playlist_link_results.append(el.get_attribute('href'))

        playlist_link_results = list(OrderedDict.fromkeys(playlist_link_results))
        playlist_link_results = [n for n in playlist_link_results if self.is_add_video_link(search_url, n) != None]

        for n in playlist_link_results:
            self.search_video_links.append(n)
            link_cnt += 1
            if link_cnt >= self.MAX_DOWNLOAD_COUNT:
                break

        # 파일 저장
        self.save_get_video_info_page('main_search_page.html', self.driver.page_source)

        return True

    def get_video_info(self, url, seq):
        video_info = {
                        'seq':seq,
                        'title':'',
                        'description':'',
                        'videoid':'',
                        'type':'',
                        'subtype':'',
                        'url':'',
                        'category':'',
                        'license':'',
                        'filename':''
                     }
        return video_info

    def get_all_video_info(self):
        seq = 1
        for url in self.search_video_links:
            video_info = self.get_video_info(url, seq)
            if video_info['videoid'] != '':
                self.search_video_infos.append(video_info)
                seq += 1

    def download_file_headers(self, video_info):
        headers = request.get(video_info['url'], headers=True)
        return headers

    def download_file(self, video_info):
        #output_path = os.getcwd() + '/download'
        #output_path = '/usr/share/tomcat/webapps/crawler'
        output_path = '(path)/download/{sid}'.format(sid=self.search_sid)
        if not os.path.isdir(output_path):
            os.mkdir(output_path)

        filename = video_info['filename']

        # file path
        fp = os.path.join(output_path, filename)
        with open(fp, 'wb') as fh:
            for chunk in request.get(video_info['url'], streaming=True):
                # reduce the (bytes) remainder by the length of the chunk.
                fh.write(chunk)

    def download_files(self):
        for info in self.search_video_infos:
            self.download_file(info)

    def save_get_video_info_page(self, filename, html):
        try:
            output_path = os.getcwd() + '/html/{sid}'.format(sid=self.search_sid)
            if not os.path.isdir(output_path):
                os.mkdir(output_path)

            fp = os.path.join(output_path, filename)
            if os.path.isfile(fp):
                os.remove(fp)
            fid = open(fp, "wb")
            fid.write(html)
            fid.close()
        except:
            print 'save_get_video_info_page({filename})'.format(filename=filename)
