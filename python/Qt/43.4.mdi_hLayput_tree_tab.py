'''
https://www.jb51.net/article/181948.htm
'''

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *


class MainWindow(QMainWindow):
    count = 0

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        #实例化Qmidarea区域
        self.mdi = QMdiArea()

        #设置为中间控件
        self.setCentralWidget(self.mdi)

        hlayout = QHBoxLayout()
        self.mdi.setLayout(hlayout)
        self.wd1 = QMdiSubWindow()
        self.wd2 = QMdiSubWindow()
        self.tree = QTreeWidget(self.mdi)
        self.tab = QTabWidget(self.mdi)
        self.wd1.setWidget(self.tree)
        self.wd2.setWidget(self.tab)
        hlayout.addWidget(self.wd1)
        hlayout.addWidget(self.wd2)



        #实例化菜单栏
        bar = self.menuBar()
        #添加主菜单
        file = bar.addMenu('&File')
        activate = bar.addMenu('&Activate')
        mode = bar.addMenu('&Mode')     
        view = bar.addMenu('&View')

        #添加 file 子菜单
        file.addAction('New')
        quitAction = file.addAction('Quit')
        # add 'Quit' submenu
        quitSubMenu = QMenu('Quit')
        quitSubMenu.addAction('Close Active')
        quitSubMenu.addAction('Close All')
        quitSubMenu.addAction('Exit')
        quitAction.setMenu(quitSubMenu)

        # Add Activate actions
        activate.addAction('Activate one')
        activate.addAction('Activate next')
        activate.addAction('Activate previous')

        # Add Mode ations
        mode.addAction('Cascade subWindow')
        mode.addAction('Tiled subWindow')
        mode.addAction('Tab view').setShortcut('ctrl+t')
        mode.addAction('SubWindow view')

        # 'view' 继续增加 子菜单
        view.addAction('Show Normal')
        view.addAction('Show Fullscreen').setShortcut('ctrl+F')
        view.addAction('Show All')

        #点击QAction绑定自定义的槽函数（传递有值【QAction】）
        file.triggered[QAction].connect(self.windowaction)
        activate.triggered[QAction].connect(self.windowaction)
        mode.triggered[QAction].connect(self.windowaction)
        view.triggered[QAction].connect(self.windowaction)
        #设置主窗口的标题
        self.setWindowTitle("MDI demo")

        #set status bar
        self.statusBar().showMessage(
            'a statusbar ready to show message here ...')

    def windowaction(self, q):
        # print('Triggered:', q.__dict__)
        print('Triggered:', q.text())
        # print(dir(q))
        if q.text() == 'New':
            #子窗口增加一个
            MainWindow.count = MainWindow.count + 1
            #实例化多文档界面对象
            sub = QMdiSubWindow()
            #向sub内添加内部控件
            sub.setWidget(QTextEdit(f'text: count = {MainWindow.count}'))
            if (MainWindow.count == 1):
                sub.setWidget(QPushButton('test'))
            #设置新建子窗口的标题
            sub.setWindowTitle('subWindow' + str(MainWindow.count))
            sub.resize(300, 240)
            sub.setAttribute(Qt.WA_DeleteOnClose)
            #将子窗口添加到Mdi区域
            self.mdi.addSubWindow(sub)
            
            #子窗口显示
            sub.show()
        if q.text() == 'Cascade subWindow':
            #cascadeSubWindows()：安排子窗口在Mdi区域级联显示
            self.mdi.cascadeSubWindows()
        if q.text() == 'Tiled subWindow':
            #tileSubWindow():安排子窗口在Mdi区域平铺显示
            self.mdi.tileSubWindows()

        if q.text() == 'Tab view':
            self.mdi.setViewMode(QMdiArea.TabbedView)
            print('view mode: ', self.mdi.viewMode)

        if q.text() == 'SubWindow view':
            self.mdi.setViewMode(QMdiArea.SubWindowView)
            print('view mode: ', self.mdi.viewMode)

        # close
        if q.text() == 'Close Active':
            self.mdi.closeActiveSubWindow()

        if q.text() == 'Close All':
            self.mdi.closeAllSubWindows()

        if q.text() == 'Exit':
            self.close()

        if q.text() == 'Activate one':  # ???
            print('activate no, do nothing ???')
            self.mdi.activateWindow()
        
        if q.text() == 'Activate next':
            self.mdi.activateNextSubWindow()

        if q.text() == 'Activate previous':
            self.mdi.activatePreviousSubWindow()

        if q.text() == 'Show Normal':
            wind = self.mdi.activeSubWindow()
            if wind is not None:
                wind.showNormal()

        if q.text() == 'Show Fullscreen':
            self.mdi.tab
             #  subWindow 无法显示 全屏模式！！
            print('==> Full Screen：  subWindow 无法显示 全屏模式！！') 
            wind = self.mdi.activeSubWindow()
            if wind is not None:
                print('Is the activeWindow fullscreen:', wind.isFullScreen())  
                wind.setWindowFlags(Qt.Dialog)
                wind.showFullScreen()

        if q.text() == 'Show All':
            for wd in self.mdi.subWindowList():
                print(f'{wd.windowTitle()} , visible:{wd.isVisible()}, Enable:{wd.isEnabled()}')
                if (wd.isVisible() is False):
                    print('show it again')
                    wd.show()

 


if __name__ == '__main__':
    app = QApplication(sys.argv)
    demo = MainWindow()
    demo.show()
    sys.exit(app.exec_())
