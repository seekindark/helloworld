from PyQt5 import QtGui,QtCore
from PyQt5.QtWidgets import *
import sys
 
class DockDemo(QMainWindow):
    def __init__(self):
        super(DockDemo, self).__init__()

        # 悬浮窗口
        dock_poi = QDockWidget(self.tr("POI点"), self)
        dock_poi.setAllowedAreas(QtCore.Qt.NoDockWidgetArea)   ########禁止停靠任何地方
        dock_poi.setFloating(False)                             #######打开悬浮状态

        button_window = QWidget()           ######外部窗体
        list_window = QWidget()         #########内部承载滚动条窗体

        #######创建滚动条
        scroll = QScrollArea()
        scroll.setWidget(list_window)
        scroll.setMaximumSize(150, 400)

        ########设置上下布局，并添加内部窗体
        vbox = QVBoxLayout()
        vbox.addWidget(scroll)
        button_window.setLayout(vbox)

        dock_poi.setWidget(button_window)               ######显示外部窗体

        dock_poi.setMaximumSize(150,500)
        dock_poi.setMinimumSize(150,500)
        dock_poi.show()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main = DockDemo()
    main.show()
    sys.exit(app.exec_()) 