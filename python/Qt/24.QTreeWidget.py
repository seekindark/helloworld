import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QIcon, QBrush, QColor
from PyQt5.QtCore import Qt


class TreeWidgetDemo(QMainWindow):
    def __init__(self, parent=None):
        super(TreeWidgetDemo, self).__init__(parent)
        self.setWindowTitle('TreeWidget 例子')

        self.tree = QTreeWidget()
        # 设置列数
        self.tree.setColumnCount(3)
        # 设置树形控件头部的标题
        self.tree.setHeaderLabels(['Key', 'Value','other'])

        # 设置根节点 of the 'tree'
        root = QTreeWidgetItem(self.tree)
        root.setText(0, 'Root')
        root.setIcon(0, QIcon('./images/root.png'))

        # todo 优化2 设置根节点的背景颜色
        brush_red = QBrush(Qt.red)
        root.setBackground(0, brush_red)
        brush_blue = QBrush(Qt.blue)
        root.setBackground(1, brush_blue)
        root.setBackground(2, QBrush(Qt.cyan))      # set the 3rd column in color 'cyan'

        # 设置树形控件的列的宽度
        self.tree.setColumnWidth(0, 150)        # set the width of 1st column

        # 设置子节点1
        child1 = QTreeWidgetItem()
        child1.setText(0, 'child1')
        child1.setText(1, 'ios')
        child1.setIcon(0, QIcon('./images/IOS.png'))
        # todo 优化1 设置节点的状态
        # 设置指定列的选中状态: Qt.Checked:节点选中; Qt.Unchecked:节点没有选中
        child1.setCheckState(0, Qt.Checked)

        root.addChild(child1)

        # 设置子节点2
        child2 = QTreeWidgetItem(root)
        child2.setText(0, 'child2')
        child2.setText(1, '')
        child2.setIcon(0, QIcon('./images/android.png'))
        child2.setCheckState(0, Qt.Unchecked)

        # 设置子节点3 , 节点3是节点2的子节点
        child3 = QTreeWidgetItem(child2)
        child3.setText(0, 'child3')
        child3.setText(1, 'android')
        child3.setIcon(0, QIcon('./images/music.png'))

        # 设置节点4， 是节点1的子节点
        child4 = QTreeWidgetItem(child1)
        child4.setText(0,'child4')
        child4.setText(1, 'testOS')
        child4.setText(2, 'any words here...')
        child4.setBackground(0, QBrush(Qt.darkGray))

        # 加载根节点的所有属性与子控件
        self.tree.addTopLevelItem(root)

        # TODO 优化3 给节点添加响应事件
        self.tree.clicked.connect(self.onClicked)

        #为tree增加顶级项目
        self.tree.addTopLevelItem(child1)       # ??

        # 节点全部展开
        self.tree.expandAll()
        self.setCentralWidget(self.tree)

    def onClicked(self, qmodeLindex):
        item = self.tree.currentItem()
        item.setCheckState(0, Qt.Checked)
        print('Key=%s,value=%s' % (item.text(0), item.text(1)))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    tree = TreeWidgetDemo()
    tree.show()
    tree.resize(800, 600)
    sys.exit(app.exec_())
