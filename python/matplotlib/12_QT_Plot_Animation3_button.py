'''
  pyqt5的UI中嵌入matplotlib图形并实时刷新

1) 导入模块比较简单，首先声明使用pyqt5，
2) 通过FigureCanvasQTAgg创建画布，可以将画布的图像显示到UI，相当于pyqt5的一个控件，后面的绘图就建立在这个画布上，
3) 然后把这个画布当中pyqt5的控件添加到pyqt5的UI上，
4) 其次要导入matplotlib.figure的Figure ，这里要注意的是matplotlib.figure中的Figure，不是matplotlib.pyplot模块中的Figure，要区分清楚。
'''
import sys
import numpy as np
import matplotlib
matplotlib.use("Qt5Agg")              # 声明使用pyqt5
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg # pyqt5的画布  通过FigureCanvasQTAgg创建画布，可以将画布的图像显示到UI
import matplotlib.pyplot as plt
from matplotlib.figure import Figure 
from matplotlib.animation import FuncAnimation
from PyQt5.QtWidgets import QApplication, QPushButton, QMainWindow, QVBoxLayout, QHBoxLayout,QWidget, QLabel
import PyQt5.QtCore as QtCore


'''
上面是使用axes.cla()的方式刷新图表，但是你有可能会遇到，你要展示的下一个图形于前面一次图表完全不同，包括画布背景色等都不同，那么用上面的axes.cla()只清理绘图区就不够了，
需要用得到清理画布figure.clf()，这个地方你要看清楚，清理绘图区方法是cla（），而清理画布是clf（）一字之差。
另外一个需要注意的地方就是，清理画布后之前画布上的绘图区axes也清理了，需要重新添加axes，
'''
class MyMatPlotAnimation(FigureCanvasQTAgg):
    """
    创建pyqt5画布，并简单设置样式
    创建一个画布类，继承上面导入的FigureCanvasQTAgg，通过Figure 创建画布，并且作为参数传递给父类FigureCanvasQTAgg（这里是关键一步！没有这一步后面一切都是白费，不会添加成功！），
    最后一步添加绘图区self.axes
    """
    def __init__(self, width=10, heigh=10, dpi=100):

        plt.rcParams['figure.facecolor'] = 'c'  # 设置窗体（画布）颜色
        plt.rcParams['axes.facecolor'] = 'y'    # 设置绘图区颜色

        '''
        # 创建一个Figure,该Figure为matplotlib下的Figure，不是matplotlib.pyplot下面的Figure
        # 这里还要注意，width, heigh可以直接调用参数，不能用self.width、self.heigh作为变量获取，因为self.width、self.heigh 在FigureCanvasQTAgg模块中已经使用，这里定义会造成覆盖
          （因为FigureCanvasQTAgg父类中导入了backend_qt5.py模块，而backend_qt5模块内部也使用了相同的变量名self.width和self.heigh）
        '''
        self.figs = Figure(figsize=(width, heigh), dpi=dpi)
        super(MyMatPlotAnimation, self).__init__(self.figs)   # 在父类种激活self.fig， 否则不能显示图像（就是在画板上放置画布）


    def mat_plot_drow_axes(self, t, s):
        """
        用清除画布刷新的方法绘图
        :return:
        """
        self.figs.clf() # 清除画布
        self.axes = self.figs.add_subplot(111) # 清理画布后必须重新添加绘图区
        self.axes.patch.set_facecolor("#01386a") # 设置ax区域背景颜色      blue !!
        self.axes.patch.set_alpha(0.5)           # 设置ax区域背景颜色透明度
        self.figs.patch.set_facecolor('#01386a') # 设置绘图区域颜色

        self.axes.spines['bottom'].set_color('r')   # 设置下边界颜色        ///就是 X 轴的 (横轴)的颜色
        self.axes.spines['top'].set_visible(False)  # 顶边界不可见
        self.axes.spines['right'].set_visible(False) # 右边界不可见

        #self.axes.spines['left'].set_visible(True) # 注意: 没有 左 边界的设置 ！！！ 
        #self.axes.spines['bottom'].set_visible(True) #注意: 没有 底 边界的设置 ！！！ 

        # 设置左、下边界在（0，0）处相交
        self.axes.spines['bottom'].set_position(('data', 0)) # 设置y轴线原点数据为 0
        self.axes.spines['left'].set_position(('data', 0)) # 设置x轴线原点数据为 0
        #self.axes.plot(t, s, 'o-r', linewidth=0.5)        # 指明线条的颜色，样式和宽度
        self.axes.plot(t, s)

        #
        #   重绘画  （只用下面一种方法调用，也能画出来，似乎效果一样。
        #             但可能在运行过程中闪退，所以还是加上比较安全 ---网上评论
        #
        self.figs.canvas.draw()         # 这里注意是画布重绘，self.figs.canvas
        self.figs.canvas.flush_events() # 画布刷新self.figs.canvas



#
# 把画布添加到pyqt5的UI中
#
class MainDialogImgBW(QMainWindow):
    def __init__(self):
        super(MainDialogImgBW, self).__init__()
        self.setWindowTitle("显示matplotlib")
        self.setObjectName("widget")
        self.resize(800, 600)
        self.label = QLabel(self)
        self.label.setText('what lable !')  # 这个LABEL 有什么用？？
        self.label.setGeometry(QtCore.QRect(0, 0, 800, 600))
        self.canvas = MyMatPlotAnimation(width=5, heigh=4, dpi=100)
        
        # self.plotcos()      # 绘制 cosin 函数图
        
        self.hboxlayout = QVBoxLayout(self.label)
        
        self.btn_start = QPushButton(self)
        self.btn_start.setText('draw')
        self.btn_start.clicked.connect(self.plotcos)

        #self.hboxlayout.addWidget(self.label)
        self.hboxlayout.addWidget(self.canvas)
        self.hboxlayout.addWidget(self.btn_start)

    def plotcos(self):
        t = np.arange(0.0, 5.0, 0.01)
        s = np.cos(2 * np.pi * t)
        #print(self.canvas.__dict__)
        #self.canvas.axes.plot(t, s)
        self.canvas.mat_plot_drow_axes(t, s)
        self.canvas.figs.suptitle("cosin") # 设置标题

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main = MainDialogImgBW()
    main.show()
    sys.exit(app.exec_())