# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *


class FirstWindow(QWidget):

    close_signal = pyqtSignal()

    def __init__(self, parent=None):
        # super这个用法是调用父类的构造函数
        # parent=None表示默认没有父Widget，如果指定父亲Widget，则调用之
        super(FirstWindow, self).__init__(parent)
        self.resize(480, 600)
        self.setWindowTitle("First Window")
        self.btn = QToolButton(self)
        self.btn.setText("click")

    def closeEvent(self, event):            #重写 closeEvent 事件
        print("Close Event !")
        self.close_signal.emit()
        self.close()


class SecondWindow(QWidget):
    def __init__(self, parent=None):
        super(SecondWindow, self).__init__(parent)
        self.resize(320, 400)
        self.setStyleSheet("background: black")
        self.setWindowTitle('Second Window')

    def handle_click(self):
        print('second window: handle_click()')
        if not self.isVisible():
            self.show()

    def handle_close(self):
        print('second window: handle_close()')
        self.close()


if __name__ == "__main__":
    App = QApplication(sys.argv)
    ex = FirstWindow()
    s = SecondWindow()
    ex.btn.clicked.connect(s.handle_click)
    ex.btn.clicked.connect(ex.hide)
    ex.close_signal.connect(ex.close)    #
    ex.show()
    print('first window show !')
    sys.exit(App.exec_())