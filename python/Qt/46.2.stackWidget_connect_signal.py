# -*- coding: utf-8 -*-
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
import sys


# QTextCodec.setCodecForTr(QTextCodec.codecForName("utf8"))
class Stacked(QDialog):
    def __init__(self, parent=None):
        super(Stacked, self).__init__(parent)
        self.setWindowTitle(self.tr("StackedWidget"))

        leftlist = QListWidget(self)
        leftlist.insertItem(0, 'window1')
        leftlist.insertItem(1, 'window2')
        leftlist.insertItem(2, 'window3')

        label1 = QLabel('windowTest1\n11111111 ')
        label2 = QLabel('windowTest2\n22222222 ')
        label3 = QLabel('windowTest3\n33333333 ')

        stack = QStackedWidget(self)
        stack.addWidget(label1)
        stack.addWidget(label2)
        stack.addWidget(label3)
        self.stack = stack

        mainLayout = QHBoxLayout(self)
        mainLayout.setContentsMargins(5, 5, 5, 5)  # 对话框边距设为5 Margin 边距  5px
        mainLayout.setSpacing(5)  # 内部控件间距为5 Spacing间距  5px
        mainLayout.addWidget(leftlist)
        mainLayout.addWidget(stack, 0, Qt.AlignHCenter)
        mainLayout.setStretchFactor(leftlist, 1)
        mainLayout.setStretchFactor(stack, 3)  # 设定了list与stack比例为1:3。
        # connect(leftlist, SIGNAL('currentRowChanged(int)'), stack, SLOT('setCurrentIndex(int)'))  # 这种PYQT4的 连接 信号槽的方式 在PYQT5里不支持了
        self.leftlist = leftlist
        self.leftlist.currentRowChanged.connect(self.display)


    def display(self, idx):
        self.stack.setCurrentIndex(idx)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = Stacked()
    main.show()
    app.exec_()