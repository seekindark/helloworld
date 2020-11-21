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

class MyMatPlotAnimation(FigureCanvasQTAgg):
    """
    创建pyqt5画布，并简单设置样式
    创建一个画布类，继承上面导入的FigureCanvasQTAgg，通过Figure 创建画布，并且作为参数传递给父类FigureCanvasQTAgg（这里是关键一步！没有这一步后面一切都是白费，不会添加成功！），
    最后一步添加绘图区self.axes
    """
    def __init__(self, width=10, heigh=10, dpi=100):

        plt.rcParams['figure.facecolor'] = 'r'  # 设置窗体颜色
        plt.rcParams['axes.facecolor'] = 'y'    # 设置绘图区颜色

        '''
        # 创建一个Figure,该Figure为matplotlib下的Figure，不是matplotlib.pyplot下面的Figure
        # 这里还要注意，width, heigh可以直接调用参数，不能用self.width、self.heigh作为变量获取，因为self.width、self.heigh 在FigureCanvasQTAgg模块中已经使用，这里定义会造成覆盖
          （因为FigureCanvasQTAgg父类中导入了backend_qt5.py模块，而backend_qt5模块内部也使用了相同的变量名self.width和self.heigh）
        '''
        self.figs = Figure(figsize=(width, heigh), dpi=dpi)
        super(MyMatPlotAnimation, self).__init__(self.figs)   # 在父类种激活self.fig， 否则不能显示图像（就是在画板上放置画布）
        #self.figs.patch.set_facecolor('#01386a')    # 设置画布（窗体）的背景色 等效于  plt.rcParams['figure.facecolor'] = 'b'
        self.axes = self.figs.add_subplot(111)      # 添加绘图区
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
        self.label.setGeometry(QtCore.QRect(0, 0, 800, 600))
        self.canvas = MyMatPlotAnimation(width=5, heigh=4, dpi=100)
        self.plotcos()      # 绘制 cosin 函数图
        self.hboxlayout = QHBoxLayout(self.label)
        self.hboxlayout.addWidget(self.canvas)

    def plotcos(self):
        t = np.arange(0.0, 5.0, 0.01)
        s = np.cos(2 * np.pi * t)
        #self.canvas.set_mat_func(t, s)
        #self.canvas.plot_tick()
        #print(self.canvas.__dict__)
        self.canvas.axes.plot(t, s)
        self.canvas.figs.suptitle("cosin") # 设置标题

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main = MainDialogImgBW()
    main.show()
    sys.exit(app.exec_())