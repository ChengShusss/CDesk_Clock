"""
Description:

Author: Cheng Shu
Date: 2021-10-04 15:45:40
LastEditTime: 2021-10-05 19:57:26
LastEditors: Cheng Shu
@Copyright © 2020 Cheng Shu
License: MIT License
"""

import hmac
import time
import base64
import sqlite3
import requests


DEBUG = False
API = 'https://api.seniverse.com/v3/weather/daily.json'
UNIT = 'c'  # 单位
LANGUAGE = 'en'  # 查询结果的返回语言
UID = b'PsMDXqxivaDAHqZrK'
KEY = b'Swyw29ZZNwflNOaGB'


db = "/home/shadow/weather.db"
citys = ["Nanjing", "Suzhou"]
wait_time = 5
# total_time_gap = 4 * 3600 - len(citys) * wait_time
total_time_gap = 25 - len(citys) * wait_time
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
    t = time.asctime(time.localtime(time.time()))
    print(f"[{t}] Fetch Weather Date: {result_dict}")
    if "results" not in result_dict or not len(result_dict["results"]):
        return None, None, None
    city = result_dict["results"][0]["location"]["name"]
    daily = result_dict["results"][0]["daily"]
    last_update_time = result_dict["results"][0]["last_update"]
    dailyReport = []
    for day in daily:
        txt = ""
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
        dailyReport.append(txt)
    print(dailyReport)

    return city, last_update_time, dailyReport


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
    c.execute('''DROP TABLE IF EXISTS WEATHER;''')
    c.execute('''CREATE TABLE IF NOT EXISTS WEATHER
              (ID            INTEGER      PRIMARY KEY,
              UPDATETIME     DATETIME     NOT NULL,
              CITY           TEXT         NOT NULL,
              DATE           TEXT,
              INFO           CHAR(255));''')
    print("Table created successfully")
    conn.commit()
    conn.close()

def updateCityWeather(city, info):
    t = time.asctime(time.localtime(time.time()))
    updatetime = time.strftime(
        "%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
    date = info[:10]
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(f"""
                SELECT ID FROM WEATHER
                WHERE DATE == '{date}'
                AND CITY == '{city}'
                """)
    r = c.fetchone()
    if r:
        id = r[0]
        print(f"[{t}]   UPDATE CITY={city}, INFO='{info}', UPDATETIME = '{updatetime}'")
        c.execute(f"""UPDATE WEATHER
                SET INFO = '{info}', UPDATETIME = '{updatetime}'
                WHERE ID == '{id}'
                """)
    else:
        print(f"[{t}]   INSERT (null, '{updatetime}', '{city}', '{date}', '{info}')")
        c.execute(f'''INSERT INTO WEATHER
                (ID, UPDATETIME, CITY, DATE , INFO)
                VALUES
                (null, '{updatetime}', '{city}', '{date}', '{info}')''')
    conn.commit()
    conn.close()


def updateWeather(city):
    t = time.asctime(time.localtime(time.time()))
    city, last_update_time, daily = fetchWeather(city)
    print(f"[{t}] Weather Info is Updated at: {last_update_time}")
    updatetime = time.strftime(
        "%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
    if isNeedUpdate(updatetime, city):
        for day in daily:
            updateCityWeather(city, day)
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
        t = time.asctime(time.localtime(time.time()))
        print(f"[{t}] Sleep {total_time_gap} seconds before update next city")
        time.sleep(total_time_gap)


def test():
    res = fetchWeather("Nanjing")
    print(res)

def checkout():
    conn = sqlite3.connect(db)
    c = conn.cursor()
    # c.execute('''SELECT * FROM WEATHER''')
    # r = c.fetchall()
    # for line in r:
    #     print(line)

    date = "2004"
    city = "Nanjing"
    print(date)
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(f"""
                SELECT ID FROM WEATHER
                WHERE DATE == '{date}'
                AND CITY == '{city}'
                """)
    r = c.fetchone()
    print(r)

    conn.close()

def checkall():
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute('''SELECT * FROM WEATHER''')
    r = c.fetchall()
    for line in r:
        print(line)

    conn.close()


if __name__ == '__main__':
    # main()
    test()
    # checkout()
    # checkall()
