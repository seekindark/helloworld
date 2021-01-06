import sys

from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

class DockDemo(QMainWindow):
    def __init__(self):
        super(DockDemo, self).__init__()
        self.setWindowTitle("停靠控件")

        self.items = QDockWidget()
        self.listWidget = QListWidget()
        self.listWidget.addItem('I1')
        self.listWidget.addItem('I2')
        self.listWidget.addItem('I3')

        self.items.setWidget(self.listWidget)

        self.items.setFloating(False)       # 似乎，这里的 True or False 不影响 ???

        self.setCentralWidget(QLineEdit())
        self.addDockWidget(Qt.LeftDockWidgetArea, self.items)  # 添加一个dockWidget， 并指定停靠的位置在 LeftDockWidgetArea




if __name__ == "__main__":
    app = QApplication(sys.argv)
    main = DockDemo()
    main.show()
    sys.exit(app.exec_()) 