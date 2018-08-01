#!/usr/bin/python3
# -*- coding: utf-8 -*-

from configparser import ConfigParser
from WindowGUI import WindowGUI

if __name__ == "__main__":
    cf = ConfigParser()
    cf.read('config.ini')
    app = WindowGUI(1024, 888,
            cf.get('HB', 'ServerAddress'),
            cf.get('HB', 'ServerPort'),
            cf.get('HB', 'LogUDPAddress'),
            cf.get('HB', 'LogUDPPort'))
