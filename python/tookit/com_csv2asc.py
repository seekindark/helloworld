'''
Created on 2017-3-21
@author: soundslow
'''
#coding=utf-8
#python默认使用ASKII码保存文件，所以在文件开头需要声明保存编码的格式（例：#coding=utf-8）
import sys
from sys import argv
import csv

argc = '1.asc'
script, filename = argc, argv
fw = open(argc, 'w')
# string为：从ncols到NODATA_value这几行是一样的，因为数据源是一致的
string = ("ncols  4800\nnrows  1550\nxllcorner 284687.500000\nyllcorner 2412912.500000\ncellsize 25.000000\nNODATA_value -9999.000000\n")
fw.write(string)
fr = open(argc, 'r')
with open('result_50_1.csv') as f:
    reader = csv.reader(f)
    '''
    第一个for循环是为了提取出一个中间介质row_num作为行数的表示，
    由于从表格中提取的数据（行号）并不是数字类型，
    不能直接使用（1 == f_rows[2]进行判断，所以直接使用自身的类型。）
    （补充：应该是字符串类型）
    '''
    for i, f_rows_temp in enumerate(reader):
        if(i == 0):
            continue
        row_num = f_rows_temp[2]
        print(row_num)
        break

    # 遍历写入数据，遍历行数（即csv文件第三列f_rows[2]）
    for i, f_rows in enumerate(reader):
        if(i == 0):
            continue
        if(f_rows[2] != row_num):   # 判断是否换行
            fw.write('\n')
        row_num = f_rows[2]
        fw.write(f_rows[1])
        fw.write(' ')
        sys.stdout.flush()  # 强制IO刷新，写入文件数据
        print(f_rows[1])    # 两个打印是为了便于观察数据运行情况
        print(f_rows)

fw.close()
fr.close()