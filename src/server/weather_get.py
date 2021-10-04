"""
Description:

Author: Cheng Shu
Date: 2021-10-04 15:45:40
LastEditTime: 2021-10-04 17:52:26
LastEditors: Cheng Shu
@Copyright © 2020 Cheng Shu
License: MIT License
"""

import hmac
import time
import base64
import sqlite3
import requests


DEBUG = True
API = 'https://api.seniverse.com/v3/weather/daily.json'
UNIT = 'c'  # 单位
LANGUAGE = 'en'  # 查询结果的返回语言
UID = b'PsMDXqxivaDAHqZrK'
KEY = b'Swyw29ZZNwflNOaGB'


db = "/home/shadow/weather.db"
citys = ["Nanjing", "Suzhou"]
wait_time = 5
total_time_gap = 4 * 3600 - len(citys) * wait_time
if DEBUG:
    db = "data/test.db"


def fetchWeather(location):
    t = int(time.time())
    message = f'ts={t}&ttl=300&uid=PsMDXqxivaDAHqZrK'
    h = hmac.new(KEY, message.encode(), digestmod='SHA1')
    sig = base64.b64encode(h.digest()).decode()
    result = requests.get(API, params={
       #  'key': '4r9bergjetiv1tsd',
        'location': location,
        'language': LANGUAGE,
        'unit': UNIT,
        'start': 0,
        'days': 3,
        'ts': str(t),
        'ttl': 300,
        'uid': UID,
        'sig': sig
    }, timeout=1)
    if result.status_code != 200:
        print(result.text)
        return None, None, None
    result_dict = result.json()
    if "results" not in result_dict or not len(result_dict["results"]):
        return None, None, None
    city = result_dict["results"][0]["location"]["name"]
    daily = result_dict["results"][0]["daily"]
    last_update_time = result_dict["results"][0]["last_update"]
    txt = "T;"
    for day in daily:
        txt += day["date"] + ';'
        txt += day["code_day"] + ';'
        txt += day["code_night"] + ';'
        txt += day["high"] + ';'
        txt += day["low"] + ';'
        txt += (day["rainfall"]) + ';'
        txt += day["precip"] + ';'
        txt += day["wind_direction"] + ';'
        txt += day["wind_speed"] + ';'
        txt += day["wind_scale"] + ';'
        txt += day["humidity"] + ';'

    return city, last_update_time, txt


def isNeedUpdate(updateTime, city):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(f'''
              SELECT UPDATETIME FROM WEATHER
              
              WHERE CITY = '{city}'
              AND   UPDATETIME LIKE '{updateTime[:10]}%'
              LIMIT 1
              ''')
    res = c.fetchall()
    conn.close()
    if res and res[0][0][:19] >= updateTime[:19]:
        return False
    return True


def init():
    conn = sqlite3.connect(db)
    c = conn.cursor()
#     c.execute('''DROP TABLE IF EXISTS WEATHER;''')
    c.execute('''CREATE TABLE IF NOT EXISTS WEATHER
              (ID            INTEGER      PRIMARY KEY,
              CITY           TEXT         NOT NULL,
              UPDATETIME     DATETIME     NOT NULL,
              TIMESTAMP      BIG INT,
              INFO           CHAR(255));''')
    print("Table created successfully")
    conn.commit()
    conn.close()


def insertWeather(city, updatetime, info):
    timestamp = int(time.mktime(time.strptime(
        updatetime[:19], '%Y-%m-%dT%H:%M:%S')))
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(f'''INSERT INTO WEATHER
              (ID, CITY,UPDATETIME ,TIMESTAMP , INFO)
              VALUES
              (null, '{city}', '{updatetime}', {timestamp}, '{info}')''')
    conn.commit()
    conn.close()


def updateWeather(city):
    t = time.asctime(time.localtime(time.time()))
    city, last_update_time, txt = fetchWeather(city)
    if isNeedUpdate(last_update_time, city):
        insertWeather(city, last_update_time, txt)
        print(f"[{t}] Update Success, City:{city}, time:{last_update_time}")
    else:
        print(f"[{t}] Abort Update, City:{city}, time:{last_update_time}")


def main():
    init()
    while 1:
        for city in citys:
            try:
                updateWeather(city)
                t = time.asctime(time.localtime(time.time()))
                print(f"[{t}] Sleep {wait_time} seconds before update next city")
                time.sleep(wait_time)
            except BaseException as e:
                print(e)
        
        print(f"[{t}] Sleep {total_time_gap} seconds before update next city")
        time.sleep(total_time_gap)

def test():
    t = int(time.time())
    message = f'ts={t}&ttl=300&uid=PsMDXqxivaDAHqZrK'
    key = b'Swyw29ZZNwflNOaGB'
    h = hmac.new(key, message.encode(), digestmod='SHA1')
    sig = base64.b64encode(h.digest()).decode()
    print(sig)


if __name__ == '__main__':
       main()
#     test()
