
import serial
import vgamepad
import time

ser=serial.Serial("com5",9600,timeout=1)
 
# 创建一个虚拟XBOX 360手柄
gamepad = vgamepad.VX360Gamepad()
 
# 定义所有的XBox360游戏手柄按键
UP    = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP
DOWN  = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN
LEFT  = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT
RIGHT = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT
 
START = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_START
BACK  = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_BACK
GUIDE = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_GUIDE
 
LEFT_THUMB     = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB
RIGHT_THUMB    = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB
LEFT_SHOULDER  = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER
RIGHT_SHOULDER = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER
 
A = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_A
B = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_B
X = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_X
Y = vgamepad.XUSB_BUTTON.XUSB_GAMEPAD_Y
 
 
def LEFT_TRIGGER(value):
    gamepad.left_trigger_float(value)
    # 左扳机轴 value改成0.0到1.0之间的浮点值
def RIGHT_TRIGGER(value):    
    gamepad.right_trigger_float(value)
    # 右扳机轴 value改成0.0到1.0之间的浮点值
def LEFT_JOYSTICK(x_value, y_value):
    gamepad.left_joystick_float(x_value, y_value)
    # 左摇杆XY轴  x_values和y_values改成-1.0到1.0之间的浮点值
def RIGHT_JOYSTCIK(x_value, y_value):
    gamepad.right_joystick_float(x_value, y_value)
    # 右摇杆XY轴  x_values和y_values改成-1.0到1.0之间的浮点值

def PadDo(word):
    if(word=="A"):
        gamepad.press_button(A)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(A)
        gamepad.update()
    if(word=="B"):
        gamepad.press_button(B)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(B)
        gamepad.update()
    if(word=="Y"):
        gamepad.press_button(Y)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(Y)
        gamepad.update()
    if(word=="X"):
        gamepad.press_button(X)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(X)
        gamepad.update()
    if(word=="UP"):
        gamepad.press_button(UP)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(UP)
        gamepad.update()
        time.sleep(0.1)
    if(word=="DOWN"):
        gamepad.press_button(DOWN)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(DOWN)
        gamepad.update()
    if(word=="LEFT"):
        gamepad.press_button(LEFT)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(LEFT)
        gamepad.update()
        time.sleep(0.1)
    if(word=="RIGHT"):
        gamepad.press_button(RIGHT)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(RIGHT)
        gamepad.update()
        time.sleep(0.1)
    if(word=="START"):
        gamepad.press_button(START)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(START)
        gamepad.update()
    if(word=="BACK"):
        gamepad.press_button(BACK)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(BACK)
        gamepad.update()
    if(word=="GUIDE"):
        gamepad.press_button(GUIDE)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(GUIDE)
        gamepad.update()
    if(word=="LEFT_THUMB"):
        gamepad.press_button(LEFT_THUMB)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(LEFT_THUMB)
        gamepad.update()
    if(word=="RIGHT_THUMB"):
        gamepad.press_button(RIGHT_THUMB)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(RIGHT_THUMB)
        gamepad.update()
    if(word=="LEFT_SHOULDER"):
        gamepad.press_button(LEFT_SHOULDER)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(LEFT_SHOULDER)
        gamepad.update()
    if(word=="RIGHT_SHOULDER"):
        gamepad.press_button(RIGHT_SHOULDER)
        gamepad.update()
        time.sleep(0.1)
        gamepad.release_button(RIGHT_SHOULDER)
        gamepad.update()



def PadJoy(word,num,num2):
    num=num/1000
    num2=num2/1000
    #print(num)
    if(word=="LTrigger"):LEFT_TRIGGER(num)
    if(word=="RTrigger"):RIGHT_TRIGGER(num)
    if(word=="LJoystick"):LEFT_JOYSTICK(num, num2)
    if(word=="RJoystick"):RIGHT_JOYSTICK(num, num2)
    gamepad.update()

time.sleep(5)
print("开始了")
while(1):
    if ser.in_waiting > 0:  # 如果串口有数据可读取
        text = ser.readline().decode().strip()  # 读取串口数据并转成字符串形式
        #print(text)
        
    try:
        text = text.split(",")
        PadDo(text[0])
        PadJoy(text[0],int(text[1]),int(text[2]))
    except:
        pass
 
del gamepad
print("虚拟手柄已销毁")
