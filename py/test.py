#!/usr/bin/python
# -*- coding: utf-8 -*-


import os
import cookielib
import urllib2
import sys
import re
import traceback
import requests

def find_content(content):
	'''regex'''
	#print content
	#<table class="table" style="margin-bottom:0;">
	mypage_info=re.findall(r'<td>(.*?)</td>',content,re.S)
	#print mypage_info
	return mypage_info
def find_morePage(content):
	'''��ҳ�����ҵ������url����'''
	more_page=re.findall(r'<a href="(.*?)">(.*?)</a>',content,re.S)
	print "ҳ��:%s"%more_page
	return more_page
	
def Spider(url):
	print "���֤����........"
	req=urllib2.Request(url)
	#ģ��Mozilla�������������
	req.add_header("user-agent","Mozilla/5.0")
	page = urllib2.urlopen(req)
	# page
	print page.getcode()
	content=page.read()
	finded=find_content(content)
	pages=find_morePage(content)
	#print finded.len()
	strr=map(str,finded)
	print strr
	for s in range(len(finded)/4):
		#if isinstance(finded[s],str):
		#	finded[s]=finded[s].decode('utf-8')
		s=s*4
		print finded[s].decode('utf-8'),finded[s+1].decode('utf-8'),finded[s+2].decode('utf-8'),finded[s+3].decode('utf-8')
	
	
	for w in pages:
		print w[0].decode('utf-8')
	print "���ȣ�%d" % len(page.read())



if __name__=='__main__':
	os.environ['http_proxy'] = ''
	print "start "
	url="http://m.sfzdq.diqibu.com/sfz/520302.html"
	try:
		Spider(url)
	except Exception,e:
		print "�쳣����"
		traceback.print_exc()
		sys.exit(1)