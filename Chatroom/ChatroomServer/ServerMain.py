#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from ServerSocket import *

# HOST = '127.0.0.1'
HOST = '192.168.3.235'
PORT = 5796

if __name__ == '__main__':
    server = CServerSocket(HOST, PORT)
    server.myAccept()
