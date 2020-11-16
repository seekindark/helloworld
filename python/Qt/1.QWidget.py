import sys
from PyQt5 import QtWidgets

if __name__ == '__main__':
    app = QtWidgets.QApplication(
        sys.argv)  # create main app, and receive the sytem arguments from user
    widget = QtWidgets.QWidget()  # create widget window
    widget.resize(600, 600)  # size 360px * 360px
    widget.move(600,
                60)  # set the position to the screen (300, 300), 坐标原点是屏幕的左上角
    widget.setWindowTitle("Hello, PyQt5!")
    widget.show()
    sys.exit(app.exec_())
