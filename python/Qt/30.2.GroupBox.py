import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow, QHBoxLayout, QDialog, \
     QVBoxLayout, QGridLayout, QGroupBox, QFormLayout, QRadioButton
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot


class App(QDialog):
    def __init__(self):
        super(App, self).__init__()
        self.title = "QDialog with QGroupBox"
        self.left = 300  #the coordinate of window opened on screen
        self.top = 300
        self.width = 600
        self.height = 480
        self.initUI()

    def initUI(self):
        print("init...")
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.groupBox = QGroupBox("Exclusive optons", self)     # 只有设置了 parent， 该控件才能显示
        #self.groupBox.setParent(self)

        layout = QVBoxLayout()
        radio = QRadioButton('last button')
        radio.setChecked(True)
        layout.addWidget(radio)
        layout.addWidget(QRadioButton('&RadioButton 1'))
        layout.addWidget(QRadioButton('R&adioButton 2'))
        layout.addWidget(QRadioButton('Ra&dioButton 3'))
        layout.setStretch(1, 1)
        self.groupBox.setLayout(layout)
    
        self.show()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = App()
    sys.exit(app.exec_())