#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
ZetCode PyQt5 tutorial 

This program shows a confirmation 
message box when we click on the close
button of the application window. 

Author: Jan Bodnar
Website: zetcode.com 
Last edited: August 2017

默认情况下，我们点击标题栏的×按钮，QWidget就会关闭。但是有时候，我们修改默认行为。
比如，如果我们打开的是一个文本编辑器，并且做了一些修改，我们就会想在关闭按钮的时候让用户进一步确认操作。
"""

import sys
from PyQt5.QtWidgets import QWidget, QMessageBox, QApplication



class Example(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):               
        self.setGeometry(300, 300, 250, 150)        
        self.setWindowTitle('Message box')    
        self.show()
    '''
    如果关闭QWidget，就会产生一个QCloseEvent，并且把它传入到closeEvent函数的event参数中。改变控件的默认行为，就是替换掉默认的事件处理。
    
    这里重载了 closeEvent（）函数
    '''
    def closeEvent(self, event):
        reply = QMessageBox.question(self, 'Message',
            "Are you sure to quit?",
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.No)

        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()        


if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())