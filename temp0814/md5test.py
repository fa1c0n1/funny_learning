#!/usr/bin/env python3

import hashlib

while True:
    s = input('请输入字符串: \n')
    md5Str = hashlib.md5(s.encode('utf-8')).hexdigest()
    print('该字符串的md5值为：\n' + md5Str)
