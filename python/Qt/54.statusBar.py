#导入模块
from PyQt5.QtWidgets import QApplication,QWidget,QProgressBar,QPushButton
from PyQt5.QtCore import QBasicTimer #定时器
from PyQt5.QtGui import QIcon  #设置窗口图标
import sys
#创建一个窗口类
class Example(QWidget):
    def __init__(self):
        #解决多重继承问题
        super().__init__()
        self.initUI()
        #信号槽
        self.btn.clicked.connect(self.doAction)
    #UI布局
    def initUI(self):
        #构建一个滚动条
        self.pbar=QProgressBar(self)
        #从左上角30-50点开始，创建一个200*25的界面
        self.pbar.setGeometry(30,50,200,25)
        #构建一个按钮
        self.btn=QPushButton('运行',self)
        self.btn.move(50,90)
        #构建定时器
        self.timer=QBasicTimer()
        self.step=0  #设置进度条的初始值
        self.setGeometry(300,300,280,170)
        #设置窗口标题
        self.setWindowTitle('Jaes进度条')
        #设置窗口图标
        self.setWindowIcon(QIcon('tit.ico'))
        #显示界面
        self.show()
    def timerEvent(self, *args, **kwargs):
        if self.step>=100: #停止进度条
            self.timer.stop()
            self.btn.setText('完成')
            return
        self.step=self.step+1
        #重置、刷新进度条
        self.pbar.setValue(self.step)
    #槽函数
    def doAction(self):
        #判断进度条是否处于激活且执行状态
        if self.timer.isActive():
            self.timer.stop()
            self.btn.setText('运行')
        else:
            #立即激活进度条并开始执行
            self.timer.start(100,self)
            self.btn.setText('停止')
if __name__=='__main__':
    #构建一个QT应用对象
    app=QApplication(sys.argv)
    ex=Example()
    #开始运行程序
    sys.exit(app.exec_())