"""
Description: Provide Weather data API

Author: Cheng Shu
Date: 2021-10-04 20:23:03
LastEditTime: 2021-10-05 13:37:14
LastEditors: Cheng Shu
@Copyright © 2020 Cheng Shu
License: MIT License
"""

import time
from flask import Flask
import sqlite3


DEBUG = False

db = "/home/shadow/weather.db"
if DEBUG:
    db = "data/test.db"

app = Flask(__name__)


@app.route('/')
def hello_world():
   return 'Hello World'

@app.route('/<city>')
def getWeatherInfo(city):
    today = time.strftime("%Y-%m-%d", time.localtime(time.time()))
    tomorrow = time.strftime("%Y-%m-%d", time.localtime(time.time() + 3600 * 24))
    print(today)
    print(tomorrow)
    conn = sqlite3.connect(db)
    res = "T;"
    c = conn.cursor()
    c.execute(f'''
              SELECT INFO FROM WEATHER
              
              WHERE CITY == '{city}'
              AND DATE == '{today}'
              LIMIT 1
              ''')
    r = c.fetchone()
    if not r:
        return "F;"
    res += r[0]
    c.execute(f'''
              SELECT INFO FROM WEATHER
              
              WHERE CITY = '{city}'
              AND DATE = '{tomorrow}'
              LIMIT 1
              ''')
    r = c.fetchone()
    if not r:
        return "F"
    res += r[0]
    conn.close()
    return res


if __name__ == '__main__':
   app.run()