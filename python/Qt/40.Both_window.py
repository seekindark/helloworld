import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow, QHBoxLayout, QDialog, \
     QVBoxLayout, QGridLayout, QGroupBox, QFormLayout, QRadioButton, QMenu, QMenuBar, \
    QTreeWidget, QTreeWidgetItem, QTextEdit, QFrame
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot, QRect


class MyMainWind(QMainWindow):
    def __init__(self):
        super(MyMainWind, self).__init__()
        self.title = "2 windows within one frame"
        self.left = 300  #the coordinate of window opened on screen
        self.top = 300
        self.width = 600
        self.height = 480
        self.initUI()

    def initUI(self):
        # set basic
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        # status bar
        self.status = self.statusBar()
        self.status.setSizeGripEnabled(True)  # 显示右下角
        self.status.showMessage("StatusBar Ready ...")

        # menu bar
        self.menu = QMenu()
        self.menu.addAction('&Open')
        self.menu.addAction('&Exit')
        self.menuBar = QMenuBar()
        fileAct = self.menuBar.addAction('&File')
        fileAct.setMenu(self.menu)
        self.menuBar.addAction('&About')
        self.setMenuBar(self.menuBar)

        # 2 widget
        self.tree = QTreeWidget()
        self.tree.setColumnCount(1)  #设置列数
        self.tree.setHeaderLabels(['QProfiler items'])  #设置树形控件头部的标题
        self.tree.setIndentation(20)        # 项目的缩进
                        #设置根节点
        Perfmon = QTreeWidgetItem()
        Perfmon.setText(0, 'Perfmon')
        perfmon_00 = QTreeWidgetItem()
        perfmon_00.setText(0, 'perfmon_00')
        Perfmon.addChild(perfmon_00)
        perfmon_01 = QTreeWidgetItem()
        perfmon_01.setText(0, 'perfmon_01')
        Perfmon.addChild(perfmon_01)
        perfmon_02 = QTreeWidgetItem()
        perfmon_02.setText(0, 'perfmon_02')
        Perfmon.addChild(perfmon_02)
        perfmon_03 = QTreeWidgetItem()
        perfmon_03.setText(0, 'perfmon_03')
        Perfmon.addChild(perfmon_03)
        self.tree.addTopLevelItem(Perfmon)

        # separator line
        self.line = QFrame()
        self.line.setGeometry(QRect(250, 340, 3, 61))
        self.line.setFrameShape(QFrame.VLine)
        self.line.setFrameShadow(QFrame.Sunken)

        self.editBox = QTextEdit('test')

        hLayout = QHBoxLayout()
        hLayout.addWidget(self.tree)
        hLayout.addWidget(self.line)
        hLayout.addWidget(self.editBox)
        hLayout.setStretch(0, 1)
        hLayout.setStretch(1, 3)
        # set layout
        widget = QWidget()
        widget.setLayout(hLayout)
        self.setCentralWidget(
            widget
        )  # 如果时 QMainWindow, layout 只能设置在centralWidget上, 所以是需要一个 widget 作为 centralWdiget
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    myMain = MyMainWind()
    sys.exit(app.exec_())