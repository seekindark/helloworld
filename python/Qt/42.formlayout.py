import sys
from PyQt5.QtWidgets import QApplication ,QWidget ,QFormLayout , \
                            QLineEdit, QLabel, QPushButton,QRadioButton
from PyQt5.QtCore import Qt


class Winform(QWidget):
    def __init__(self, parent=None):
        super(Winform, self).__init__(parent)
        self.setWindowTitle("窗体布局管理例子")
        self.resize(400, 100)
        fromlayout = QFormLayout()
        labl1 = QLabel("标签1")
        lineEdit1 = QLineEdit()
        labl2 = QLabel("标签2")
        lineEdit2 = QLineEdit()
        labl3 = QLabel("标签3")
        lineEdit3 = QLineEdit()
        fromlayout.addRow(labl1, lineEdit1)
        fromlayout.addRow(labl2, lineEdit2)
        fromlayout.addRow(labl3, lineEdit3)
        fromlayout.addRow('xxxssssssssssdddd', QLineEdit())
        fromlayout.addRow(QPushButton('aaa'), QRadioButton('bbb'))
        # 该参数通常适用于小屏幕中，当标签和文本框在本行显示不全时，文本框会显示在下一行，使得标签独占一行
        fromlayout.setRowWrapPolicy(QFormLayout.WrapLongRows)
        fromlayout.setLabelAlignment(Qt.AlignRight)  # 设置标签的对齐方式
        self.setLayout(fromlayout)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    form = Winform()
    form.show()
    sys.exit(app.exec_())