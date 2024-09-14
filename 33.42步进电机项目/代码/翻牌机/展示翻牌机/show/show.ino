
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>
#include "Wire.h"
#include "AS5600.h"


String xdata;   //宣告资料
AS5600 as5600;  //  use default Wire


/*
dst 保存分割好的数据的数组
str 被分割的字符串
spl 分割符
*/
int split(char dst[][20], char *str, const char *spl) {
  int n = 0;
  char *result = NULL;
  result = strtok(str, spl);

  while (result != NULL) {
    strcpy(dst[n++], result);
    result = strtok(NULL, spl);
  }
  return n;
}



//EspNow数据设定
//接收数据保存的结构体和发送方一致
typedef struct struct_message {
  String a;
  int b;
} struct_message;

//创建结构体变量
struct_message myData;
//根据自己的板子修改
uint8_t broadcastAddress[2][6] = {
  { 0xE0, 0x98, 0x06, 0xAF, 0x8E, 0x4C },  //42步进电机
  { 0x80, 0x7D, 0x3A, 0x1A, 0xB6, 0x78 },  //电脑接收器

};

/*
这是一个arspro和8266互相通讯的范例
需要用到

espnow协议
ticker计时
*/


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


//EspNow设定

//接收和发送
//在这种情况下，无论是否成功发送该消息，都会简单地打印出来
String cheakData = "good";
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("发送成功");
    cheakData = "good";
  } else {
    Serial.println("发送不成功");
    cheakData = "Bad";
  }
}

//创建一个回调函数作为接收数据后的串口显示
String worddata;  //临时存放myData.a
int numdata;
int typeword = 0;  //临时开关避免执行时还收到讯息
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

//计算板子有几个
int sizelen = 0;  //全部有几块板子
int mymac = 99;   //我的板子是第几块 新的板子是设为99
//espnow初始化
void espnowinit() {
  //设置ESP8266模式
  WiFi.mode(WIFI_STA);
  //初始化 ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //设置ESP8266角色：
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  //打印自己的版子ip
  Serial.print("我的IP地址: ");
  Serial.println(WiFi.macAddress());

  //计算板子有几个
  sizelen = sizeof(broadcastAddress) / sizeof(broadcastAddress[0]);
  Serial.print("板子数量有");
  Serial.println(sizelen);

  //对比自己的板子
  String macnum = "";
  for (int j = 0; j < sizelen; j++) {
    for (int i = 0; i <= 6; i++) {
      String Mac1 = String(broadcastAddress[j][i], HEX);
      Mac1.toUpperCase();
      //Serial.println(Mac1);
      //Serial.println(Mac1.length());
      if (Mac1.length() == 1) {
        Mac1 = "0" + Mac1;
      }
      if (i == 0) {
        macnum = Mac1;
      }
      if (i > 0 and i < 6) {
        macnum = macnum + ":" + Mac1;
      }
    }
    if (macnum == WiFi.macAddress()) {
      mymac = j;
      Serial.print("我的顺序是");
      Serial.println(j);
    }
    if (macnum != WiFi.macAddress()) {
      //与另一个ESP-NOW设备配对以发送数据
      esp_now_add_peer(broadcastAddress[j], ESP_NOW_ROLE_COMBO, 1, NULL, 0);
      //Serial.print("配对的板子");
      //Serial.println(macnum);
    }
  }

  //先前创建的功能。
  esp_now_register_send_cb(OnDataSent);

  //先前创建的功能 测试ESP-NOW通信
  esp_now_register_recv_cb(OnDataRecv);
}
//发送数据
void sendone(int i) {
  esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
  delay(50);
  int a = 0;  //临时计算重新传送次数
  while (cheakData == "Bad" and a < 10) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(100);
  }
  //Serial.print("已经发送给");
  //Serial.print(i);
  //Serial.println("号");
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


//步进电机
void mysetup0() {
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
  delay(100);
  digitalWrite(dirPin, HIGH);  //顺

  for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
    if (as5600.readAngle() < 24) break;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
void mysetup1() {
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  espnowinit();
  Serial.println("启动了");
  if (mymac == 0) mysetup0();
  if (mymac == 1) mysetup1();
}


int autogo = 4070;
int pernum;       //上次设定的位置
void myloop0() {  //42步进电机

  if (typeword == 1) {
    // 设置电机顺时针旋转
    if (worddata == "go") digitalWrite(dirPin, HIGH);  //顺
    // if (worddata == "back") digitalWrite(dirPin, LOW);//逆
    numdata = numdata % 360;
    numdata = numdata * 11.3;
    pernum = numdata;
    for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
      if (abs(as5600.readAngle() - numdata) < 24) break;
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    myData.a = "角度:";
    myData.b = as5600.readAngle() / 11.3;
    sendone(1);
    typeword = 0;
  }



  for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
    if (abs(as5600.readAngle() - autogo) < 24) break;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  autogo = autogo - 406.8;
  if (autogo < 5) {
    autogo = 4070;
  }
  delay(3000);
}

String receive() {
  String data;
  if (Serial.available()) {
    delay(10);
    data = (char)Serial.read();
    data += receive();
  }
  return data;
}


String gogogo = "";
//控制 编号0,go,角度
void myloop1() {  //电脑接收器
  xdata = receive();
  if (xdata != "") {
    Serial.print(xdata);
    xdata.replace("\n", "");
    //修改引出字符串
    char *comein = const_cast<char *>(xdata.c_str());  //转成字符串
    char cOutput[8][20];
    int cnt = split(cOutput, comein, ",");
    for (int i = 0; i < cnt; i++) {
      Serial.println(cOutput[i]);
    }
    gogogo = cOutput[1];
    if (gogogo == "go") {
      myData.a = cOutput[1];
      myData.b = atoi(cOutput[2]);
      Serial.println(myData.b);
      sendone(atoi(cOutput[0]));
      Serial.print("已完成");
    }
    if (gogogo == "run") {
      int num = 0;
      while (num <= 360) {
        myData.a = "go";
        myData.b = num;
        Serial.println(myData.b);
        sendone(atoi(cOutput[0]));
        Serial.print("已完成");
        num += 36;
        delay(1500);
      }
    }
  }
}

void loop() {
  if (mymac == 0) myloop0();
  if (mymac == 1) myloop1();
}
