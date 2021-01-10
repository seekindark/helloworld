import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QFont, QIcon
from PyQt5.QtWidgets import (QApplication, QMainWindow, QMenuBar, QMenu,
                             QAction, QActionGroup, QToolBar, QStyle,
                             QToolButton, QMdiArea, QMdiSubWindow,
                             QPlainTextEdit, QFileDialog, QMessageBox,
                             QListWidget, QListWidgetItem, QStackedWidget,
                             QTabWidget, QTreeWidget, QTreeWidgetItem, QDockWidget,
                             QStatusBar, QWidget)
# import resource_rc
import random
import matplotlib
# Make sure that we are using QT5
matplotlib.use('Qt5Agg')

from numpy import arange, sin, pi
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

perfmon_x = arange(1, 20, 1)
# define a customized QTreeWidgetItem
class myQTreeWidgetItem(QTreeWidgetItem):
    def __init__(self, y):
        super(myQTreeWidgetItem, self).__init__()
        self.y = y
        self.x = perfmon_x

class MyMplCanvas(FigureCanvas):
    """Ultimately, this is a QWidget (as well as a FigureCanvasAgg, etc.)."""

    # 宽度和长度的单位都是 inch， 英寸， DPI 的含义是 像素/每英寸
    def __init__(self, parent=None, width=5, height=4, dpi=100, treeWidgetItem = None):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)

        self.compute_initial_figure(treeWidgetItem)

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QtWidgets.QSizePolicy.Expanding,
                                   QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def compute_initial_figure(self, treeWidgetItem):
        pass


class MyCanvas(MyMplCanvas):
    """Simple canvas with a sine plot."""
    def compute_initial_figure(self, treeWidgetItem):

        '''
        t = arange(0.0, 3.0, 0.01)
        s = sin(2*pi*t)
        '''
        
        self.axes.plot(treeWidgetItem.x, treeWidgetItem.y)


class DemoMdi(QMainWindow):
    def __init__(self, parent=None):
        super(DemoMdi, self).__init__(parent)

        # 设置窗口标题
        self.setWindowTitle('MDI with a dockWidget tree and a tab-view mdiArea')
        # 设置窗口大小
        self.resize(800, 640)
        self.initUi()

        self.mytimer = QTimer(self)
        self.mytimer.start(1000)
        self.mytimer.timeout.connect(self.timerCallback)

    def initUi(self):
        self.initMenuBar()
        self.initToolBar()
        self.initDockTree()
        self.initStatusBar()

        self.mdiArea = QMdiArea(self)
        self.setCentralWidget(self.mdiArea)

        # set as tabbedView by default
        self.mdiArea.setViewMode(QMdiArea.TabbedView)  
        self.mdiArea.setTabShape(QTabWidget.Triangular)
        self.mdiArea.setTabsClosable(True)
        self.mdiArea.setTabsMovable(True)
        
        # index of document
        self.newDocIndex = 1

    def initDockTree(self):
        self.dockWind = QDockWidget(self)
        self.dockWind.setWindowTitle('QProfile Explorer')

        self.initTree()
        self.dockWind.setWidget(self.tree)
        self.dockWind.setFloating(False)                            # set floating = false
        self.addDockWidget(Qt.LeftDockWidgetArea, self.dockWind)    # set the position at left side
        # remove all features of DockWidget like Closable, Moveable, Floatable, VerticalTitle etc.
        self.dockWind.setFeatures(QDockWidget.NoDockWidgetFeatures) 

    def initTree(self):
        self.tree = QTreeWidget()
        self.tree.setColumnCount(1)  #设置列数
        #self.tree.setHeaderLabels(['QProfiler items'])  #设置树形控件头部的标题
        self.tree.setIndentation(20)  # 项目的缩进
        self.tree.setHeaderHidden(True)
        #设置根节点

        Perfmon = myQTreeWidgetItem(sin(pi*perfmon_x))
        Perfmon.setText(0, 'Perfmon')
        perfmon_00 = myQTreeWidgetItem(sin(2*pi*perfmon_x))
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
        # CPU 
        cpuLoad = QTreeWidgetItem()
        cpuLoad.setText(0, 'CPU')
        cpuLoad_1 = QTreeWidgetItem()
        cpuLoad_1.setText(0, 'core 1')
        cpuLoad.addChild(cpuLoad_1)
        cpuLoad_2 = QTreeWidgetItem()
        cpuLoad_2.setText(0, 'core 2')
        cpuLoad.addChild(cpuLoad_2)
        self.tree.addTopLevelItem(cpuLoad)

        # treeItem signal
        self.tree.itemClicked[QTreeWidgetItem, int].connect(self.treeItemClicked)

    def treeItemWindow_open(self, item):
        title = item.text(0)
        subWind = QMdiSubWindow(self)
        subWind.setAttribute(Qt.WA_DeleteOnClose)
        subWind.setWindowTitle(title)
        self.newDocIndex += 1
        mainWid = QWidget()
        l = QtWidgets.QVBoxLayout(mainWid)
        txtWind = QPlainTextEdit(mainWid)
        txtWind.setPlainText(f"perfmon.x = {item.x}, \n y = {item.y}")
        figWind = MyCanvas(mainWid, width=5, height=4, dpi=100, treeWidgetItem = item)
        l.addWidget(figWind)
        l.addWidget(txtWind)
        l.setStretch(0, 3)  # 设置第一列的伸展比例为 3
        l.setStretch(1, 1)  # 设置第二列的伸展比例为 1， 这样2列的伸展比为3：1

        subWind.setWidget(mainWid)
        self.mdiArea.addSubWindow(subWind)
        subWind.show()

    def treeItemClicked(self, item, column):
        tab = self.get_treeItem_tab(item.text(column))
        if tab is not None:
            tab.setFocus()
        else:
            if item.text(column) == 'Perfmon':
                self.treeItemWindow_open(item)
            else:
                newDoc = QMdiSubWindow(self)
                newDoc.setAttribute(Qt.WA_DeleteOnClose)
                newDoc.setWindowTitle(item.text(column))
                self.newDocIndex += 1
                newDoc.setWidget(QPlainTextEdit(item.text(column)*10, newDoc))
                self.mdiArea.addSubWindow(newDoc)
                newDoc.show()



    def get_treeItem_tab(self, title):
        for wind in self.mdiArea.subWindowList():
            if title == wind.windowTitle():
                return wind
        return None


    def initStatusBar(self):
        self.statusBar = self.statusBar()
        self.statusBar.showMessage('Ready to start ...', 0)

    def initMenuBar(self):
        menuBar = self.menuBar()
        style = QApplication.style()

        #==== 文件 ====#
        fileMenu = menuBar.addMenu('文件')

        #新建一个文档
        aFileNew = QAction('新建文档', self)
        aFileNew.setIcon(style.standardIcon(QStyle.SP_FileIcon))
        aFileNew.triggered.connect(self.onFileNew)
        fileMenu.addAction(aFileNew)

        #打开一个文档
        aFileOpen = QAction('打开文档', self)
        aFileOpen.setIcon(style.standardIcon(QStyle.SP_DialogOpenButton))
        aFileOpen.triggered.connect(self.onFileOpen)
        fileMenu.addAction(aFileOpen)

        #关闭一个文档
        aFileCloseAll = QAction('关闭全部', self)
        aFileCloseAll.setIcon(style.standardIcon(QStyle.SP_DialogCloseButton))
        aFileOpen.triggered.connect(self.onFileCloseAll)
        fileMenu.addAction(aFileCloseAll)

        #添加分割线
        fileMenu.addSeparator()

        #退出
        aFileExit = QAction('退出', self)
        aFileExit.triggered.connect(self.close)
        fileMenu.addAction(aFileExit)

        #==== 编辑 ====#
        editMenu = menuBar.addMenu('编辑')

        #剪切
        aEditCut = QAction('剪切', self)
        aEditCut.setIcon(QIcon(':/ico/cut.png'))
        aEditCut.triggered.connect(self.onEditCut)
        editMenu.addAction(aEditCut)

        #复制
        aEditCopy = QAction('复制', self)
        aEditCopy.setIcon(QIcon(':/ico/copy.png'))
        aEditCopy.triggered.connect(self.onEditCopy)
        editMenu.addAction(aEditCopy)

        #粘贴
        aEditPaste = QAction('粘贴', self)
        aEditPaste.setIcon(QIcon(':/ico/paste.png'))
        aEditPaste.triggered.connect(self.onEditPaste)
        editMenu.addAction(aEditPaste)

        #==== 窗口排列方式 ====#
        windowMenu = menuBar.addMenu('窗口')

        #子窗口模式
        aWndSubView = QAction('子窗口模式', self)
        aWndSubView.triggered.connect(lambda: self.onWinowdMode(0))
        windowMenu.addAction(aWndSubView)
        #标签页模式
        aWndTab = QAction('标签页模式', self)
        aWndTab.triggered.connect(lambda: self.onWinowdMode(1))
        windowMenu.addAction(aWndTab)

        windowMenu.addSeparator()

        #平铺模式
        aWndTile = QAction('平铺模式', self)
        aWndTile.triggered.connect(lambda: self.onWinowdMode(2))
        windowMenu.addAction(aWndTile)
        #窗口级联模式
        aWndCascade = QAction('窗口级联模式', self)
        aWndCascade.triggered.connect(lambda: self.onWinowdMode(3))
        windowMenu.addAction(aWndCascade)

    def initToolBar(self):
        toolBar = self.addToolBar('ToolBar')
        style = QApplication.style()

        min_width = 64

        btnFileNew = QToolButton(self)
        btnFileNew.setText('新建文档')
        btnFileNew.setMinimumWidth(min_width)
        btnFileNew.setIcon(style.standardIcon(QStyle.SP_FileIcon))
        btnFileNew.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnFileNew.clicked.connect(self.onFileNew)
        toolBar.addWidget(btnFileNew)

        btnFileOpen = QToolButton(self)
        btnFileOpen.setText('打开文档')
        btnFileOpen.setMinimumWidth(min_width)
        btnFileOpen.setIcon(style.standardIcon(QStyle.SP_DialogOpenButton))
        btnFileOpen.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnFileOpen.clicked.connect(self.onFileOpen)
        toolBar.addWidget(btnFileOpen)

        btnFileCloseAll = QToolButton(self)
        btnFileCloseAll.setText('关闭全部')
        btnFileCloseAll.setMinimumWidth(min_width)
        btnFileCloseAll.setIcon(style.standardIcon(
            QStyle.SP_DialogCloseButton))
        btnFileCloseAll.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnFileCloseAll.clicked.connect(self.onFileCloseAll)
        toolBar.addWidget(btnFileCloseAll)

        toolBar.addSeparator()

        btnEditCut = QToolButton(self)
        btnEditCut.setText('剪切')
        btnEditCut.setMinimumWidth(64)
        btnEditCut.setIcon(QIcon(':/ico/cut.png'))
        btnEditCut.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnEditCut.clicked.connect(self.onEditCut)
        toolBar.addWidget(btnEditCut)

        btnEditCopy = QToolButton(self)
        btnEditCopy.setText('复制')
        btnEditCopy.setMinimumWidth(64)
        btnEditCopy.setIcon(QIcon(':/ico/copy.png'))
        btnEditCopy.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnEditCopy.clicked.connect(self.onEditCopy)
        toolBar.addWidget(btnEditCopy)

        btnEditPaste = QToolButton(self)
        btnEditPaste.setText('粘贴')
        btnEditPaste.setMinimumWidth(64)
        btnEditPaste.setIcon(QIcon(':/ico/paste.png'))
        btnEditPaste.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        btnEditPaste.clicked.connect(self.onEditPaste)
        toolBar.addWidget(btnEditPaste)

    def msgCritical(self, strInfo):
        dlg = QMessageBox(self)
        dlg.setIcon(QMessageBox.Critical)
        dlg.setText(strInfo)
        dlg.show()

    def onFileNew(self):
        newDoc = QMdiSubWindow(self)
        newDoc.setAttribute(Qt.WA_DeleteOnClose)
        newDoc.setWindowTitle('新文档 ' + str(self.newDocIndex))
        self.newDocIndex += 1
        newDoc.setWidget(QPlainTextEdit(newDoc))
        self.mdiArea.addSubWindow(newDoc)
        newDoc.show()
        

    def onFileOpen(self):
        path, _ = QFileDialog.getOpenFileName(self, '打开文件', '', '文本文件 (*.txt, *.prf)')
        if path:
            try:
                with open(path, 'rU') as f:
                    text = f.read()
            except Exception as e:
                self.msgCritical(str(e))
            else:
                openDoc = QMdiSubWindow(self)
                openDoc.setWindowTitle(path)
                txtEdit = QPlainTextEdit(openDoc)
                txtEdit.setPlainText(text)
                openDoc.setWidget(txtEdit)
                self.mdiArea.addSubWindow(openDoc)
                openDoc.show()

    def onFileCloseAll(self):
        self.mdiArea.closeAllSubWindows()

    def onEditCut(self):
        txtEdit = self.mdiArea.activeSubWindow().widget()
        txtEdit.cut()

    def onEditCopy(self):
        txtEdit = self.mdiArea.activeSubWindow().widget()
        txtEdit.copy()

    def onEditPaste(self):
        txtEdit = self.mdiArea.activeSubWindow().widget()
        txtEdit.paste()

    def onWinowdMode(self, index):
        if index == 3:
            self.mdiArea.cascadeSubWindows()
        elif index == 2:
            self.mdiArea.tileSubWindows()
        elif index == 1:
            self.mdiArea.setViewMode(QMdiArea.TabbedView)
        else:
            self.mdiArea.setViewMode(QMdiArea.SubWindowView)

    def timerCallback(self):
        self.statusBar.showMessage(f'Document Index = {self.newDocIndex}, subWind num ={len(self.mdiArea.subWindowList())}', 0)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = DemoMdi()
    window.show()
    sys.exit(app.exec())