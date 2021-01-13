import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pandas import DataFrame

#全局替换字体，显示中文
plt.rcParams['font.sans-serif'] = ['SimHei']  # 步骤一（替换sans-serif字体）
plt.rcParams['axes.unicode_minus'] = False  # 步骤二（解决坐标轴负数的负号显示问题）

# 导入数据
data = pd.read_excel('货运.xls')
print(data)

# 绘图
'''
  Unnamed: 0     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug
0         铁路   31100   32100   41100   31100   71100   31100   41100   35100
1         公路   28690   28690   28690   28690   28690   28690   28690   28690
2         水运   33910   33910   33910   33910   33910   33910   33910   33910
3         民航  424000  424001  424002  424003  424004  424005  424006  424007

data.loc[0,:][1:]   从xls文件的第0行，第1列开始，读取整行的值

注意： 这里感觉 文件的 真正的第0行 “Jan， Feb, ....' 作为title没有计算在内 ！！
'''
plt.bar(np.arange(8),
        data.loc[0, :][1:],
        color='red',
        alpha=0.8,
        label='铁路',
        align='center')
plt.bar(np.arange(8),
        data.loc[1, :][1:],
        bottom=data.loc[0, :][1:],
        color='green',
        alpha=0.8,
        label='公路',
        align='center')
plt.bar(np.arange(8),
        data.loc[2, :][1:],
        bottom=data.loc[0, :][1:] + data.loc[1, :][1:],
        color='m',
        alpha=0.8,
        label='水运',
        align='center')
plt.bar(np.arange(8),
        data.loc[3, :][1:],
        bottom=data.loc[0, :][1:] + data.loc[1, :][1:] + data.loc[2, :][1:],
        color='black',
        alpha=0.8,
        label='民航',
        align='center')
# 添加轴标签
plt.xlabel('月份')
plt.ylabel('货物量(万吨)')
# 添加标题
plt.title('2017年各月份物流运输量')
# 添加刻度标签
print('data.columns[0:] = ', data.columns[0:])
print('data.index = ', data.index)
print('data.loc = ', data.loc[0:][0:])

plt.xticks(np.arange(8), data.columns[1:])  # title 行
# 设置Y轴的刻度范围
plt.ylim([0, 1000000])
# 为每个条形图添加数值标签
word_hight = 20000
for x_t, y_t in enumerate(data.loc[0, :][1:]):
    plt.text(x_t,
             y_t - word_hight,
             '%sW' % (round(data.loc[0, :][1:][x_t] / 10000, 2)),
             ha='center',
             color='white')
for x_g, y_g in enumerate(data.loc[0, :][1:] + data.loc[1, :][1:]):
    plt.text(x_g,
             y_g - word_hight,
             '%sW' % (round(data.loc[1, :][1:][x_g] / 10000, 2)),
             ha='center',
             color='white')
for x_s, y_s in enumerate(data.loc[0, :][1:] + data.loc[1, :][1:] +
                          data.loc[2, :][1:]):
    plt.text(x_s,
             y_s - word_hight,
             '%sW' % (round(data.loc[2, :][1:][x_s] / 10000, 2)),
             ha='center',
             color='white')
for x_k, y_k in enumerate(data.loc[0, :][1:] + data.loc[1, :][1:] +
                          data.loc[2, :][1:] + data.loc[3, :][1:]):
    plt.text(x_k,
             y_k - word_hight,
             '%sW' % (round(data.loc[3, :][1:][x_k] / 10000, 2)),
             ha='center',
             color='white')
# 显示图例
plt.legend(loc='upper center', ncol=4)
# 显示图形
plt.show()