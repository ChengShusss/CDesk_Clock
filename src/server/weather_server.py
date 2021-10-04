"""
Description: Provide Weather data API

Author: Cheng Shu
Date: 2021-10-04 20:23:03
LastEditTime: 2021-10-04 21:57:37
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
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(f'''
              SELECT INFO FROM WEATHER
              
              WHERE CITY = '{city}'
              ORDER BY TIMESTAMP
              LIMIT 1
              ''')
    res = c.fetchone()
    conn.close()
    date = time.strftime("%Y-%m-%d", time.localtime())
    if res and res[0][2:12] == date:
        return res[0]
    return "F"

if __name__ == '__main__':
   app.run()