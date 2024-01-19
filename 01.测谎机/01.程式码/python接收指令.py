import serial   #导入串口通讯库
import sys
import time
import urllib
import urllib.request
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

ser=serial.Serial("com3",9600,timeout=1)

a=""
stext=""

##串口输入指令
##c=input("")
##ser.write(c.encode())
##while 1:
##    text=ser.readline().decode(errors='ignore')
##    if(text==''):
##        pass
##    else:
##        print(text)#一次读取多少行
    

def get_file_content(filePath): #讀取圖片
    with open(filePath, 'rb') as fp:
        return fp.read()

def download(url,name):
    urllib.request.urlretrieve(url, name)
    
class Windows(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):
        
        
        #文字輸入使用
        self.inputText = QLineEdit('',self)
        self.inputText2 = QTextEdit('',self)

        #時間控鍵使用
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.timecircul)
        self.timer.start(3000) #只能設定一次 3000等於3秒鐘

        
        #按鍵使用
        self.okButton = QPushButton("開始")
        self.okButton.clicked.connect(self.buttonClicked)

       
        hbox = QHBoxLayout() 
        #vbox.addStretch(1) #空1~99行
        hbox.addWidget(self.inputText)
        hbox2 = QHBoxLayout()
        hbox2.addWidget(self.okButton)
        hbox3 = QHBoxLayout()
        hbox3.addWidget(self.inputText2) #加入控鍵

        
        
        vbox = QVBoxLayout(self)
        vbox.addLayout(hbox) #放入框架
        vbox.addLayout(hbox2)
        vbox.addLayout(hbox3)
        
        #視窗設定
        self.setLayout(vbox) #顯示框架
        self.setGeometry(600, 600, 300, 300) #起始位置
        self.setWindowTitle('功能集合')  #修改標題
        self.setWindowIcon(QIcon('kitty.jpg'))#修改Icon
        self.setWindowFlags(Qt.WindowStaysOnTopHint)#視窗最上層
        
        self.show() #視窗顯示

    def buttonClicked(self):
        global stext
        print('按鍵按下')
        self.okButton.setText('已經按下了') #修改按鍵文字
        c=self.inputText.text()
        ser.write(c.encode())
        self.inputText2.setText(self.inputText.text()+"\n"+stext)
        stext = a+"\n"+stext


    def timecircul(self):
        global a,stext
        self.timer.stop()
        while(ser.inWaiting()):
            a=ser.readline().decode(errors='ignore')
            print(a)#一次读取多少行
            self.inputText2.setText(a+"\n"+stext)
            stext = a+"\n"+stext
        self.timer.start()

def mainprogram():
    app = QApplication(sys.argv)
    ex = Windows()
    sys.exit(app.exec_())

    
if __name__ == '__main__':
    mainprogram()
