#!/usr/bin/env python
# -*- coding:utf-8 -*-

# system imports
import sys

# pyqt imports
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt, QCoreApplication


class MainWindow(QtWidgets.QWidget):
    def __init__(self):
        
        QtWidgets.QWidget.__init__(self)
        #super(MainWindow, self).__init__(self)

        # 初始化position
        self.m_DragPosition = self.pos()
        #screen = QtGui.QDesktopWidget().screenGeometry()
        #self.resize(screen.width(), screen.height())
        # self.resize(1280,800)
        #self.setWindowFlags(Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint)
        self.setStyleSheet("background-color:#2C3E50;")

        # 按钮一
        qbtn_one = QtWidgets.QPushButton(u"开始测试", self)
        qbtn_one.setGeometry(400, 360, 120, 80)
        qbtn_one.setStyleSheet("QPushButton{background-color:#16A085;border:none;color:#ffffff;font-size:20px;}"
                               "QPushButton:hover{background-color:#333333;}")

        qbtn_close = QtWidgets.QPushButton(u"关闭此窗口", self)
        qbtn_close.setGeometry(600, 360, 120, 80)
        qbtn_close.setStyleSheet("QPushButton{background-color:#D35400;border:none;color:#ffffff;font-size:20px;}"
                                 "QPushButton:hover{background-color:#333333;}")

        #self.showFullScreen()  # 全屏显示必须放在所有组件画完以后执行
        # 注册事件
        qbtn_one.clicked.connect(self.showFullScreen)
        #qbtn_close.clicked.connect(QCoreApplication.instance().quit)  # 退出程序
        qbtn_close.clicked.connect(self.close)                          # 退出窗口


    def keyPressEvent(self, event):
        if event.key() == QtCore.Qt.Key_A:
            self.showFullScreen()
        if event.key() == QtCore.Qt.Key_Escape:
            self.showNormal()


if __name__ == "__main__":

    mapp = QtWidgets.QApplication(sys.argv)
    mw = MainWindow()
    mw.show()
    sys.exit(mapp.exec_())
