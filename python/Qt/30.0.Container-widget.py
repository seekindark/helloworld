import sys
from PyQt5.QtWidgets import QApplication, \
    QWidget, QPushButton, QAction, QLineEdit, \
    QMessageBox, QMainWindow
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot

class mainWindow(QMainWindow):
    #the QmainWindow has the menu bar
    def __init__(self):
        super(mainWindow, self).__init__()
        self.title = "MainWin - PY"
        self.left = 100  #the coordinate of window opened on screen
        self.top = 100
        self.width = 640
        self.height = 320
        self.initUI()       # initialize GUI
        self.count = 0

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        #create the menu bar container
        mainMenu = self.menuBar()
        fileMenu = mainMenu.addMenu("file")
        editMenu = mainMenu.addMenu("edit")
        viewMenu = mainMenu.addMenu("view")
        searchMenu = mainMenu.addMenu("search")
        toolMenu = mainMenu.addMenu("tool")
        helpMenu = mainMenu.addMenu("help")

        exitButton = QAction("Exit", self)
        exitButton.setShortcut("ctrl+Q")
        # self.close is the standard function
        exitButton.triggered.connect(self.close)
        # unwrap menu of fileMenu
        fileMenu.addAction(exitButton)


        self.show()



class App(QWidget):
    # Qwiget: provide methods for  window configuration and initiation
    def __init__(self):
        super(App, self).__init__()
        self.title = "app - PY"
        self.left = 100  #the coordinate of window opened on screen
        self.top = 100
        self.width = 640
        self.height = 320
        self.initUI()
        self.count = 0

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left,self.top, self.width, self.height)
        #the first button
        button = QPushButton("example", self)
        button.setToolTip("this is an example button") #label when cursor hover
        button.move(10, 20) #the position of the corner of button at the window
        button.clicked.connect(self.on_click)   #binding the method of clicked button
        #the second button
        button2 = QPushButton("发送",self)
        button2.setToolTip("发送")
        button2.move(10, 80)
        button2.clicked.connect(self.submit)
        #the txt box
        self.textbox = QLineEdit(self)
        self.textbox.move(10, 150)
        self.textbox.resize(280,40)

        self.show()


    @pyqtSlot() #反应槽
    def on_click(self):
        print("COUNT = ", self.count)
        self.count += 1

    @pyqtSlot()
    def submit(self):
        msg = self.textbox.text()
        print(msg) # submit the text content in the TextBox Widget
        QMessageBox.question(self, "Tips", "->"+msg,
                             QMessageBox.Apply,
                             QMessageBox.Ok) # the message box (can be alert or tips to warn the user)
        self.textbox.setText("")



if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    rx = mainWindow()
    sys.exit(app.exec_())