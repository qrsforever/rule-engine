#!/usr/bin/python3
# -*- coding: utf-8 -*-

import threading
import socket
import time

class LogThread(threading.Thread):

    def __init__(self):
        super(LogThread, self).__init__()
        self.quit = False

    def start(self, addr, port, cb):
        self.addr = addr
        self.port = int(port)
        self.cb =cb
        super(LogThread, self).start()
        time.sleep(1)

    def stop(self):
        self.quit = True

    def run(self):
        bufsize = 1024
        udp_server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udp_server.bind((self.addr, self.port))
        udp_server.settimeout(2)
        while not self.quit:
            try:
                data, addr = udp_server.recvfrom(bufsize)
                data = data.decode(encoding='utf-8')
                self.cb(data)
            except socket.timeout:
                continue
            except Exception as e:
                print("error:", e)
                break
        print("LogThread quit");
        udp_server.close()
