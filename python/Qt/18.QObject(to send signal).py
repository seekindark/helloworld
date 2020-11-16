"""
PyQt5 tutorial 
 
In this example, we determine the event sender
object.
 
author: py40.com
last edited: 2017年3月
"""
 
 
import sys
from PyQt5.QtCore import pyqtSignal, QObject
from PyQt5.QtWidgets import QMainWindow, QApplication
 
 
class Communicate(QObject):
    sig_closeApp = pyqtSignal()     # 我们创建了一个名为closeApp的信号。这个信号会在按下鼠标时触发，它连接着QMainWindow的close()插槽
    
 
class Example(QMainWindow):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):      
 
        self.c = Communicate()
        self.c.sig_closeApp.connect(self.close)       # 连接QMainWindow的close()插槽
        
        self.setGeometry(300, 300, 290, 150)
        self.setWindowTitle('Emit signal')
        self.show()
        
        
    def mousePressEvent(self, event):       # 重载 事件函数
        self.statusBar()
       
        print("event: ", event)
        self.statusBar().showMessage(' was pressed')

        
        #self.c.sig_closeApp.emit()      # 发送信号 
        
        
if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())