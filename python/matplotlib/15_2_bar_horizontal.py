import matplotlib.pyplot as plt
# 显示中文
from matplotlib.font_manager import FontProperties
font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=14)

price = [39.5,39.9,45.4,38.9,331.34]
# 绘图
'''
代码解读
水平条形图的绘制与垂直条形图的绘制步骤一致，只是调用了barh函数来完成。需要注意的是，条形图的数值标签设置有一些不
一样，需要将标签垂直居中显示，使用va参数即可。
'''
plt.barh(range(5), price, align = 'center',color='steelblue', alpha = 0.8)
# 添加轴标签
plt.xlabel('价格', fontproperties=font)
# 添加标题
plt.title('不同平台书的最低价比较', fontproperties=font)
# 添加刻度标签
plt.yticks(range(5),['亚马逊','当当网','中国图书网','京东','天猫'], fontproperties=font)
# 设置Y轴的刻度范围
plt.xlim([32,470])
# 为每个条形图添加数值标签
for x,y in enumerate(price):
    plt.text(y+0.1, x, f'{y}', va='center', fontproperties=font)
# 显示图形
plt.show()

