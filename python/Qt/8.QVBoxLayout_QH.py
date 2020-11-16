# -*- coding: utf-8 -*-
 
"""
Py40 PyQt5 tutorial 
 
In this example, we position two push
buttons in the bottom-right corner 
of the window. 
 
author: Jan Bodnar
website: py40.com 
last edited: January 2015
"""
 
import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, 
    QHBoxLayout, QVBoxLayout, QApplication)
 
 
class Example(QWidget):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):
        
        okButton = QPushButton("OK")
        cancelButton = QPushButton("Cancel")
 
        vbox = QHBoxLayout()
        vbox.addStretch(0)
        vbox.addWidget(okButton)
        vbox.addWidget(cancelButton)
 
        hbox = QVBoxLayout()
        hbox.addStretch(0)
        hbox.addLayout(vbox)
        
        self.setLayout(hbox)    
        
        self.setGeometry(300, 300, 300, 150)
        self.setWindowTitle('Buttons')    
        self.show()
        
        
if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())