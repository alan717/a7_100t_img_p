#!/usr/bin/python
# -*- coding: utf-8 -*-
 
import cookielib
import urllib2
import sys
 
url = "http://www.baidu.com"
response1 = urllib2.urlopen(url)
print "��һ�ַ���"
#��ȡ״̬�룬200��ʾ�ɹ�
print response1.getcode()
#��ȡ��ҳ���ݵĳ���
print len(response1.read())
 
print "�ڶ��ַ���"
 
request = urllib2.Request(url)
#ģ��Mozilla�������������
request.add_header("user-agent","Mozilla/5.0")
response2 = urllib2.urlopen(request)
print response2.getcode()
print len(response2.read())
 
print "�����ַ���"
cookie = cookielib.CookieJar()
#����urllib2����cookie������
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))
urllib2.install_opener(opener)
response3 = urllib2.urlopen(url)
print response3.getcode()
print len(response3.read())
print cookie