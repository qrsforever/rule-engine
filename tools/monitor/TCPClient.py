#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket
import time
import sys


class TCPClient(object):

    def __init__(self, bufsize):
        self.connectted = False
        self.buf_size = bufsize

    def connect(self, srv_addr, srv_port):
        self.srv_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.srv_socket.settimeout(3);
        try:
            self.srv_socket.connect((srv_addr, int(srv_port)))
            result = self.srv_socket.recv(8).decode('utf-8')
        except Exception as e:
            print("connect error:", e)
            return False
        print("connect result: ", result)
        if result != 'success':
            return False
        self.connectted = True
        return True

    def close(self):
        if self.connectted:
            try:
                self.srv_socket.send(b'quit')
                time.sleep(1)
                self.srv_socket.close()
            finally:
                sys.exit(0)

    def command(self, cmd, *args):
        if not self.connectted:
            return None

        cmdstr = cmd;
        for arg in args:
            cmdstr += ';'
            cmdstr += arg

        try:
            self.srv_socket.send(cmdstr.encode())
            res = self.srv_socket.recv(self.buf_size).decode('utf-8')
            if res == "-1":
                return ""
            print("command[%s]:%s" % (cmdstr, res))
            return res;
        except socket.timeout:
            return ""
        except BrokenPipeError :
            print("socket error!")
            sys.exit(0)
        else:
            return ""
