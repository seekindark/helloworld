from PyQt5.QtWidgets import (QApplication,QWidget,QPushButton,QTextBrowser,QGridLayout)
from PyQt5.QtCore import Qt,pyqtSignal
import sys

class Example(QWidget):
    # 声明无参数的信号
    signal1 = pyqtSignal()
    # 声明带一个int类型参数的信号
    signal2 = pyqtSignal(int)
    # 声明带int和str类型参数的信号
    signal3 = pyqtSignal(int, str)
    # 声明带一个列表类型参数的信号
    signal4 = pyqtSignal(list)
    # 声明带一个字典类型参数的信号
    signal5 = pyqtSignal(dict)
    # 声明一个多重载版本的信号，包括带int和str类型参数的信号和带str类型参数的信号
    signal6 = pyqtSignal([int, str], [str])

    def __init__(self):
        super(Example, self).__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(300,300,450,380)
        self.setWindowTitle("自定义信号与槽练习")
        gridLayout = QGridLayout()
        self.btn1 = QPushButton("无参数信号")
        self.btn2 = QPushButton("int信号")
        self.btn3 = QPushButton("int和str信号")
        self.btn4 = QPushButton("list信号")
        self.btn5 = QPushButton("dict信号")
        self.btn6 = QPushButton("多重载信号[int,str],[str]")
        self.textBrowser = QTextBrowser()
        gridLayout.addWidget(self.textBrowser,0,0,4,12)
        gridLayout.addWidget(self.btn1,5,0,1,2)
        gridLayout.addWidget(self.btn2,5,2,1,2)
        gridLayout.addWidget(self.btn3,5,4,1,2)
        gridLayout.addWidget(self.btn4,5,6,1,2)
        gridLayout.addWidget(self.btn5,5,8,1,2)
        gridLayout.addWidget(self.btn6,5,10,1,2)
        self.setLayout(gridLayout)
        self.mytxt = ""
        #空信号
        self.btn1.clicked.connect(self.mySignal1)
        self.signal1.connect(self.mySlotFunc1)
        #int数字信号
        self.btn2.clicked.connect(self.mySignal2)
        self.signal2.connect(self.mySlotFunc2)
        #int和str（数字和字符串）信号
        self.btn3.clicked.connect(self.mySignal3)
        self.signal3.connect(self.mySlotFunc3)
        #list列表信号
        self.btn4.clicked.connect(self.mySignal4)
        self.signal4.connect(self.mySlotFunc4)
        #dict字典信号
        self.btn5.clicked.connect(self.mySignal5)
        self.signal5.connect(self.mySlotFunc5)
        # 多重载信号
        self.btn6.pressed.connect(self.mySignal6_1)
        self.signal6[int, str].connect(self.mySlotFunc6_1)

        self.btn6.released.connect(self.mySignal6_2)
        self.signal6[str].connect(self.mySlotFunc6_2)


    def mySignal1(self):
        self.signal1.emit()
    def mySlotFunc1(self):
        self.mytxt = "无参数的信号"
        self.textBrowser.setText(self.mytxt)

    def mySignal2(self):
        self.signal2.emit(123456)
    def mySlotFunc2(self,val):
        self.mytxt = "Int信号:" + str(val)
        self.textBrowser.setText(self.mytxt)

    def mySignal3(self):
        self.signal3.emit(123456,"这是我的电话号码：")
    def mySlotFunc3(self, val,text):
        self.textBrowser.setText(text+str(val))

    def mySignal4(self):
        self.signal4.emit([1,5,9,0,0,0,0,1,2,3,4])
    def mySlotFunc4(self,li):
        print(li)
        self.mytxt = 'List信号：'
        for i in li:
            self.mytxt += str(i)
            # print(self.mytxt)
        self.textBrowser.setText(self.mytxt)

    def mySignal5(self):
        self.signal5.emit({"phone":90001111,
                           "addr":["浙江","宁波"]})
    def mySlotFunc5(self,mydict):
        self.mytxt = 'Dict 信号：'
        self.mytxt += str(mydict["phone"])+str(mydict["addr"][0]) + ', ' + str(mydict["addr"][1])
        self.textBrowser.setText(self.mytxt)

    def mySignal6_1(self):
        # self.signal6.emit(123456,'phone6')    # 2种方式效果一样
        self.signal6[int, str].emit(123456,'phone6')
    def mySlotFunc6_1(self, num, st):
        self.mytxt += 'Sigal 6_1 with Str+Int: ' + st + ', ' + str(num)
        self.textBrowser.setText(self.mytxt)

    def mySignal6_2(self):
        self.signal6[str].emit('string test...')
    def mySlotFunc6_2(self, st):
        self.mytxt = 'Sigal 6_2 with Int: ' + st
        self.textBrowser.setText(self.mytxt)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    ex.show()
    sys.exit(app.exec_())