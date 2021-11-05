import random
import sys
from faker import Faker        # 界面中的人物名称和邮箱地址均为faker自动随机生成的
from PyQt5.QtCore import Qt, QModelIndex
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
 
fake = Faker(locale="zh")
 
 
class MyTabView(QTableView):
    def __init__(self, parent=None):
        """方便继承添加函数,可无视"""
        super().__init__(parent=parent)
        print(123)
 
 
class CenterItem(QStandardItem):
    def __init__(self, *args):
        """个性化item使用"""
        super().__init__(*args)
        self.setTextAlignment(Qt.AlignHCenter)
        self.setFont(QFont("楷体", 18))
        self.setForeground(QBrush(QColor(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))))
 
 
class MyWindow(QMainWindow):
    def __init__(self, panrent=None):
        """"""
        super().__init__()
        self.resize(444, 333)
        widget = QWidget()
        self.setCentralWidget(widget)
 
        lay = QVBoxLayout()
        widget.setLayout(lay)
 
        # tableview
        self.tab_view = MyTabView(self)
        lay.addWidget(self.tab_view)
        self.tab_view.clicked.connect(self.tab_view_clicked)
 
        # 创建并设置模型
        self.model = QStandardItemModel(self)
        self.model.setHorizontalHeaderLabels(["名称", "邮箱"])
        self.tab_view.setModel(self.model)
        [self.model.appendRow([CenterItem(fake.name()), CenterItem(fake.email())]) for x in range(100)]  # 随机添加数据
        self.tab_view.resizeColumnsToContents()  # 宽度自适应
 
        self.edit = QLineEdit(self)
        lay.addWidget(self.edit)
 
        self.btn = QPushButton(self)
        self.btn.setText("添加行")
        self.btn.clicked.connect(self.btn_clicked)
        lay.addWidget(self.btn)
 
    def btn_clicked(self):
        """点击添加行"""
        self.model.appendRow([CenterItem(fake.name()), CenterItem(fake.email())])
        # self.tab_view.setCurrentIndex()
        pass
 
    def tab_view_clicked(self, index: QModelIndex):
        """列表视图被单击"""
        col1 = self.model.horizontalHeaderItem(0).text()
        col2 = self.model.horizontalHeaderItem(1).text()
 
        item = self.model.itemFromIndex(index)  # type:QStandardItem
        col1val = self.model.item(item.row(), 0).text()
        col2val = self.model.item(item.row(), 1).text()
        self.edit.setText("index:{index},   "
                          "{col1}:{col1val},   "
                          "{col2}:{col2val}".format(index=item.row(),
                                                    col1=col1,
                                                    col1val=col1val,
                                                    col2=col2,
                                                    col2val=col2val))
        print(item.row(), item.text())
 
 
if __name__ == '__main__':
    app = QApplication(sys.argv)
    ui = MyWindow()
    ui.show()
    sys.exit(app.exec_())