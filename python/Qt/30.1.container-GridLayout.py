import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow, QHBoxLayout, QDialog, \
     QVBoxLayout, QGridLayout, QGroupBox, QFormLayout
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot

'''
 QGroupBox为构建分组框提供了支持。
 分组框通常带有一个边框和一个标题栏，作为容器部件来使用，在其中可以布置各种窗口部件。
 布局时可用作一组控件的容器，
 但是需要注意的是，
     - 内部必须使用布局控件（如QBoxLayout）进行布局
'''



class App(QDialog):
    def __init__(self):
        super(App, self).__init__()
        self.title = "QDialog"
        self.left = 300  #the coordinate of window opened on screen
        self.top = 300
        self.width = 200
        self.height = 150
        self.initUI()

    def initUI(self):
        print("init...")
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.createGridLayout()
        windowlayout = QVBoxLayout()  #QVBoxLayout create the block to slice into grid
        windowlayout.addWidget(self.horizontalGroupBox)
        # initiate the pre-defined slice grip
        self.setLayout(windowlayout)
        self.show()

    def createGridLayout(self):
        print('createGridLayout ...')
        # create a group by QGroupBox
        # and slice them into grid by QgridLayout
        self.horizontalGroupBox = QGroupBox("Grid")
        layout = QGridLayout()
        layout.setColumnStretch(1, 4)    # 第1列 (from 0 counting ...)  缩放比例为4
        layout.setColumnStretch(2, 2)       # 第 2 列，缩放比例为 2，

        button = QPushButton("Press for Test")
        button.clicked.connect(self.feedBack)
        layout.addWidget(button, 0, 0)
        layout.addWidget(QPushButton("2"), 0, 1)
        layout.addWidget(QPushButton("3"), 0, 2)
        layout.addWidget(QPushButton("4"), 1, 0)
        layout.addWidget(QPushButton("5"), 1, 1)
        layout.addWidget(QPushButton("6"), 1, 2)
        layout.addWidget(QPushButton("7"), 2, 0)
        layout.addWidget(QPushButton("8"), 2, 1)
        layout.addWidget(QPushButton("9"), 2, 2)
        layout.addWidget(QLineEdit('input:'), 3, 0, 1, 3)      # columnSpan = 3, 占3列

        self.horizontalGroupBox.setLayout(layout)  # add gridLayout into groupBox

    @pyqtSlot()
    def feedBack(self):
        print("the first button works")


if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = App()
    sys.exit(app.exec_())