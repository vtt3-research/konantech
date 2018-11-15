# -*- coding: utf-8 -*-
"""
@author: konantech
"""

import Konan_Utils
import Site_Handler
import re
import json
import time
from pytube import YouTube
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions

class YouTubeHandler(Site_Handler.SiteHandler):

    def __init__(self, search_url = '' ):
        super(YouTubeHandler, self).__init__(search_url)
        self.site_name = 'youtube'
        self.site_main_url = 'https://www.youtube.com'
        self.site_login_url = 'https://accounts.google.com/signin/v2/sl/pwd?passive=true&hl=ko&service=youtube&continue=https://www.youtube.com&flowName=GlifWebSignIn&flowEntry=ServiceLogin'

    def site_login_process(self, login_id, login_pass):
        self.driver.find_element_by_id("identifierId").send_keys(login_id)
        self.driver.find_element_by_id("identifierNext").click()
        time.sleep(1)
        self.driver.find_element_by_name("password").send_keys(login_pass)
        self.driver.find_element_by_id("passwordNext").click()
        time.sleep(1)

    def build_link(self):
        super(YouTubeHandler, self).build_link()

    def get_find_element_str(self, search_url):
        return '//a[@id="video-title"]'

    def get_video_id(self, target_url):
        return Konan_Utils.regex_search(r'(v=)([0-9A-Za-z_-]{11}).*', target_url, group=2)

    def is_add_video_link(self, search_url, href):
        if re.search(r'/watch', href):
            return href
        else:
            return None

    def get_ytplayer_config(self, html):
        pattern = r';ytplayer\.config\s*=\s*({.*?});'
        yt_player_config = Konan_Utils.regex_search(pattern, html, group=1)
        return json.loads(yt_player_config)

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
        self.driver.get(url)
        try:
            WebDriverWait(self.driver, 5).until(
                expected_conditions.presence_of_element_located(
                            (By.XPATH, '//div[@id="description"]')
                    )
                )
        except:
            return video_info

        # 파일 저장
        self.save_get_video_info_page('search_page_{search_seq}.html'.format(search_seq=seq), self.driver.page_source)

        try:
            yt = YouTube(url)
        except:
            print url
            self.search_error += 1
            return video_info

        video_info['title'] = yt.title
        video_info['description'] = ''
        video_info['url'] = ''
        video_info['videoid'] = self.get_video_id(url)
        video_info['url'] = yt.streams.first().url
        video_info['type'] = yt.streams.first().type
        video_info['subtype'] = yt.streams.first().subtype

        try:
            el = self.driver.find_element(By.XPATH, '//div[@id="description"]')
            video_info['description'] = el.text
        except:
            video_info['description'] = ''

        try:
            el = self.driver.find_element_by_id('more')
            el.click()
        except:
            video_info['category'] = ''

        try:
            els = self.driver.find_elements(By.XPATH, '//ytd-metadata-row-renderer')

            cate_pt = re.compile(u'카테고리')
            lice_pt = re.compile(u'라이선스')
            for el in els:
                title = unicode(el.find_element_by_id('title').text)
                content = unicode(el.find_element_by_id('content').text)

                if cate_pt.search(title):
                     video_info['category'] = content
                if lice_pt.search(title):
                     video_info['license'] = content
        except:
            video_info['category'] = 'UnKnown'

        video_info['filename'] = '{sitename}_{filename}.{subtype}'.format(sitename=self.site_name, filename=video_info['videoid'], subtype=video_info['subtype'])

        return video_info
