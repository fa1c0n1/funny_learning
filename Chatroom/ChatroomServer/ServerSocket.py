#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from socket import *
from enum import Enum
from DataBaseHelper import *
from hashlib import md5
from threading import Thread
import struct
import time

# 枚举类：用来表示数据包的请求目的
class EnumMsgType(Enum):
    ANONYMOUS = 1       # 表示用户上线，加入聊天室
    CHAT = 2            # 群聊
    ONE2ONE = 3         # 1VS1私聊
    REGISTER = 4        # 用户注册
    LOGIN = 5           # 用户登录
    ADDFRIEND = 6       # 添加好友
    SEARCHUSER = 7      # 搜索用户
    FILETRANS = 8       # 传文件
    MSGRECORD = 9       # 查询聊天记录
    UPDATEUSER = 10     # 更新聊天室用户列表
    UPDATEFRIEND = 11   # 更新好友列表


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

    # 开启子线程接收客户端连接
    def myAccept(self):
        # 创建线程 accept
        thrd = Thread(target=self.__acceptProc)
        thrd.start()

    # 循环接收连接请求
    #  每连接上一个客户端，都开启一个子线程去处理该客户端的请求
    def __acceptProc(self):
        while True:
            # accept返回的是个元组(套接字，客户端地址)
            socketClient, addrClient = self.socketServer.accept()
            recvThread = Thread(target=self.__recvProc, args=(socketClient,))
            recvThread.start()

    # 循环处理客户端的各种请求
    def __recvProc(self, s):
        while True:
            try:
                message = s.recv(CServerSocket.BUFSIZE + 10)
                # 消息类型
                type, = struct.unpack('i', message[:4])
                print('type=...... ', type)
                # 根据不同的消息类型，调用不同的处理函数
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

    # 用户上线
    def __chatForAnonymous(s, msg):
        len, = struct.unpack('i', msg[4:8])
        buf, = struct.unpack('%ds' % len, msg[8:8+len])
        name = buf.decode('gb2312').rstrip('\0')
        print(name + '加入聊天室')
        CServerSocket.dictClient[s] = name
        #回送数据包告诉用户当前聊天室在线用户都有哪些
        CServerSocket.updateUser(s, True, name)
        # 回送数据包告诉用户的好友都有哪些
        CServerSocket.updateFriend(s, name)

    # 群聊
    def __chatForChat(s, msg):
        # 将用户发的聊天内容，转发给其他在线用户
        for each in CServerSocket.dictClient:
            if each == s:
                continue
            each.send(msg)

    # 1VS1 私聊
    def __chatForOne2One(s, msg):
        dwLen, = struct.unpack('i', msg[4:8])
        fromName, = struct.unpack('64s', msg[8:72])
        toName, = struct.unpack('64s', msg[72:136])
        toName = toName.decode('gb2312').rstrip('\0')
        print('私聊对象：', toName)
        # 将用户发过来的聊天内容转发给该用户的私聊对象
        for each in CServerSocket.dictClient:
            if toName == CServerSocket.dictClient[each]:
                each.send(msg)
                break

        # 保存聊天记录
        msgFrom = CServerSocket.dictClient[s]
        msgTo = toName
        msgInfo, = struct.unpack('1024s', msg[136:136+1024])

        # 对聊天记录进行解密后在存入数据库
        msgEncryptByteArr = bytearray(msgInfo)
        for i in range(dwLen):
            msgEncryptByteArr[i] ^= 15

        msgInfo = msgEncryptByteArr.decode('gb2312').rstrip('\0')
        print('msgFrom=%s, msgTo=%s, msgInfo=%s' % (msgFrom, msgTo, msgInfo))
        #把消息添加到数据库，数据库设置外键了，所以只会添加双方都是已注册用户的聊天信息
        CServerSocket.conn.insert(
            "insert into msginfo(userfrom,userto,msgcontent) values(%s,%s,%s)",
            (msgFrom, msgTo, msgInfo))

    # 用户登录
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
            "select * from userinfo where username=%s", (nameStr,))

        # 获取该用户的IP地址
        clientIp = s.getpeername()[0]

        message_type = EnumMsgType.LOGIN
        message = ''
        if result == None or result[1] == 0:
            message = '请先注册!'.encode('gb2312')
        elif pwdEncrypt != result[0][0][1].decode('utf-8'):
            message = '用户名或密码错误'.encode('gb2312')
        else:
            if nameStr in CServerSocket.dictClient.values():
                for tmpSocket in CServerSocket.dictClient.keys():
                    if nameStr == CServerSocket.dictClient[tmpSocket]:
                        if clientIp == tmpSocket.getpeername()[0]:
                            message = '该账号已在本机登录!'.encode('gb2312')
                            break
                        else:
                            # 将原来已在其他IP上登录的相同用户名的用户踢下线
                            message = '当前账号已在别的地方登录,您已被踢下线!'.encode('gb2312')
                            msgSend = struct.pack('i2048s', message_type.value, message)
                            tmpSocket.send(msgSend)
                            tmpSocket.close()
                            CServerSocket.dictClient.pop(tmpSocket)
                            # 更新其他用户的在线用户列表
                            CServerSocket.updateUser(tmpSocket, False, nameStr)

                            # 当前IP的相同用户名的用户登录成功
                            message = '登录成功!'.encode('gb2312')
                            msgSend = struct.pack('i2048s', message_type.value, message)
                            s.send(msgSend)
                            return
            else:
                message = '登录成功!'.encode('gb2312')
        message_send = struct.pack('i2048s', message_type.value, message)
        s.send(message_send)

    # 用户注册
    def __chatForRegister(s, msg):
        md5Helper = md5()
        nameOrg, = struct.unpack('64s', msg[4:68])
        pwdOrg, = struct.unpack('64s', msg[68:132])
        nameStr = nameOrg.decode('gb2312').rstrip('\0')
        pwdStr = pwdOrg.decode('gb2312').rstrip('\0')
        # 用md5算法将密码加密后再保存到数据库
        md5Helper.update(pwdStr.encode())
        pwdEncrypt = md5Helper.hexdigest()

        message_type = EnumMsgType.REGISTER
        message = ''
        #构造查询语句
        result = CServerSocket.conn.query(
            "select username from userinfo where username=%s", (nameStr,))

        if result != None and result[1] == 1:
            message = '该用户名已被注册，请尝试其他用户名!'.encode('gb2312')
        else:
            #将新用户添加到用户表
            result = CServerSocket.conn.insert(
                "insert into userinfo(username,pwd) values(%s,%s)",
                (nameStr, pwdEncrypt))

            if result == None:
                message = '注册失败!'.encode('gb2312')
            else:
                message = '注册成功!'.encode('gb2312')
        message_send = struct.pack('i2048s', message_type.value, message)
        s.send(message_send)

    # 添加好友
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

    # 搜索用户
    def __chatForSearchUser(s, msg):
        name, = struct.unpack('64s', msg[4:68])
        name = name.decode('gb2312').rstrip('\0')

        # 构造查询语句
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

    # 查询聊天记录
    def __chatForGetMsgRecord(s, msg):
        name = CServerSocket.dictClient[s]
        # 构造查询语句
        result = CServerSocket.conn.query(
            "select * from msginfo where userfrom=%s or userto=%s", (name,name))

        message_type = EnumMsgType.MSGRECORD
        if result == None or result[1] == 0:
            pass
        else:
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

    BUFSIZE = 2048 +4    # 通信协议的数据包大小
    dictClient = {}      # 保存在线用户的字典，key表示用户的socket，value为用户名



