import numpy as np
import matplotlib.pyplot as plt

def f(t):
    return np.exp(-t) * np.cos(2*np.pi*t)

t1 = np.arange(0.0, 5.0, 0.1)
t2 = np.arange(0.0, 5.0, 0.02)

plt.figure(1)
'''
    subplot(nrows, ncols, plot_number)
    分割当前的图片为 nrows * ncols 的矩形阵列，plot-number指当前是第几片，计数从左到右，从上到下
    1，如果都是小于10的横纵，可以连起来写，比如，subplot(223) ， 等效于subplot(2, 2, 3)， 即图片会分成2*2的4块， 当前指明第3块
    2. 默认是 111, 就只有1块
'''
plt.subplot(221)            # 2*2 块，当前是第1块
plt.plot(t1, f(t1), 'bo', t2, f(t2), 'k')
plt.xlabel("aaax")

plt.subplot(2,2,3, facecolor='y')   # 2*2块，当前是第3块
plt.plot(t2, np.cos(2*np.pi*t2), 'r--')

plt.subplot(2,4,4,facecolor='y')   # 2*4块，之前的2块，肯定和221的设计画布有重叠， 所以至少应该从3块开始
plt.plot(range(12))

plt.subplot(2,4,8,  facecolor='r', polar=True) 
plt.plot(range(12))
plt.xlabel("ploar-x")

plt.show()

'''
还有其它的关键字参数

facecolor:      背景颜色指定
    The background color of the subplot, which can be any valid color specifier. See matplotlib.colors for more information.
polar:
    A boolean flag indicating whether the subplot plot should be a polar projection. Defaults to False.
projection:
    A string giving the name of a custom projection to be used for the subplot. This projection must have been previously registered. See matplotlib.projections.
'''