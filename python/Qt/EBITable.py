import random
import sys
from faker import Faker        # 界面中的人物名称和邮箱地址均为faker自动随机生成的
from PyQt5.QtCore import Qt, QModelIndex
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *


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
        font = QFont()
        font.setFamily(u"Calibri")
        font.setPointSize(11)
        self.setFont(font)
        self.setText(f"{args[0]}")


class EBIWindow(QWidget):
    def __init__(self, panrent=None):
        """"""
        super().__init__()
        self.resize(444, 333)
  
        lay = QVBoxLayout()
        self.setLayout(lay)
 
        # tableview
        self.tab_view = MyTabView(self)
        self.tab_view.clicked.connect(self.tab_view_clicked)

         # 创建并设置模型
        self.model = QStandardItemModel(self)
        self.model.setHorizontalHeaderLabels(["DID", "Length", "Name", "Value"])
        self.tab_view.setModel(self.model)
        self.model.appendRow([CenterItem(0x183), CenterItem(32), CenterItem("BootSW_FP"), CenterItem("fasfa")])  # 随机添加数据
        self.model.appendRow([CenterItem(0x184), CenterItem(4), CenterItem("CAN-TxID"), CenterItem(0x7EC)])  # 随机添加数据
        self.tab_view.resizeColumnsToContents()  # 宽度自适应
 
        self.btn_read = QPushButton(self)
        self.btn_read.setText("Read EBI")
        self.btn_write = QPushButton(self)
        self.btn_write.setText("Write EBI")
        self.btn_read.clicked.connect(self.btn_clicked_Read)
        self.btn_write.clicked.connect(self.btn_clicked_Write)

        lay.addWidget(self.tab_view)

        lay.addWidget(self.btn_read)
        lay.addWidget(self.btn_write)
 
    def btn_clicked_Read(self):
        """点击添加行"""
        self.model.appendRow([CenterItem(0x185), CenterItem(4), CenterItem("CAN-RxID"), CenterItem(0x7E4)])
        # self.tab_view.setCurrentIndex()
        pass
 
    def btn_clicked_Write(self):
        """点击添加行"""
        self.model.appendRow([CenterItem(0x154), (4), CenterItem("bootMode"), CenterItem("Programming")])
        # self.tab_view.setCurrentIndex()
        pass

    def tab_view_clicked(self, index: QModelIndex):
        """列表视图被单击"""
        col1 = self.model.horizontalHeaderItem(0).text()
        col2 = self.model.horizontalHeaderItem(1).text()
 
        item = self.model.itemFromIndex(index)  # type:QStandardItem
        col1val = self.model.item(item.row(), 0).text()
        col2val = self.model.item(item.row(), 1).text()
        print("index:{index},   "
                          "{col1}:{col1val},   "
                          "{col2}:{col2val}".format(index=item.row(),
                                                    col1=col1,
                                                    col1val=col1val,
                                                    col2=col2,
                                                    col2val=col2val))
        print(item.row(), item.text())
 
 
if __name__ == '__main__':
    app = QApplication(sys.argv)
    ui = EBIWindow()
    ui.show()
    sys.exit(app.exec_())