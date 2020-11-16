import matplotlib.pyplot as plt
import numpy as np

# create some data to use for the plot
dt = 0.001
t = np.arange(0.0, 10.0, dt)
r = np.exp(-t[:1000]/0.05)               # impulse response
x = np.random.randn(len(t))
s = np.convolve(x, r)[:len(x)]*dt  # colored noise

# the main axes is subplot(111) by default
plt.plot(t, s)
plt.axis([0, 1, 1.1*np.amin(s), 2*np.amax(s)])    #xmin, xmax, ymin, ymax
plt.xlabel('time (s)')
plt.ylabel('current (nA)')
plt.title('Gaussian colored noise')


# this is an inset axes over the main axes  ;;在主图上放一个小插图，使用axes()
#  Add an axes (坐标轴) to the figure, and 位置在 rect
#  1. axes() by itself creates a default full subplot(111) window axis.
#  2. axes(rect, facecolor='w') where rect = [left, bottom, width, height] in normalized (0, 1) units. facecolor is the background color for the axis, default white.
#  2. axes(h) where h is an axes instance makes h the current axis. An Axes instance is returned.
#
a = plt.axes([0.65, 0.6, .2, .2], facecolor='y')   # 注意： 这里的rect 位置是从整个画布的 左下角 (0,0）开始计算的，而不是从坐标轴的原点开始计算的。
n, bins, patches = plt.hist(s, 100, density=True)     # hist(x, bins), bins 应该是细化的步数， 数值越大，越是细化
print(s)
plt.title('Probability')
#plt.xticks([])
#plt.yticks([])

# this is another inset axes over the main axes
a = plt.axes([0.2, 0.6, .2, .2], facecolor='c')
plt.plot(t[:len(r)], r)
plt.title('Impulse response')
plt.xlim(0, 0.2)
plt.xticks(np.linspace(0.0, 1.0, 1))
plt.yticks([])

plt.show()