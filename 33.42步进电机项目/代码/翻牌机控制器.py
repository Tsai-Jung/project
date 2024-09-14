import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QPushButton, QLabel
import serial
import time
# 打开串口，根据需要修改串口号和波特率
ser = serial.Serial('com5', 9600)

class MyWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()

        # 第一排添加电机名称标签
        for i in range(1, 11):
            label = QLabel(f'电机{i}')
            layout.addWidget(label, 0, i)

        # 2 - 10 排按钮及对应输出数字
        button_numbers_1_to_10 = list(range(1, 11))
        index = 0
        for row in range(1, 10 + 1):
            for col in range(1, 11):
                button = QPushButton(f'按钮{row}')
                button.clicked.connect(lambda checked, num=row, num2=col: self.on_button_clicked(num, num2))
                layout.addWidget(button, row, col)
                index += 1

        # 第 11 排总控制标签
        total_control_label = QLabel('总控制')
        layout.addWidget(total_control_label, 11, 1, 1, 10)

        # 12 - 15 排按钮及对应输出数字
        button_numbers_10_to_40 = [10 * i for i in range(1, 41)]
        index = 0
        for row in range(12, 15 + 1):
            for col in range(1, 11):
                button = QPushButton(f'按钮{col}')
                button.clicked.connect(lambda checked, num=button_numbers_10_to_40[index], num2=col: self.on_button_clicked(num,num2))
                layout.addWidget(button, row, col)
                index += 1

        self.setLayout(layout)

        self.setGeometry(300, 300, 800, 600)
        self.setWindowTitle('电机按钮窗口')
        self.show()

    def on_button_clicked(self, button_number,button_number2):
        print(f'按钮被按下，数字为{button_number}。')
        button_number = button_number*36
        message = f'{button_number2-1},go,{button_number}。'
        print(message)
        if(button_number>360):
            message = f'{button_number2-1},run,{button_number}。'
        ser.write(message.encode())


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyWindow()
    sys.exit(app.exec_())
