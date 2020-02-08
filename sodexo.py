#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import requests, time
from pprint import pprint

date=time.strftime("%Y/%m/%d")

icon=""
try: 
	r = requests.get('http://www.sodexo.fi/ruokalistat/output/daily_json/16363/'+date+'/fi')
	json=r.json()

	courses=json['courses']
	koti=courses[0]
	title=koti['title_fi']


	print(icon + " " + title)
except (IndexError):
	print(icon + " vklp :^)")

