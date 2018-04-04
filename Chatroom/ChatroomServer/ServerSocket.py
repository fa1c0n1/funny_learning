#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from socket import *
from enum import Enum
from DataBaseHelper import *
from hashlib import md5
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
    UPDATEFRIEND = 11


class CServerSocket():
    conn = CSqlForChat()
    def __init__(self, ip, port):
        addr = (ip, port)
        # 初始化 socket
        print('正在启动服务器...')
        self.socketServer = socket(AF_INET, SOCK_STREAM)
        # 允许地址复用
        self.socketServer.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
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
                print('type=...... ', type)
                CServerSocket.dictFun[type](s, message)
            except Exception as e:
                print('exception= ', e)
                name = CServerSocket.dictClient.get(s)
                if name == None:
                    return
                s.close()
                name = CServerSocket.dictClient.pop(s)
                print('客户端退出: ' + name)
                CServerSocket.updateUser(s, False, name)
                return

    def __chatForAnonymous(s, msg):
        len, = struct.unpack('i', msg[4:8])
        buf, = struct.unpack('%ds' % len, msg[8:8+len])
        name = buf.decode('gb2312').rstrip('\0')
        print(name + '加入聊天室')
        CServerSocket.dictClient[s] = name
        CServerSocket.updateUser(s, True, name)
        CServerSocket.updateFriend(s, name)

    def __chatForChat(s, msg):
        for each in CServerSocket.dictClient:
            if each == s:
                continue
            each.send(msg)

    def __chatForOne2One(s, msg):
        fromName, = struct.unpack('64s', msg[4:68])
        toName, = struct.unpack('64s', msg[68:132])
        toName = toName.decode('gb2312').rstrip('\0')
        print('私聊对象：', toName)
        for each in CServerSocket.dictClient:
            if toName == CServerSocket.dictClient[each]:
                each.send(msg)
                break

        # 保存聊天记录
        msgFrom = CServerSocket.dictClient[s]
        msgTo = toName
        msgInfo, = struct.unpack('1024s', msg[132:132+1024])
        msgInfo = msgInfo.decode('gb2312').rstrip('\0')

        print('msgFrom=%s, msgTo=%s, msgInfo=%s' % (msgFrom, msgTo, msgInfo))
        #把消息添加到数据库，数据库设置外键了，所以只会添加双方都是已注册用户的聊天信息
        CServerSocket.conn.insert(
            "insert into msginfo(userfrom,userto,msgcontent) values(%s,%s,%s)",
            (msgFrom, msgTo, msgInfo))

    def __chatForLogin(s, msg):
        md5Helper = md5()
        nameOrg, = struct.unpack('64s', msg[4:68])
        pwdOrg, = struct.unpack('64s', msg[68:132])
        nameStr = nameOrg.decode('gb2312').rstrip('\0')
        pwdStr = pwdOrg.decode('gb2312').rstrip('\0')
        md5Helper.update(pwdStr.encode())
        pwdEncrypt = md5Helper.hexdigest()
        # 构造查询语句
        result = CServerSocket.conn.query(
            "select * from userinfo where username=%s and pwd=%s",
            (nameStr, pwdEncrypt))

        message_type = EnumMsgType.LOGIN
        message = ''
        if result == None or result[1] == 0:
            message = '登录失败!'.encode('gb2312')
        else:
            message = '登录成功!'.encode('gb2312')
        message_send = struct.pack('i2048s', message_type.value, message)
        s.send(message_send)

    def __chatForRegister(s, msg):
        md5Helper = md5()
        nameOrg, = struct.unpack('64s', msg[4:68])
        pwdOrg, = struct.unpack('64s', msg[68:132])
        nameStr = nameOrg.decode('gb2312').rstrip('\0')
        pwdStr = pwdOrg.decode('gb2312').rstrip('\0')
        md5Helper.update(pwdStr.encode())
        pwdEncrypt = md5Helper.hexdigest()

        #将新用户添加到用户表
        result = CServerSocket.conn.insert(
            "insert into userinfo(username,pwd) values(%s,%s)",
            (nameStr, pwdEncrypt))

        message_type = EnumMsgType.REGISTER
        message = ''
        if result == None:
            message = '注册失败!'.encode('gb2312')
        else:
            message = '注册成功!'.encode('gb2312')
        message_send = struct.pack('i2048s', message_type.value, message)
        s.send(message_send)

    def __chatForAddFriend(s, msg):
        # 获取要添加好友的双方姓名
        name, = struct.unpack('64s', msg[5:69])
        frd, = struct.unpack('64s', msg[69:133])
        name = name.decode('gb2312').rstrip('\0')
        frd = frd.decode('gb2312').rstrip('\0')
        # 构造查询语句
        result = CServerSocket.conn.query(
            "select * from userfriend where (name=%s and friend=%s) "
            "or (name=%s and friend=%s)", (name, frd, frd, name)
        )

        print('result=', str(result))

        message_type = EnumMsgType.ADDFRIEND
        message = ''
        bRetAdd = False
        if result == None or result[1] == 0:
            retCnt = CServerSocket.conn.insert(
                "insert into userfriend(name, friend) values(%s,%s)", (name, frd))
            if retCnt == None:
                message = ('添加 \'' + frd + '\' 失败!').encode('gb2312')
            else:
                bRetAdd = True
                message = ('添加 \'' + frd + '\' 成功!').encode('gb2312')
        else:
            message = ('你和 \'' + frd + ' \'已经是好友啦!').encode('gb2312')

        message_send = struct.pack('i?64s64s1919s', message_type.value,
                        bRetAdd, name.encode('gb2312'), frd.encode('gb2312'),
                        message)

        # 如果添加好友成功，需要通知该好友
        if bRetAdd == True:
            for clientSocket, clientName in CServerSocket.dictClient.items():
                if clientName == frd:
                    message = ('添加 \'' + name + '\' 成功!').encode('gb2312')
                    msgSend = struct.pack('i?64s64s1919s', message_type.value,
                                bRetAdd, frd.encode('gb2312'), name.encode('gb2312'),
                                message)
                    clientSocket.send(msgSend)
                    break

        # 如果添加好友成功，需要通知发起添加请求的用户
        s.send(message_send)

    def __chatForSearchUser(s, msg):
        name, = struct.unpack('64s', msg[4:68])
        name = name.decode('gb2312').rstrip('\0')
        result = CServerSocket.conn.query(
            "select username from userinfo where username=%s", (name,))

        message_type = EnumMsgType.SEARCHUSER
        message = ''
        if result == None or result[1] == 0:
            message = '查无此人!'.encode('gb2312')
        else:
            if name in CServerSocket.dictClient.values():
                message = '用户在线，双击列表内用户名 1VS1 聊天!'.encode('gb2312')
            else:
                message = '用户不在线!'.encode('gb2312')
        message_send = struct.pack('i2048s', message_type.value, message)
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
            msgFrom = result[0][i][1].decode('utf-8')
            msgTo = result[0][i][2].decode('utf-8')
            msgContent = result[0][i][3].decode('utf-8')
            # 把每条信息分别打包发送给客户端s
            msgFrom = msgFrom.encode('gb2312')
            msgTo = msgTo.encode('gb2312')
            msgContent = msgContent.encode('gb2312')
            msgSend = struct.pack('i64s64s1920s', message_type.value,
                                  msgFrom, msgTo, msgContent)
            s.send(msgSend)
            time.sleep(0.1)

        # 最后发个 END 过去，告诉客户端聊天记录已全部发完
        msgFrom = '~~~end~~~'.encode('gb2312')
        msgSend = struct.pack('i2048s', message_type.value, msgFrom)
        s.send(msgSend)

    @staticmethod
    def updateUser(s, bAdd, name):
        try:
            # 先给所有客户端的用户列表里添加新用户
            message_type = EnumMsgType.UPDATEUSER
            message = name.encode('gb2312')
            message_len = len(message)
            message_send = struct.pack('i?i2043s', message_type.value, bAdd,
                                       message_len, message)
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                each.send(message_send)
            if bAdd == False:
                return
            print('dictClient: ', CServerSocket.dictClient)
            # 再给新用户的在线列表添加之前登录的用户名
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                print('更新 ', CServerSocket.dictClient[each])
                message = CServerSocket.dictClient[each].encode('gb2312')
                message_len = len(message)
                message_send = struct.pack('i?i2043s', message_type.value, bAdd,
                                           message_len, message)
                s.send(message_send)
                time.sleep(0.1)
        except:
            return

    @staticmethod
    def updateFriend(s, name):
        friendNameList = []
        message_type = EnumMsgType.UPDATEFRIEND
        result = CServerSocket.conn.query(
            "select friend from userfriend where name=%s", (name,))

        if result != None and result[1] > 0:
            for i in range(result[1]):
                friendName = result[0][i][0].decode('utf-8') #mysql字符集是utf-8
                # 将好友保存到列表
                friendNameList.append(friendName)

        result = CServerSocket.conn.query(
            "select name from userfriend where friend=%s", (name,))

        if result != None and result[1] > 0:
            for i in range(result[1]):
                friendName = result[0][i][0].decode('utf-8') #mysql字符集是utf-8
                # 将好友保存到列表
                friendNameList.append(friendName)

        # 让客户端s更新好友列表
        for friendName in friendNameList:
            msgSend = struct.pack('i?2047s',
                        message_type.value, True, friendName.encode('gb2312'))
            s.send(msgSend)
            time.sleep(0.1)


    # 类变量
    dictFun = {
        1: __chatForAnonymous,
        2: __chatForChat,
        3: __chatForOne2One,
        4: __chatForRegister,
        5: __chatForLogin,
        6: __chatForAddFriend,
        7: __chatForSearchUser,
        9: __chatForGetMsgRecord
    }

    BUFSIZE = 2048 +4
    dictClient = {}



