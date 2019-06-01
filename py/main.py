#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import cookielib
import urllib2
import sys
import re
import traceback
import requests
import multiprocessing as mp
import time
import redis


def find_content(content):
	'''regex'''
	#print content
	#<table class="table" style="margin-bottom:0;">
	mypage_info=re.findall(r'<td>(.*?)</td>',content,re.S)
	print "找到的身份证信息:%s\n " % mypage_info
	return mypage_info
def find_morePage(content):
	'''在页面中找到更多的url来爬'''
	more_page=re.findall(r'<a href="(.*?)">(.*?)</a>',content,re.S)
	print "需要爬的页面:%s\n"%more_page
	for w in more_page:
		#爬去页面信息
		#检查数据
		#print   "页面信息:",w[0].decode('utf-8'), w[1].decode('utf-8')
		#print "网址:%s   " %   type(w[1])
		#line=w[0].decode('utf-8')
		line=w[0].decode('utf-8')
		if line.find("http")!=-1:
			print line,w[1].decode('utf-8')
		if r.sadd('unget',line)==1: #添加成功redis
			print "添加纪录到redis ",line
		#print type(w)
	

	print r.smembers('unget')
		
	return 
	
def Spider(url):
	print "身份证下载........"
	#req=urllib2.Request(url)
	#模拟Mozilla浏览器进行爬虫
	#req.add_header("user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36")
	#req.add_header("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8")
	#page = urllib2.urlopen(req)
	headers = {'User-agent' : 'Mozilla/5.0 (Windows NT 6.2; WOW640; rv:22.0) Gecko/20100101 Firefox/22.0'}
	request = urllib2.Request(url, headers = headers)
	page = urllib2.urlopen(request)
	# page
	print page.getcode()
	content=page.read()
	
	finded=find_content(content)
	find_morePage(content) #py
	#print finded.len()
	strr=map(str,finded)
	#print strr
	print "打印身份证信息"
	for s in range(len(finded)/4):
		#身份证信息
		#if isinstance(finded[s],str):
		#	finded[s]=finded[s].decode('utf-8')
		s=s*4
		if finded[s].decode('utf-8').find("<")!=-1:
			finded[s]="Nan"
		print finded[s].decode('utf-8'),finded[s+1].decode('utf-8'),finded[s+2].decode('utf-8'),finded[s+3].decode('utf-8')

	print "长度：%d" % len(page.read())
	
	page.close()

def worker(interval):
    n = 5
    while n > 0:
        print("The time is {0}".format(time.ctime()))
        time.sleep(interval)
        n -= 1

	
	
	
	
	
	
	

if __name__=='__main__':
	#本机有代理,此条清除代理
	os.environ['http_proxy'] = ''
	print "start "
	#url ="http://www.95nw.com/"
	url="http://sfzdq.uzuzuz.com/"
	#url="http://m.diqibu.com/wap/sfzdq/index.html"
	r = redis.StrictRedis(host='localhost', port=6379, db=0)
	#r.sadd('unget','www.baidu.com')
	#que=mp.Queue()
	#pools=mp.Pool(10)
	#pro=mp.Process(target= worker,args=(3,))
	#pro.start()
	#print "pid:",pro.pid,"name:",pro.name,"live?",pro.is_alive()
	#pools.map()
	
	
	#pro.join()
	try:
		Spider(url)
	except Exception,e:
		print "异常处理"
		traceback.print_exc()
		sys.exit(1)