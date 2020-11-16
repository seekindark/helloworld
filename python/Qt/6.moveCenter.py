#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
ZetCode PyQt5 tutorial 

This program centers a window 
on the screen. 

Author: Jan Bodnar
Website: zetcode.com 
Last edited: August 2017
"""

import sys
from PyQt5.QtWidgets import QWidget, QDesktopWidget, QApplication


class Example(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):               

        self.resize(550, 750)
        self.center1()

        self.setWindowTitle('Center')    
        self.show()


    def center1(self):

        qr = self.frameGeometry()                           # 获得主窗口的框架对象
        cp = QDesktopWidget().availableGeometry().center()  # 获得屏幕的中心位置
        qr.moveCenter(cp)                                   # 把 主窗口框架 的中心点放到屏幕的中心点  ；；；； 这里移动的是框架， 不是窗口本身
        self.move(qr.topLeft())                             # 通过move函数把主窗口 的左上角移动到 其框架的 左上角


if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())