//接收
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>
#include "Wire.h"
#include "AS5600.h"


String xdata;   //宣告资料
AS5600 as5600;  //  use default Wire

//接收数据保存的结构体和发送方一致
typedef struct struct_message {
  String a;
  int b;
} struct_message;

//创建结构体变量
struct_message myData;


String worddata;  //临时存放myData.a
int numdata;
int typeword = 0;  //临时开关避免执行时还收到讯息

//创建一个回调函数作为接收数据后的串口显示
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));


  worddata = myData.a;
  numdata = myData.b;
  typeword = 1;
  //简单打印出收到的讯息
  Serial.print(myData.a);
  Serial.print(" ");
  Serial.println(myData.b);
}


//计时
Ticker ticker;
int countT = 0;
void countime() {
  countT = countT + 1;
}

void lastTime(int a) {
  if (countT >= a) {
    digitalWrite(16, LOW);
    //ESP.deepSleep(3e6); //睡3秒
    //ESP.deepSleep(0);  //一直休眠
  }
}

void ResetTime(int a) {
  if (countT >= a) {
    ESP.deepSleep(1e6);  //1秒后重启
    //ESP.deepSleep(0);  //一直休眠
  }
}


//测电压部份
int analogValue;
void VAo0() {
  analogValue = analogRead(A0);
  Serial.println(analogValue);
  if (analogValue > 1022) {
    analogValue = 1022;
  }
  delay(10);
}

//IO2闪灯一次
void ledshow() {
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
  delay(100);
}

// 定义电机控制用常量

// A4988连接Arduino引脚号
const int dirPin = 14;   // 方向引脚
const int stepPin = 12;  // 步进引脚
//const int sleepPin = 2;

// 电机每圈步数
const int STEPS_PER_REV = 800;  //1/4就是800 全速是200

/*步进电机控制
IO13控制MS2    0是全速   1是1/4
IO12控制步数
IO14控制方向
*/

void setup() {
  //初始化窗口
  Serial.begin(9600);

  //设置ESP8266模式
  WiFi.mode(WIFI_STA);

  //初始化 ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //设置ESP8266角色：
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  //先前创建的功能 测试ESP-NOW通信
  esp_now_register_recv_cb(OnDataRecv);

  // Arduino控制A4988步进和方向的引脚为输出模式
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(dirPin, LOW);
  digitalWrite(13, HIGH);  //是1/4


  //初始化as5600
  Wire.begin(4, 5);                               //  前面4(SDA) 后面5(SCL)
  as5600.begin();                                 //  前面4(SDA) 后面5(SCL)
  as5600.setDirection(AS5600_COUNTERCLOCK_WISE);  // default, just be explicit.
  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);
  Serial.println("地址:");
  Serial.println(WiFi.macAddress());
  delay(100);
  digitalWrite(dirPin, LOW);  //逆

  for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
    if (as5600.readAngle() < 24) break;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}

int pernum;  //上次设定的位置

void loop() {
  if (typeword == 1) {
    // 设置电机顺时针旋转
    if (worddata == "go") digitalWrite(dirPin, LOW);  //逆
    // if (worddata == "back") digitalWrite(dirPin, LOW);//逆
    numdata = numdata % 360;
    numdata = numdata * 11.3;
    pernum = numdata;
    for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
      if (abs(as5600.readAngle() - numdata) < 24) break;
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    typeword = 0;
  }

  if (as5600.readAngle() > pernum) {
    if (abs(4070 - as5600.readAngle() + pernum) > abs(as5600.readAngle() - pernum)) {
      digitalWrite(dirPin, LOW);
    } else digitalWrite(dirPin, HIGH);
  }
  if (as5600.readAngle() < pernum) {
    if (abs(4070 - pernum + as5600.readAngle()) > abs(as5600.readAngle() - pernum)) {
      digitalWrite(dirPin, HIGH);
    } else digitalWrite(dirPin, LOW);
  }
  for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
    if ((abs(as5600.readAngle() - pernum) < 24) or (abs(4070 - as5600.readAngle() + pernum)) < 24) break;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

  delay(100);
}
