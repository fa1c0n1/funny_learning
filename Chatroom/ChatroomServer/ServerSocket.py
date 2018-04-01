#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from socket import *
from enum import Enum
from DataBaseHelper import *

from threading import Thread
import struct
import time

class EnumMsgType(Enum):
    ANONYMOUS = 1
    CHAT = 2
    ONE2ONE = 3
    REGISTER = 4
    LOGIN = 5
    ADDFRIEND = 6
    SEARCHUSER = 7
    FILETRANS = 8
    MSGRECORD = 9
    UPDATEUSER = 10

class CServerSocket():
    conn = CSqlForChat()
    def __init__(self, ip, port):
        addr = (ip, port)
        # 初始化 socket
        print('正在启动服务器...')
        self.socketServer = socket(AF_INET, SOCK_STREAM)
        self.socketServer.bind(addr)
        self.socketServer.listen()
        print('服务器启动成功，等待客户端连接...')

    def myAccept(self):
        # 创建线程 accept
        thrd = Thread(target=self.__acceptProc)
        thrd.start()

        # accept 的回调函数
    def __acceptProc(self):
        while True:
            # accept返回的是个元组(套接字，客户端地址)
            socketClient, addrClient = self.socketServer.accept()
            recvThread = Thread(target=self.__recvProc, args=(socketClient,))
            recvThread.start()

        # 接收消息线程的回调
    def __recvProc(self, s):
        while True:
            try:
                message = s.recv(CServerSocket.BUFSIZE + 10)
                # 消息类型
                type, = struct.unpack('i', message[:4])
                CServerSocket.dictFun[type](s, message)
            except:
                name = CServerSocket.dictClient.get(s)
                if name == None:
                    return
                s.close()
                name = CServerSocket.dictClient.pop(s)
                print('客户端退出: ' + name)
                CServerSocket.updateUser(s, False, name)
                return


    def __chatForAnonymous(s, msg):
        len, = struct.unpack('L', msg[4:8])
        buf, = struct.unpack('%ds' % len, msg[8:8+len])
        name = buf.decode('gb2312')
        print(name + '加入聊天室')
        CServerSocket.dictClient[s] = name.rstrip('\0')
        for each in CServerSocket.dictClient:
            each.send(msg)
        # 通知给每个客户端更新在线用户列表
        CServerSocket.updateUser(s, True, name)

    def __chatForChat(s, msg):
        len, = struct.unpack('L', msg[4:8])
        buf, = struct.unpack('%ds' % len, msg[8:8+len])
        message_recv = buf.decode('gb2312')
        print(message_recv)
        for each in CServerSocket.dictClient:
            if each == s:
                continue
            each.send(msg)

    def __chatForOne2One(s, msg):
        name, = struct.unpack('50s', msg[4:54])
        name = name.decode('gb2312').rstrip('\0')
        for each in CServerSocket.dictClient:
            if name == CServerSocket.dictClient[each]:
                name = struct.pack('50s', CServerSocket.dictClient[s].encode('gb2312'))
                each.send(msg[:4] + name + msg[54:])
                break

        # 是否保存聊天记录
        msgFrom = CServerSocket.dictClient[s]
        msgTo, = struct.unpack('50s', msg[4:54])
        msgTo = msgTo.decode('gb2312').rstrip('\0')
        msgInfo = struct.unpack('1024s', msg[54:54+1024])
        msgInfo = msgInfo.decode('gb2312').rstrip('\0')

        #把消息添加到数据库，数据库设置外键了，所以只会添加双方都是已注册用户的聊天信息
        CServerSocket.conn.insert(
            "insert into msgInfo(userfrom,userto,msgcontent) values(%s,%s,%s)",
            (msgFrom, msgTo, msgInfo))

    def __chatForLogin(s, msg):
        name, = struct.unpack('50s', msg[4:54])
        pwd, = struct.unpack('50s', msg[54:104])
        name = name.decode('gb2312').rstrip('\0')
        pwd = pwd.decode('gb2312').rstrip('\0')
        # 构造查询语句
        result = CServerSocket.conn.query(
            "select * from userinfo where name=%s and pwd=%s", (name, pwd))

        message_type = EnumMsgType.LOGIN
        message_len = 64
        message = ''
        if result == None or result[1] == 0:
            message = '登录失败!'.encode('gb2312')
        else:
            message = '登录成功!'.encode('gb2312')
        message_send = struct.pack('l2048s', message_type.value, message)
        s.send(message_send)

    def __chatForRegister(s, msg):
        name, = struct.unpack('64s', msg[4:68])
        pwd, = struct.unpack('64s', msg[68:132])
        print('b_name=%s, b_pwd=%s' % (name, pwd))
        name = name.decode('gb2312').rstrip('\0')
        print('name=%s' % name)
        pwd = pwd.decode('gb2312').rstrip('\0')
        print('name=%s, pwd=%s' % (name, pwd))

        #构造查询语句
        result = CServerSocket.conn.insert(
            "insert into userinfo(username,pwd) values(%s,%s)", (name, pwd))

        message_type = EnumMsgType.REGISTER
        message = ''
        if result == None:
            message = '注册失败!'.encode('gb2312')
        else:
            message = '注册成功!'.encode('gb2312')
        print('b_message=%s' % message)
        message_send = struct.pack('i2048s', message_type.value, message)
        s.send(message_send)

    def __chatForAddFriend(s, msg):
        # 获取要添加好友的双方姓名
        name, = struct.unpack('50s', msg[4:54])
        frd, = struct.unpack('50s', msg[54:104])
        name = name.decode('gb2312').rstrip('\0')
        frd = frd.decode('gb2312').rstrip('\0')
        # 构造查询语句
        result = CServerSocket.conn.insert(
            "insert into userfriend(name, friend) values(%s,%s)", (name, frd))

        message_type = EnumMsgType.ADDFRIEND
        message_len = 64
        message = ''
        if result == None:
            message = '添加好友失败!'.encode('gb2312')
        else:
            message = '添加好友成功!'.encode('gb2312')
        message_send = struct.pack('l2048s', message_type.value, message)
        s.send(message_send)


    def __chatForSearchUser(s, msg):
        name, = struct.unpack('50s', msg[4:54])
        name = name.decode('gb2312').rstrip('\0')
        result = CServerSocket.conn.query(
            "select username from userinfo where name=%s", (name,))
        message_type = EnumMsgType.SEARCHUSER
        message_len = 64
        message = ''
        if result == None or result[1] == 0:
            message = '查无此人!'.encode('gb2312')
        else:
            if name in CServerSocket.dictClient.values():
                message = '用户在线，双击列表内用户名 1VS1 聊天!'.encode('gb2312')
            else:
                message = '用户不在线!'.encode('gb2312')
        message_send = struct.pack('l2048s', message_type.value, message)
        s.send(message_send)

    def __chatForGetMsgRecord(s, msg):
        name = CServerSocket.dictClient[s]
        # 查询所有信息
        result = CServerSocket.conn.query(
            "select * from msginfo where userfrom=%s or userto=%s", (name,name))

        if result == None or result[1] == 0:
            return

        message_type = EnumMsgType.MSGRECORD
        for i in range(result[1]):
            # 第i条信息
            print(str(i) + ':')
            print('from:' + result[0][i][0])
            print('to:' + result[0][i][1])
            print('content:' + result[0][i][2])
            # 把每条信息分别打包发送给客户端s
            msgFrom = result[0][i][0].encode('gb2312')
            msgTo = result[0][i][1].encode('gb2312')
            msgContent = result[0][i][2].encode('gb2312')
            msgSend = struct.pack('l50s50s1948s', message_type.value,
                                  msgFrom, msgTo, msgContent)
            s.send(msgSend)

        # 最后发个 END 过去，告诉客户端聊天记录已全部发完
        msgFrom = '~~~end~~~'.encode('gb2312')
        msgSend = struct.pack('l2048s', message_type.value, msgFrom)
        s.send(msgSend)

    @staticmethod
    def updateUser(s, bAdd, name):
        try:
            # 先给所有客户端的用户列表里添加新用户
            message_type = EnumMsgType.UPDATEUSER
            message = name.encode('gb2312')
            message_len = len(message)
            message_send = struct.pack('lll2040s', message_type.value, bAdd,
                                       message_len, message)
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                each.send(message_send)
            if bAdd == False:
                return
            # 再给新用户的在线列表添加之前登录的用户名
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                message = CServerSocket.dictClient[each].encode('gb2312')
                message_len = len(message)
                message_send = struct.pack('lll2040s', message_type.value, bAdd,
                                           message_len, message)
                s.send(message_send)
        except:
            return

    # 类变量
    dictFun = {
        1: __chatForAnonymous,
        2: __chatForChat,
        3: __chatForOne2One,
        4: __chatForRegister,
        5: __chatForLogin,
        6: __chatForAddFriend,
        7: __chatForSearchUser,
        8: __chatForGetMsgRecord
    }

    BUFSIZE = 2048 +4
    dictClient = {}



