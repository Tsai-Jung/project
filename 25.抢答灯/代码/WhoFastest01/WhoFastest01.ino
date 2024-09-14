/*
抢答灯 
1.每个拍拍灯待机5分钟
2.主控控制其他5个灯
3.抢答灯发射给主控
*/

#include "FastLED.h"  // 此示例程序需要使用FastLED库
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>


//13灯     4 5 14 按键   16开机


//灯带的部份
#define NUM_LEDS 12      // LED灯珠数量
#define DATA_PIN 13      // Arduino输出控制信号引脚
#define LED_TYPE WS2812  // LED灯带型号
#define COLOR_ORDER GRB
uint8_t max_bright = 25;  // LED亮度控制变量，可使用数值为 0 ～ 255，数值越大则光带亮度越高
CRGB leds[NUM_LEDS];      // 建立光带leds
void colorsoon(String colorgo) {

  for (int i = 0; i <= NUM_LEDS; i++) {
    if (colorgo == "Red") leds[i] = CRGB::Red;
    else if (colorgo == "Green") leds[i] = CRGB::Green;
    else if (colorgo == "Blue") leds[i] = CRGB::Blue;
    else if (colorgo == "Red") leds[i] = CRGB::Red;
    else if (colorgo == "White") leds[i] = CRGB::White;
    else if (colorgo == "Yellow") leds[i] = CRGB::Yellow;
    else if (colorgo == "Black") leds[i] = CRGB::Black;
    FastLED.show();
    delay(50);
  }
}

void colorshow(String colorgo) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    if (colorgo == "Red") leds[i] = CRGB::Red;
    else if (colorgo == "Green") leds[i] = CRGB::Green;
    else if (colorgo == "Blue") leds[i] = CRGB::Blue;
    else if (colorgo == "Red") leds[i] = CRGB::Red;
    else if (colorgo == "White") leds[i] = CRGB::White;
    else if (colorgo == "Yellow") leds[i] = CRGB::Yellow;
    else if (colorgo == "Black") leds[i] = CRGB::Black;
  }
  FastLED.show();
}


//计时
Ticker ticker;
int countT = 0;
void countime() {
  countT = countT + 1;
}
//接收方MAC地址 根据自己的板子修改

uint8_t broadcastAddress[6][6] = {
  { 0xE8, 0x68, 0xE7, 0x81, 0x6B, 0x6C },  //主控要改
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //拍拍灯1要改
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //拍拍灯2要改
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //拍拍灯3要改
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //拍拍灯4要改
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 }   //拍拍灯5要改
};

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




String worddata;
int typeword = 0;
//创建一个回调函数作为接收数据后的串口显示
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  worddata = myData.a;
  typeword = 1;
  //Serial.print(myData.a);
  //Serial.print(" ");
  //Serial.println(myData.b);
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
    delay(100);
  }
  Serial.print(i);
  Serial.println("号");
}


//测电压部份
int analogValue;
void VAo0() {
  analogValue = analogRead(A0);
  Serial.println(analogValue);
  if (analogValue > 1022) {
    analogValue = 1022;
  }
  delay(100);
}





//IO2闪灯
void ledshow() {
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
}


//引脚设定

void LightMain() {

  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  digitalWrite(2, LOW);
  //Serial.println("灯带初始化");
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, 5);  // 初始化光带
  FastLED.setBrightness(max_bright);
  colorshow("Black");
  VAo0();

  tone(15, 532);  //duo --C3
  delay(100);
  tone(15, 587);  //re --D3
  delay(100);
  tone(15, 659);  //mi --E3
  delay(100);
  tone(15, 0);
  delay(100);

  if (analogValue < 700) {
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
  } else {
    colorshow("Red");
  }
  delay(1000);
  while (digitalRead(12) != 1 or digitalRead(16) != 1, digitalRead(14) != 1 or digitalRead(5) != 1 or digitalRead(4) != 1) {
    delay(20);
  }
  colorshow("Black");
}

void Light() {
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  digitalWrite(2, HIGH);
  digitalWrite(16, HIGH);
  //Serial.println("灯带初始化");
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带
  FastLED.setBrightness(max_bright);
  colorshow("Black");
  VAo0();
  if (analogValue < 700) {
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
    colorsoon("Red");
    delay(100);
    colorsoon("Black");
    delay(100);
  } else {
    if (mymac == 1) colorshow("Red");
    if (mymac == 2) colorshow("Green");
    if (mymac == 3) colorshow("Yellow");
    if (mymac == 4) colorshow("Blue");
    if (mymac == 5) colorshow("White");
  }
  delay(2000);
  while (digitalRead(14) != 1 or digitalRead(5) != 1 or digitalRead(4) != 1) {
    delay(20);
  }
  colorshow("Black");
  //colorsoon("Black");
}



void setup() {
  Serial.begin(9600);  // 启动串行通讯
  espnowinit();
  if (mymac == 0) {
    LightMain();
  } else {
    Light();
  }
}
int showlock = 0;
void LightMainloop() {
  if (digitalRead(4) != 1 and digitalRead(5) != 1) {
    if (showlock == 1) showlock = 0;
    else showlock = 1;
    colorsoon("Green");
    while (digitalRead(4) != 1 and digitalRead(5) != 1) {
      delay(20);
    }
    colorshow("Black");
  }

  if (typeword == 1 and showlock == 1) {
    if (worddata == "Red") {
      colorshow("Red");
    }
    if (worddata == "Green") {
      colorshow("Green");
    }
    if (worddata == "Blue") {
      colorshow("Blue");
    }
    if (worddata == "Yellow") {
      colorshow("Yellow");
    }
    if (worddata == "White") {
      colorshow("White");
    }
    delay(3000);
    colorshow("Black");
    typeword = 0;
  }

  if (digitalRead(4) != 1) {
    for (int i = 0; i <= 5; i++) {
      leds[i] = CRGB::Black;
    }
    leds[3] = CRGB::Red;
    FastLED.show();
    myData.a = "Red";
    myData.b = 0;
    sendone(4);
  }
  if (digitalRead(5) != 1) {
    for (int i = 0; i <= 5; i++) {
      leds[i] = CRGB::Black;
    }
    leds[4] = CRGB::Green;
    FastLED.show();
    myData.a = "Green";
    myData.b = 0;
    sendone(5);
  }
  if (digitalRead(12) != 1) {
    for (int i = 0; i <= 5; i++) {
      leds[i] = CRGB::Black;
    }
    leds[2] = CRGB::Yellow;
    FastLED.show();
    myData.a = "Yellow";
    myData.b = 0;
    sendone(3);
  }
  if (digitalRead(14) != 1) {
    for (int i = 0; i <= 5; i++) {
      leds[i] = CRGB::Black;
    }
    leds[1] = CRGB::Blue;
    FastLED.show();
    myData.a = "Blue";
    myData.b = 0;
    sendone(2);
  }
  if (digitalRead(16) == 0) {
    for (int i = 0; i <= 5; i++) {
      leds[i] = CRGB::Black;
    }
    leds[0] = CRGB::White;
    FastLED.show();
    myData.a = "White";
    myData.b = 0;
    sendone(1);
  }
  delay(200);
}

void Lightloop() {
  if (typeword == 1) {
    if (worddata == "Red") {
      colorshow("Red");
    }
    if (worddata == "Green") {
      colorshow("Green");
    }
    if (worddata == "Blue") {
      colorshow("Blue");
    }
    if (worddata == "Yellow") {
      colorshow("Yellow");
    }
    if (worddata == "White") {
      colorshow("White");
    }
    typeword = 0;
  }
  if (digitalRead(14) != 1 or digitalRead(5) != 1 or digitalRead(4) != 1) {
    myData.a = worddata;
    myData.b = 0;
    sendone(0);
    colorshow("Black");
  }
  delay(200);
}


void loop() {
  if (mymac == 0) {
    LightMainloop();
  } else {
    Lightloop();
  }
}
