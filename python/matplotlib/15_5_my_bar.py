import matplotlib.pyplot as plt
import numpy as np

#全局替换字体
plt.rcParams['font.sans-serif'] = ['SimHei'] # 步骤一（替换sans-serif字体）
plt.rcParams['axes.unicode_minus'] = False  # 步骤二（解决坐标轴负数的负号显示问题）


# 构建数据
perfmon_0 = [0,0,0,0,490,
            11,0,0,0,0,
            0,0,0,0,1,
            0,110,0,0,0]
perfmon_1 = [0,0,0,0,1490,
            11,0,0,0,0,
            0,0,0,0,1000,
            0,210,0,0,0]
labels = ['<64 cyc', '<128 cyc', '<1 us', '<3 us', '<6 us' ,
            '<13 us', '<27 us', '<54 us', '<109 us', '<218us',
            '<436 us', '<873us', '<1 ms', '<3ms', '<6 ms',
            '<13 ms', '<27 ms', '<55 ms', '<111 ms', '<28 s']
bar_width = 0.5

# 绘图
plt.bar(np.arange(20), perfmon_0, label='perfmon0', color='steelblue', alpha=0.8, width=bar_width, align='center')
plt.bar(np.arange(20)+bar_width, perfmon_1, label='perfmon1', color='magenta', alpha=0.8, width=bar_width, align='center')
# 添加轴标签
plt.xlabel('Execution Time')
plt.ylabel('Times')
# 添加标题
plt.title('QProfile:perfmon')
# 添加刻度标签
plt.xticks(np.arange(20), labels)
# 设置Y轴的刻度范围
plt.ylim([0, 1500])
# 为每个条形图添加数值标签
for x0,y0 in enumerate(perfmon_0):
    if y0 > 0:
        plt.text(x0, y0+10, '%s' %y0, ha='center')
for x0,y0 in enumerate(perfmon_1):
    if y0 > 0:
        plt.text(x0+bar_width, y0+10, '%s' %y0, ha='center')
#for x2017,y2017 in enumerate(Y2017):
#    plt.text(x2017+bar_width, y2017+100, '%s' %y2017, ha='center')
# 显示图例
plt.legend()
# 显示图形
plt.show()