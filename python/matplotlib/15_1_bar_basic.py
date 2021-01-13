import matplotlib.pyplot as plt

GDP = [12406.8, 13908.57, 9386.87, 9143.64]

# 绘图
# 
# bar函数指定了条形图的x轴、y轴值，设置x轴刻度标签为水平居中，条形图的填充色为铁蓝色，同时设置透明度为0.8；
plt.bar(range(4), GDP, align='center', color='steelblue', alpha=0.8)
# 添加轴标签
plt.ylabel('GDP')
# 添加标题
plt.title("四个直辖市GDP大比拼")
# 添加刻度标签
plt.xticks(range(4), ['北京市', '上海市', '天津市', '重庆市'])
# 设置Y轴的刻度范围
plt.ylim([5000, 15000])
# 为每个条形图添加数值标签    enumerate可以同时获得索引和值
for x, y in enumerate(GDP):
    plt.text(x, y + 100, '%s' % round(y, 1), ha='center')

plt.show()