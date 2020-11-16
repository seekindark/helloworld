# -*- coding: utf-8 -*-
'''
TODO:LQD
'''
import sys, os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FC
from PyQt5.QtWidgets import QApplication, QPushButton, QMainWindow, QVBoxLayout, QWidget


class QtDraw(QMainWindow):
  flag_btn_start = True
 
  def __init__(self):
    super(QtDraw, self).__init__()
    self.init_ui()
 
  def init_ui(self):
    self.resize(800, 600)
    self.setWindowTitle('PyQt5 Draw')
 
    # TODO:这里是结合的关键

    '''
    self.fig = plt.Figure(figsize=(8,6)) # size 8*6 英寸
    img_file_name = 'Error_Analysis.png'
    if os.path.isfile(img_file_name):
      image = plt.imread(img_file_name)
      plt.axis('off')
      plt.tight_layout()
      im = plt.imshow(image)
    else:
      print("image %s not existing !" % img_file_name)
    '''

    '''
    
    # 背景图片无法显示出来，因为图形加载在 plot 里，它的对象无法获取

    self.fig = matplotlib.figure.Figure(figsize=(8,6)) # size 8*6 英寸
    self.ax = self.fig.add_subplot(111)
    img_file_name = 'Error_Analysis.png'
    if os.path.isfile(img_file_name):
      image = matplotlib.image.imread(img_file_name)
      plt.axis('off')
      plt.tight_layout()
      plt.imshow(image)
      #plt.show()
    else:
      print("image %s not existing !" % img_file_name)
    '''

    self.fig = matplotlib.figure.Figure(figsize=(8,6)) # size 8*6 英寸
    self.ax = self.fig.add_subplot(111)      
    # 
    #  加载背景图片
    #
    image = matplotlib.image.imread('Error_Analysis.png')
    self.ax.axis('off')
    self.fig.set_tight_layout(True)
    self.ax.imshow(image)

    self.canvas = FC(self.fig)
    self.btn_start = QPushButton(self)
    self.btn_start.setText('draw')
    self.btn_start.clicked.connect(self.slot_btn_start)
 
    widget = QWidget()
    layout = QVBoxLayout()
    layout.addWidget(self.canvas)
    layout.addWidget(self.btn_start)
    widget.setLayout(layout)
    self.setCentralWidget(widget)


  def slot_btn_start(self):
    try:
      #self.ax.cla() # TODO:删除原图，让画布上只有新的一次的图; clear the current axes
      #self.ax.clear()    # clear the axes ；用
      self.ax.remove()   # if we use "ax = self.fig.add_subplot(111)" to create ax each time, then fome the 2nd time, an Warning will be catched !!
                         # 所以每次都需要 删除整个轴域
      #ax = self.ax      
      self.ax = self.fig.add_subplot(111)
      x = np.linspace(0, 100, 100)
      y = np.random.random(100)

      #self.ax.axis('scaled')   #  参数  ’scaled‘  似乎会把图形弄乱！！ ??? 还没搞懂 ？？
      
      # 修改轴相关参数
      #self.ax.set_autoscale_on(True)
      #self.ax.set_autoscaley_one(True)
      #self.ax .set_ylim(0.0, 10.0)
      #self.ax.set_xlim(0.0, 100.0)
      #self.ax.set_yscale("linear")
      
      self.ax.plot(x, y)
      prn_obj(self.ax)
      self.canvas.draw() # TODO:这里开始绘制

    except Exception as e:
      print(e)
 
def prn_obj(obj): 
  print('\n'.join(['%s:%s' % item for item in obj.__dict__.items()])) 

def ui_main():
  app = QApplication(sys.argv)
  w = QtDraw()
  w.show()
  sys.exit(app.exec_())
 
 
if __name__ == '__main__':
  ui_main()
