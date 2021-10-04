"""
Description: 

Author: Cheng Shu
Date: 2021-10-03 22:12:52
LastEditTime: 2021-10-03 22:29:32
LastEditors: Cheng Shu
@Copyright © 2020 Cheng Shu
License: MIT License
"""
import requests
from utils.const_value import API, KEY, UNIT, LANGUAGE
from utils.helper import getLocation


def fetchWeather(location):
    result = requests.get(API, params={
        'key': KEY,
        'location': location,
        'language': LANGUAGE,
        'unit': UNIT,
        'start': 0,
        'days': 3
    }, timeout=1)
    print(result.encoding)
    return result.text


if __name__ == '__main__':
    location = getLocation()
    result = fetchWeather("Qinhuai")
    print(result.encode('utf-8'))