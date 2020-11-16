import sys
from PyQt5 import QtWidgets, QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow
import testWind








if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    mwind = QMainWindow()
    mwind.setToolTip("This a test toolTip for mainwindow")

    ui = testWind.TestWind()
    ui.setupUi(mwind)

    mwind.show()
    sys.exit(app.exec_())