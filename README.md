# OptionMaster
C/C++ build configuration simple Demo
'''
整体的思路：
1、构造任务队列pageQueue ,存放所有要爬取的页面url
2、用多线程爬虫从公式网中爬取内容,然后将抓取的页面内容存放到data_queue中
3、用多线程程序对data_queue中的页面内容进行解析,提取公式
4、用单线程对公式进行解析
然后存放到的json文件中(一个时间点只有一个线程可以写文件IO,注意到Python的多线程机制使用了GIL锁)
'''
import threading
import requests
from queue import Queue

import threading
import json
import ctypes
import os
import uuid
import sys,os
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # __file__获取执行文件相对路径，整行为取上一级的上一级目录
sys.path.append(BASE_DIR)
from bs4 import BeautifulSoup
import pandas as pd
from datetime import datetime

'''
Queue.qsize(队列名) #返回队列的大小
Queue.empty(队列名) # 队列为空返回true,否则为false
Queue.full(队列名) # 队列满返回true
Queue.get(队列名,值) # 出队
Queue.put(队列名,值) # 入队
FIFO 先进先出
'''

CUR_PATH = os.path.dirname(__file__)

result_list=[]
script_result_list=[]
flag = False
pageQueue = Queue(1000000) # 任务队列，存放网页的队列


header_info = 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3371.0 Safari/537.36'


class Crawl_thread(threading.Thread):
    '''
    抓取线程类,注意需要继承线程类Thread
    '''

    crawl_lxlt_url='https://www.55188.com/thread-{}-1-1.html'; #理想论坛url
    def __init__(self, thread_id, index_file, event):
        threading.Thread.__init__(self)  # 需要对父类的构造函数进行初始化
        self.thread_id = thread_id
        self.event = event
        self.index_file=index_file
    
    def run(self):
        '''
        线程在调用过程中就会调用对应的run方法
        :return:
        '''
        print('启动采集线程：', self.thread_id)
        self.crawl_spider()
        print('退出采集线程：', self.thread_id)


    def crawl_spider(self):
        while True:
            if pageQueue.empty():  # 如果队列为空，则跳出
                break
            else:
                page = pageQueue.get()
                print('当前id:', page ,'\n')
                url = self.crawl_lxlt_url.format(page)
                headers = {
                    'User-Agent': header_info,
                    'Content-Type':'text/html; charset=utf-8'
                }
                try:
                    response = requests.get(url,timeout=0.6, headers=headers)
                    if response.status_code == 404:
                        print(page," is 404\n")
                        continue
                    if response.status_code != 404:
                        response.encoding = response.apparent_encoding
                        soup = BeautifulSoup(response.text,"html.parser") 
                        table = soup.find_all("div", class_="z")
                        is_outer_break=False
                        title=""
                        for table_child in table:
                            a_list=table_child.find_all('a')
                            is_find= False
                            for a_element in a_list:
                               if is_find:
                                    title = a_element.text
                                    is_outer_break=True
                                    break
                               if a_element.get("href") == "forum-90-1.html" and a_element.text == "指标编写_技术互助答疑论坛":
                                    # 获取满足条件的<a>元素的下一个元素
                                    is_find=True   
                            if is_outer_break:
                                break  # 如果标志变量为True,跳出外部的循环     itemprop="articleBody"
                        if title=="":
                            print("标题为空\n")
                            continue
                        table_content = soup.find_all("div", class_="nei")
                        for content_child in table_content:
                            if content_child.get("itemprop") == "articleBody":
                                find_text=content_child.text
                                index = find_text.find("[ 本帖最后")
                                if index != -1:
                                    result_string = find_text[:index]
                                else:
                                    # 如果没有找到子字符串，则保持原始字符串不变
                                    result_string = find_text
                                print("成功的内容:",result_string,'\n')

                    if result_string!='':
                        script_response = {
                                        'thread_id':page,
                                        'title': title,
                                        'content': result_string,
                                         }  
                        script_result_list.append(script_response)
                        #json.dump(script_response, fp=self.index_file,
                              #ensure_ascii=False)  # 存放json文件
                except Exception as e:
                    print('采集线程错误', e)


def main():
    # 获取当前日期和时间，精确到分钟
    current_datetime = datetime.now().strftime('%Y-%m-%d_%H-%M')

    # 创建文件夹（如果不存在）
    folder_path = os.path.join(current_datetime)
    os.makedirs(folder_path, exist_ok=True)
    json_file_path = os.path.join(current_datetime, 'index_list.json')
    index_file = open(json_file_path, 'a', 
                          encoding='utf-8')  # 将结果保存到一个json文件中

    #for page in range(8171000, 13517010, 1):  # 184-230664 step 10
    for page in range(8177000, 8187000, 1):  
        pageQueue.put(page) # 构造任务队列

    # 初始化采集线程
    crawl_threads = []
    crawl_name_list = ['crawl_1','crawl_2','crawl_3','crawl_4','crawl_5','crawl_6','crawl_7','crawl_8','crawl_9','crawl_10']
    for thread_id in crawl_name_list:
        thread = Crawl_thread(thread_id, index_file,
                              threading.Event())  # 启动爬虫线程
        thread.start()  # 启动线程
        crawl_threads.append(thread)

    # 等待队列情况，对采集的页面队列中的页面进行解析，等待所有页面解析完成
    while not pageQueue.empty():
        pass
    # 通知线程退出
    global flag
    flag = True
    index_file.close()
    with open(json_file_path,'w', encoding="utf8") as file:
            json.dump(script_result_list,file,ensure_ascii=False)
    df_json=pd.read_json(json_file_path)
    excel_file_path = os.path.join(current_datetime, 'index_list.xlsx')
    df_json.to_excel(excel_file_path)

    print('退出主线程')

if __name__ == '__main__':
    main()
