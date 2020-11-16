import matplotlib.pyplot as plt

plt.figure(1)                # 第一个图形
plt.subplot(231)             # 第一个图形的第一个子图
plt.plot([1, 2, 3])
plt.subplot(232, facecolor='g')             # 第一个图形的第二个子图
plt.plot([4, 5, 6])

fig = plt.figure(1)
ax1 = fig.add_subplot(234, facecolor='r')
ax1.set_xlim(0, 2)
ax1.set_ylim(1, 3)
ax2 = fig.add_subplot(235, facecolor='c', sharey=ax1) 
ax3 = fig.add_subplot(236, facecolor='b', sharey=ax1) 

# axes 得位置[0.65, 0.6, 0.1, 0.1]
# [left, bottom, width, height]  是按照整个画布得比例来安排的，比如这里 left = 0,65， 就是在从画布的原点 左移 65% (0.65)
# 之前整个画布的 grid规划就是 2*3的格子， 希望这个轴域放在第3块里面， left = 2/3, bottom =2/3
ax4 = fig.add_axes([0.66, 0.66, 0.3, 0.3], facecolor='y')  


plt.show()