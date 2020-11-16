import matplotlib.pyplot as plt
import numpy as np
t = np.arange(0., 5., 0.2) 
'''

plt.plot(t,t, t, t**2)
fig = plt.gcf()   # get current figure   //保持当前图片
plt.title('first figure')
plt.show()   # 显示当前图片后，图片被情况，无法再显示，除非再次plot()调用生成
fig.show()     # 保持的图片可以再次显示,  但问题是， 显示的是瞬时的，然后窗口就关闭了。
'''
## 第2种方法
fig2 = plt.figure()     # 初始化
plt.plot(t,t, t, t**2)
plt.title('sencod figure')
plt.show()

# 如果直接调用 show， 也是很快消失 
plt.title('3rd figure')   ##  这里无用代码！！ 因为图像已经被show()清空了
fig2.show()






##  jupyter notebook 里面是这样显示
from IPython.display import display
display(fig2)  # Shows plot again
display(fig2)  # Shows plot again..