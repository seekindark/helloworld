import sys
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QApplication, QMainWindow, QDialog
from PyQt5.uic import loadUi

class CliDataBrowser(QDialog):
    def __init__(self,parent=None):
        super(CliDataBrowser,self).__init__(parent)
        loadUi("CliReportsUI/clidata_browser.ui",self)


class MainApplication(QMainWindow):
    def __init__(self,*args):
        super(MainApplication,self).__init__(*args)
        loadUi("CliReportsUI/clireportmain.ui",self)

    @pyqtSlot()
    def on_mnu_close_triggered(self):
        sys.exit()

    @pyqtSlot()
    def on_mnu_master_triggered(self):
        dataBrowser = CliDataBrowser(self) # <--- Need this to be a mdi subwindow
        dataBrowser.show()


app = QApplication(sys.argv)
mainWin = MainApplication()
mainWin.show()
sys.exit(app.exec_())