from PyQt5 import Qt, QtGui, QtCore, QtWidgets

class myDiaglog(QtWidgets.QMessageBox):
    def _init(self, parent):
        super(myDiaglog, self).__init__(parent)
        print('construct myDiaglog')
        self.padding = 12
    
    def closeEvent(self, evt):
        print('close myDiaglog:', self.windowTitle())


    def __del__(self):
        print('delete myDiaglog:', self)

class Window(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.button = QtWidgets.QPushButton('ShowTime!', self)
        self.button.clicked.connect(self.handleButton)
        layout = QtWidgets.QVBoxLayout(self)
        layout.addWidget(self.button)
        self._dialog = None

    def handleButton(self):
        if self._dialog is None:
            print('created new dialog windown')
            self._dialog = myDiaglog(self)
            self._dialog.setWindowTitle('Messages')
            
            # 所谓模态对话框就是在其没有被关闭之前，用户不能与同一个应用程序的其他窗口进行交互，直到该对话框关闭。
            # 对于非模态对话框，当被打开时，用户既可选择和该对话框进行交互，也可以选择同应用程序的其他窗口交互
            self._dialog.setModal(False)

            # 设置属性，当close窗口时，同时也删除该窗口对象， 默认是不销毁对象的。
            # 注意： 如果close时销毁了，那么该程序有问题。
            #self._dialog.setAttribute(QtCore.Qt.WA_DeleteOnClose, True) 
            
            pos = self.pos()
            pos.setX(pos.x() + self.width() + 10)
            self._dialog.move(pos)

        self._dialog.setText(
            'The time is: %s' % QtCore.QTime.currentTime().toString())
        self._dialog.show()

    def closeEvent(self, evt):
        print('close window:',self.windowTitle())



if __name__ == '__main__':

    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())