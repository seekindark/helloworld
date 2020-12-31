from ui_mainwind import Ui_MainWindow

from PyQt5.QtWidgets import QApplication, QDialog, QMainWindow
import sys


class Dg_code(Ui_MainWindow, QMainWindow):     #注意：  一定要是先继承 Ui_Dialog, 否则就回出错！！
    def __init__(self):
        super(Dg_code, self).__init__()
        self.setupUi(self)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    print('start ...')
    dg = Dg_code()
    print('end ...')
    sys.exit(app.exec_())