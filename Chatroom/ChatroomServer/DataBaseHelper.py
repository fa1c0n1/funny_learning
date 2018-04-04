#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import mysql.connector

HOST = '192.168.3.235'

class CSqlForChat():
    def __init__(self):
        # 1. 连接数据库
        config = {'host': HOST,
                  'user': 'root',
                  'password': 'z0sa@#falconOS',
                  'port': '3306',
                  'database': '15pb_chatroom',
                  'charset': 'utf8'}
        print('正在连接数据库')
        self.conn = mysql.connector.connect(**config)
        print('数据库连接成功!')

    def __del__(self):
        self.conn.close()

    def query(self, szSql, param=None):
        cursor= self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = (cursor.fetchall(), cursor.rowcount)
            cursor.close()
            return result
        except:
            cursor.close()
            # 发生错误时，数据库进行回滚
            self.conn.rollback()
            return None

    def insert(self, szSql, param=None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        except:
            cursor.close()
            self.conn.rollback()
            return None



if __name__ == '__main__':
    sql = CSqlForChat()
    result = sql.query("select * from msginfo")

    for i in range(result[1]):
        # 第i条信息
        print(str(i) + ':')
        print(result[0][i][1].decode('utf-8'))
        print(result[0][i][2].decode('utf-8'))
        print(result[0][i][3].decode('utf-8'))

