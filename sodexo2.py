#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import requests
from pprint import pprint

r = requests.get('http://www.sodexo.fi/ruokalistat/output/daily_json/16363/2017/10/26/fi')
json=r.json()

courses=json['courses']
koti=courses[0]
title=koti['title_fi']

#print('%s %s, %i °C' % (icon, main, temp))
icon = ""
print(title)


