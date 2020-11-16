#!/usr/bin/python3
# -*- coding: utf-8 -*-
 
"""
Py40 PyQt5 tutorial 
 
In this example, we connect a signal
of a QSlider to a slot of a QLCDNumber. 
 
author: Jan Bodnar
website: py40.com 
last edited: January 2015
"""
 
import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QWidget, QLCDNumber, QSlider, 
    QHBoxLayout, QVBoxLayout, QApplication)
 
 
class Example(QWidget):
    
    def __init__(self, flag=0):
        super().__init__()
        
        self.initUI(flag)
        
        
    def initUI(self, flag):
        
        lcd = QLCDNumber(self)
        sld = QSlider(Qt.Horizontal, self)
 
        if flag == 0:
            vbox = QVBoxLayout()
            vbox.addWidget(lcd)
            vbox.addWidget(sld)
            self.setLayout(vbox)
        else:
            hbox = QHBoxLayout()
            hbox.addWidget(lcd)
            hbox.addWidget(sld)
            self.setLayout(hbox)
        
        sld.valueChanged.connect(lcd.display)
        
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Signal & slot')
        self.show()
        
 
if __name__ == '__main__':
    layoutDirection = 0
    app = QApplication(sys.argv)
    if len(sys.argv) >= 2:
        layoutDirection = int(sys.argv[1])

    ex = Example(layoutDirection)
    sys.exit(app.exec_())