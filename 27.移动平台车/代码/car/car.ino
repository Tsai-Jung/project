#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

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
  { 0x24, 0xA1, 0x60, 0x2E, 0xDE, 0x55 },  //舵机车
  { 0x80, 0x7D, 0x3A, 0x1A, 0xB6, 0x78 },  //遥控器
};

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

//舵机车
void mysetup0() {
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  servo1.attach(14);  //14左前方
  servo2.attach(5);   //5右前方
  servo3.attach(13);  //13左后方
  servo4.attach(4);   //4右后方
  delay(200);
}

//遥控器
void mysetup1() {
  delay(200);
}

void setup() {
  Serial.begin(9600);
  espnowinit();
  Serial.println("启动了");
  if (mymac == 0) mysetup0();
  if (mymac == 1) mysetup1();
}


void myloop0() {  //舵机车
  if (typeword == 1) {
    if (worddata == "go") {
      // 前进
      servo1.write(150);
      servo2.write(150);
      servo3.write(150);
      servo4.write(150);
    } else if (worddata == "back") {
      // 退后
      servo1.write(30);
      servo2.write(30);
      servo3.write(30);
      servo4.write(30);
    } else if (worddata == "left") {
      // 左边
      servo1.write(30);
      servo2.write(150);
      servo3.write(30);
      servo4.write(150);
    } else if (worddata == "right") {
      // 右边
      servo1.write(150);
      servo2.write(30);
      servo3.write(150);
      servo4.write(30);
    } else if (worddata == "stop") {
      // 暂停
      servo1.write(90);
      servo2.write(90);
      servo3.write(90);
      servo4.write(90);
    }
    typeword = 0;
  }
}

String xdata;
String receive() {
  String data;
  if (Serial.available()) {
    delay(10);
    data = (char)Serial.read();
    data += receive();
  }
  return data;
}

String gogogo;
void myloop1() {  //遥控器

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
    gogogo = cOutput[0];
    myData.a = gogogo;
    myData.b = 0;
    sendone(0);
  }
}

  void loop() {
    if (mymac == 0) myloop0();
    if (mymac == 1) myloop1();
    delay(100);
  }
