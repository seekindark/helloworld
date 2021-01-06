import sys
from PyQt5 import QtWidgets, QtCore


class Window(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setFixedSize(QtCore.QSize(100, 100))
        #self.setWindowFlag(QtCore.Qt.Tool)  # 去任务栏, 两个窗口都需要调用这个或都不调用这句， 否则就会导致 子窗口关闭的同时，主窗口也关闭了。

        self.button = QtWidgets.QPushButton(self)
        self.button.clicked.connect(self.show_diary)
        self.show()

    @staticmethod
    def show_diary():
        diary_window = MyDialog()
        diary_window.exec_()

    def closeEvent(self, e):
        sys.exit(0)


class MyDialog(QtWidgets.QDialog):
    def __init__(self):
        super().__init__()
        self.label = QtWidgets.QLabel(self)
        # self.setWindowFlag(QtCore.Qt.Tool)    # 去任务栏, 两个窗口都需要调用这个或都不调用这句， 否则就会导致 子窗口关闭的同时，主窗口也关闭了。
        self.label.setText('This is dialog')
        self.show()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    ex = Window()
    sys.exit(app.exec_())