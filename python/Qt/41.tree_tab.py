import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow, QHBoxLayout, QDialog, \
     QVBoxLayout, QGridLayout, QGroupBox, QFormLayout, QRadioButton, QMenu, QMenuBar, \
    QTreeWidget, QTreeWidgetItem, QTextEdit, QFrame, QTabWidget, QLabel, QCheckBox
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot, QRect

'''
参考资料
https://www.jb51.net/article/181684.htm
'''


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
        self.tree.setIndentation(20)  # 项目的缩进
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

        self.tab = self.createTabWidget()

        hLayout = QHBoxLayout()
        hLayout.addWidget(self.tree)
        hLayout.addWidget(self.line)
        hLayout.addWidget(self.tab)

        #hLayout.setStretch(0, 1)

        # set layout
        widget = QWidget()
        widget.setLayout(hLayout)
        self.setCentralWidget(
            widget
        )  # 如果时 QMainWindow, layout 只能设置在centralWidget上, 所以是需要一个 widget 作为 centralWdiget

        self.show()

    def createTabWidget(self):
        tab = QTabWidget()

        tab.tabCloseRequested[int].connect(self.closeTab)    # 带参数 的 pyqtsignal

        tab1 = QWidget()
        tab2 = QWidget()
        tab3 = QWidget()
        tab4 = QTextEdit()

        tab.addTab(tab1, 'tab 1')
        tab.addTab(tab2, 'tab 2')
        tab.addTab(tab3, 'tab 3')
        tab.addTab(tab4, 'tab Edit')
        tab.setTabsClosable(True)
        tab.setMovable(True)
        tab.setDocumentMode(True)

        self.tab1_ui(tab, tab1)
        self.tab2_ui(tab, tab2)
        self.tab3_ui(tab, tab3)

        return tab

    def tab1_ui(self, tab, wg):
        #表单布局
        layout = QFormLayout()
        #添加姓名，地址的单行文本输入框
        layout.addRow('姓名', QLineEdit())
        layout.addRow('地址', QLineEdit())
        layout.addRow('地址', QLineEdit())
        layout.addRow('地址', QLineEdit())
        layout.addWidget(QPushButton('button'))
        layout.addRow(QLabel('xxx'), QPushButton('yyy'))
        #设置选项卡的小标题与布局方式
        wg.setLayout(layout)
        tab.setTabText(0, '联系方式')

       
    def tab2_ui(self, tab, wg):
        #zhu表单布局，次水平布局
        layout=QFormLayout()
        sex=QHBoxLayout()
    
        #水平布局添加单选按钮
        sex.addWidget(QRadioButton('男'))
        sex.addWidget(QRadioButton('女'))
    
        #表单布局添加控件
        layout.addRow(QLabel('性别'),sex)
        layout.addRow('生日',QLineEdit())
    
        #设置标题与布局
        wg.setLayout(layout)
        tab.setTabText(1,'个人详细信息')


    def tab3_ui(self, tab, wg):
        #水平布局
        layout=QHBoxLayout()
    
        #添加控件到布局中
        layout.addWidget(QLabel('科目'))
        layout.addWidget(QCheckBox('物理'))
        layout.addWidget(QCheckBox('高数'))
    
        #设置小标题与布局方式
        wg.setLayout(layout)
        tab.setTabText(2,'教育程度')

    def closeTab(self, idx):
        print('remove tab :', idx)
        self.tab.removeTab(idx)



if __name__ == '__main__':
    app = QApplication(sys.argv)
    myMain = MyMainWind()
    sys.exit(app.exec_())