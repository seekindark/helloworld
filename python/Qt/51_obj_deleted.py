    #!/usr/local/bin/python3  
    
import sys
from PyQt5.QtCore import * 
from PyQt5.QtGui import *   
from PyQt5.QtWidgets import *
    

class Window(QMainWindow):      
    def __init__(self):         
        super().__init__()         
        self.button1 = QPushButton("1")        
        self.button2 = QPushButton("2")         
        self.setCentralWidget(self.button1)         
        self.button1.clicked.connect(lambda: self.setCentralWidget(self.button2))         
        self.button2.clicked.connect(lambda: self.setCentralWidget(self.button1))         
        self.show()  
            
if __name__ == '__main__':      
    import sys      
    app = QApplication(sys.argv)     
    window = Window()     
    sys.exit(app.exec_()) 