#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>




//计时
Ticker ticker;
int countT = 0;
void countime() {
  countT = countT + 1;
}
//接收方MAC地址 根据自己的板子修改


void lastTime(int a) {
  if (countT >= a) {
    digitalWrite(16, LOW);
    //ESP.deepSleep(3e6); //睡3秒
    //ESP.deepSleep(0);  //一直休眠
  }
}


uint8_t broadcastAddress[3][6] = { { 0x24, 0x62, 0xAB, 0x14, 0xBF, 0xFB },    //遥控器
                                   { 0x84, 0x0D, 0x8E, 0x9B, 0x56, 0x2D },    //左边控制器
                                   { 0x3C, 0x71, 0xBF, 0x2B, 0x29, 0x45 } };  //右边控制器

//接收数据保存的结构体和发送方一致
typedef struct struct_message {
  String a;
  int b;
} struct_message;

//创建结构体变量
struct_message myData;


//接收和发送
//在这种情况下，无论是否成功发送该消息，都会简单地打印出来
String cheakData = "good";
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
    cheakData = "good";
  } else {
    Serial.println("Delivery fail");
    cheakData = "Bad";
  }
}


String worddata; //预存data.a的数值
int typeword = 0;
//创建一个回调函数作为接收数据后的串口显示
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  worddata = myData.a;
  typeword = 1;
  Serial.println(myData.b);
}

//计算板子有几个
int sizelen = 0;  //全部有几块板子
int mymac = 99;   //我的板子是第几块


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
  Serial.print("ESP8266 Board MAC Address: ");
  Serial.println(WiFi.macAddress());

  //计算板子有几个
  sizelen = sizeof(broadcastAddress) / sizeof(broadcastAddress[0]);
  //Serial.print("板子数量");
  //Serial.println(sizelen);

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
      Serial.print(j);
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
  int a = 0;
  while (cheakData == "Bad" and a < 10) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(30);
  }
  Serial.print(i);
  Serial.println("号");
}


//IO2闪灯
void ledshow() {
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
}


void setup() {
  Serial.begin(9600);  // 启动串行通讯
  delay(100);
  espnowinit();
  delay(100);
  if (mymac == 0) { //遥控器
    pinMode(12, INPUT_PULLUP); //中
    pinMode(13, INPUT_PULLUP); //右
    pinMode(14, INPUT_PULLUP); //上
    pinMode(4, INPUT_PULLUP); //下
    pinMode(5, INPUT_PULLUP); //左
  }
  if (mymac == 1) { //左边控制器
    pinMode(12, OUTPUT);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(12, LOW);
  }
  if (mymac == 2) { //右边控制器
    pinMode(12, OUTPUT);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(12, LOW);
    

  }
  ticker.attach(1, countime);
}



void loop() {    //前进1,右转2,左转3,减速4,中速5
  if (mymac == 0) {
    if (digitalRead(12) != 1 ) {
      Serial.println("中");
      myData.b=5;
      sendone(1);
      sendone(2);
      while(digitalRead(12) != 1){
        delay(50);
      }
    }
    if (digitalRead(13) != 1 ) {
      Serial.println("右");
      myData.b=3;
      sendone(1);
      sendone(2);
      while(digitalRead(13) != 1){
        delay(50);
      }
    }
    if (digitalRead(14) != 1 ) {
      Serial.println("前");
      myData.b=1;
      sendone(1);
      sendone(2);
      while(digitalRead(14) != 1){
        delay(50);
      }
    }
    if (digitalRead(4) != 1 ) {
      Serial.println("后");
      myData.b=4;
      sendone(1);
      sendone(2);
      while(digitalRead(4) != 1){
        delay(50);
      }
    }
    if (digitalRead(5) != 1 ) {
      Serial.println("左");
      myData.b=2;
      sendone(1);
      sendone(2);
      while(digitalRead(5) != 1){
        delay(50);
      }
    }


  }
  if (mymac == 1) {
      if (typeword == 1) {  //开关
          if (myData.b == 1) {
            analogWrite(12,255);
          }
          if (myData.b == 2) {
            analogWrite(12,255);
          }
          if (myData.b == 3) {
            analogWrite(12,150);
          }
          if (myData.b == 4) {
            analogWrite(12,0);
          }
          if (myData.b == 5) {
            analogWrite(12,200);
          }
        typeword = 0;
      }
  }
  if (mymac == 2) {
    if (typeword == 1) {  //开关
          if (myData.b == 1) {
            analogWrite(12,255);
          }
          if (myData.b == 2) {
            analogWrite(12,150);
          }
          if (myData.b == 3) {
            analogWrite(12,255);
          }
          if (myData.b == 4) {
            analogWrite(12,0);
          }
          if (myData.b == 5) {
            analogWrite(12,200);
          }
        typeword = 0;
      }
  }
  delay(50);
}
