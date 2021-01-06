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

        # self.mdi.setViewMode(QMdiArea.TabbedView)  ## 奇怪，只有setviewmode在这里执行，才能变成 tab 模式？？


        #设置为中间控件
        self.setCentralWidget(self.mdi)
        #实例化菜单栏
        bar = self.menuBar()
        #添加主菜单
        file = bar.addMenu('File')
        about = bar.addMenu('&About')
        #添加子菜单
        file.addAction('New')
        file.addAction('cascade')
        file.addAction('Tiled')
        quitAction = file.addAction('Quit')
        about.addAction('about')

        # 给’Quit' 继续增加 子菜单
        quitSubMenu = QMenu('Quit')
        quitSubMenu.addAction('Close Active')
        quitSubMenu.addAction('Close All')
        quitSubMenu.addAction('Exit')
        quitSubMenu.addAction('Activate one')
        quitSubMenu.addAction('Activate next')
        quitSubMenu.addAction('Activate previous')
        quitSubMenu.addAction('Tab view')
        quitSubMenu.addAction('SubWindow view')
        quitAction.setMenu(quitSubMenu)

        #点击QAction绑定自定义的槽函数（传递有值【QAction】）
        file.triggered[QAction].connect(self.windowaction)
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
            #将子窗口添加到Mdi区域
            self.mdi.addSubWindow(sub)
            
            #子窗口显示
            sub.show()
        if q.text() == 'cascade':
            #cascadeSubWindows()：安排子窗口在Mdi区域级联显示
            self.mdi.cascadeSubWindows()
        if q.text() == 'Tiled':
            #tileSubWindow():安排子窗口在Mdi区域平铺显示
            self.mdi.tileSubWindows()

        if q.text() == 'Close Active':
            self.mdi.closeActiveSubWindow()

        if q.text() == 'Close All':
            self.mdi.closeAllSubWindows()

        if q.text() == 'Activate one':  # ???
            self.mdi.activateWindow()
            self.mdi.pre
        
        if q.text() == 'Activate next':
            self.mdi.activateNextSubWindow()

        if q.text() == 'Activate previous':
            self.mdi.activatePreviousSubWindow()

        if q.text() == 'Tab view':
            self.mdi.setViewMode(QMdiArea.TabbedView)
            print('view mode: ', self.mdi.viewMode)

        if q.text() == 'SubWindow view':
            self.mdi.setViewMode(QMdiArea.SubWindowView)
            print('view mode: ', self.mdi.viewMode)

        if q.text() == 'Exit':
            self.close()



if __name__ == '__main__':
    app = QApplication(sys.argv)
    demo = MainWindow()
    demo.show()
    sys.exit(app.exec_())
