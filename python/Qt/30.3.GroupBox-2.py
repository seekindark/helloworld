import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow, QHBoxLayout, QDialog, \
     QVBoxLayout, QGridLayout, QGroupBox, QFormLayout, QRadioButton, QCheckBox, QFrame, QMenu
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot, Qt


class App(QMainWindow):
    def __init__(self):
        super(App, self).__init__()
        self.title = "QMainWindow with GridLayout"
        self.left = 300  #the coordinate of window opened on screen
        self.top = 300
        self.width = 600
        self.height = 480
        self.initUI()

    def initUI(self):
        print("init...")
        
        # set basic
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)

        # grid layout
        layout = QGridLayout()
        layout.addWidget(self.createGroupBox_exclusive_1st(), 0, 0)
        layout.addWidget(self.createGroupBox_exlusive_2nd(), 0, 1)
        layout.addWidget(self.createGroupBox_non_exlusive(), 1, 0)
        layout.addWidget(self.createGroupBox_pushBotton(), 1, 1)

        #  case 1:
        if True:
            widget = QWidget()
            widget.setLayout(layout)
            self.setCentralWidget(widget)       # 如果时 QMainWindow, layout 只能设置在centralWidget上, 所以是需要一个 widget 作为 centralWdiget
        else:
            frame = QFrame()
            frame.setLayout(layout)
            self.setCentralWidget(frame)
        self.show()
    
    def createGroupBox_exclusive_1st(self):
        groupbox = QGroupBox('&Exclusive Radio Buttions')
        radio1 = QRadioButton('&Radio button 1')
        radio2 = QRadioButton('R&adio button 2')
        radio3 = QRadioButton('Ra&dio button 3')
        radio1.setChecked(True)
        
        vLayout = QVBoxLayout()
        vLayout.addWidget(radio1)
        vLayout.addWidget(radio2)
        vLayout.addWidget(radio3)

        groupbox.setLayout(vLayout)

        return groupbox

    def createGroupBox_exlusive_2nd(self):
        groupbox = QGroupBox('E&xclusive Radio Buttions')
        groupbox.setCheckable(True)         # groupBox  标题前生成一个checkbox 
        groupbox.setChecked(True)

        radio1 = QRadioButton('&Radio button 1')
        radio2 = QRadioButton('R&adio button 2')
        radio3 = QRadioButton('Ra&dio button 3')
        radio1.setChecked(True)

        checkbox = QCheckBox('Checkbox button')
        checkbox.setChecked(True)
        
        vLayout = QVBoxLayout()
        vLayout.addWidget(radio1)
        vLayout.addWidget(radio2)
        vLayout.addWidget(radio3)
        vLayout.addWidget(checkbox)
        groupbox.setLayout(vLayout)

        return groupbox

    def createGroupBox_non_exlusive(self):
        groupbox = QGroupBox('&None-Exclusive Options')
        groupbox.setFlat(True)          # Groupbox的标题顶格 写

        checkbox1 = QCheckBox('Checkbox button 1')
        checkbox1.setChecked(True)
        checkbox2 = QCheckBox('Checkbox button 2')
        triaBox = QCheckBox('Tri-&state button')
        triaBox.setTristate(True)   #启用3种状态切换, PartiallyChecked, Unchecked, Checked
        triaBox.setCheckState(Qt.PartiallyChecked)

        vlayout = QVBoxLayout()
        vlayout.addWidget(checkbox1)
        vlayout.addWidget(checkbox2)
        vlayout.addWidget(triaBox)

        groupbox.setLayout(vlayout)
        return groupbox

    def createGroupBox_pushBotton(self):
        groupbox = QGroupBox('PushButton')
        groupbox.setCheckable(True)         # groupBox  标题前生成一个checkbox 
        groupbox.setChecked(True)

        pushButton1 = QPushButton('&Normal button 1')

        pushButton2 = QPushButton('toggle &button')
        pushButton2.setStyleSheet("background-color:rgb(35,65,90)")
        pushButton2.setCheckable(True)    # 类似于 checkbox 的按钮，点击后，会显示被点击的特征， 再点击，就显示 ‘取消点击’特征
        pushButton2.setChecked(True)
        pushButton2.clicked.connect(self.on_toggle_click)
        self.toggleButton = pushButton2

        pushButton3 = QPushButton('&Flat button')
        pushButton3.setStyleSheet("background-color:rgb(0,100,90)")
        pushButton3.setFlat(True)           # 平面化按钮

        pushButton4 = QPushButton('Pop&up button')
        pushButton4.setStyleSheet("background-color:rgb(60,100,0)")
        menu = QMenu('mxxx',self)
        menu.addAction('item 1')
        menu.addAction('item 2')
        menu.addAction('item 3')
        menu.addAction('item 4')
        subActions = menu.addAction('sub actions')
        pushButton4.setMenu(menu)
        
        subMenu = QMenu('subxxx', self)
        subActions.setMenu(subMenu)
        subMenu.addAction('sub 1')
        subMenu.addAction('sub 2')
        subMenu.addAction('sub 3')

        vLayout = QVBoxLayout()
        vLayout.addWidget(pushButton1)
        vLayout.addWidget(pushButton2)
        vLayout.addWidget(pushButton3)
        vLayout.addWidget(pushButton4)
        groupbox.setLayout(vLayout)
        return groupbox

    def on_toggle_click(self):
        print('toggle button checkedState ', self.toggleButton.isChecked())

if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = App()
    sys.exit(app.exec_())