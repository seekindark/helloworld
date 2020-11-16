import matplotlib.pyplot as plt
plt.figure(1)                # 第一个图形
plt.subplot(211)             # 第一个图形的第一个子图
plt.plot([1, 2, 3])
plt.subplot(212)             # 第一个图形的第二个子图
plt.plot([4, 5, 6])

'''
利用图像编号，创建多个图像。

这里在使用 plt.show()后， 可以显示多幅图像

'''

plt.figure(2)                # 第二个图形
plt.plot([4, 5, 6])          # 默认创建 subplot(111)


plt.figure(1)                # 当前是图形 1，subplot(212)
plt.subplot(211)             # 将第一个图形的 subplot(211) 设为当前子图
plt.title('Easy as 1, 2, 3') # 子图 211 的标题， 修改当前子图的标题！！

plt.figure(2)
#plt.close()                 # 如果 关闭图形2，后面show()就无法显示了
#plt.clf()                    # 图形2 被清除，会显示空白的画布
plt.cla()                    # 图形2 的轴域内容 被清除， 但 轴（坐标轴还在）

plt.show()
