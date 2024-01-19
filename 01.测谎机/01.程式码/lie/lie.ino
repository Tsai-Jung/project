#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>



//灯带的部份
#define NUM_LEDS 5       // LED灯珠数量
#define DATA_PIN 12       // Arduino输出控制信号引脚
#define LED_TYPE WS2812  // LED灯带型号
#define COLOR_ORDER GRB
uint8_t max_bright = 30;  // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
CRGB leds[NUM_LEDS];       // 建立光带leds
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
    delay(100);
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

uint8_t broadcastAddress[2][6] = { { 0x84, 0xF3, 0xEB, 0xBB, 0xBA, 0x8C },    //我的ip
                                   { 0x4C, 0x75, 0x25, 0x08, 0x15, 0xB4 } };  //测试版

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
  while (cheakData == "Bad" and a < 1) {
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
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
}

void setup() {
  Serial.begin(9600);  // 启动串行通讯
  espnowinit();
  if (mymac == 0) {
    pinMode(2, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
    delay(100);
    digitalWrite(2, HIGH);
    digitalWrite(16, HIGH);
    //Serial.println("灯带初始化");
    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带
    FastLED.setBrightness(max_bright);
    colorshow("Black");
    colorsoon("Yellow");
    tone(15, 532);  //duo --C3
    delay(100);
    tone(15, 587);  //re --D3
    delay(100);
    tone(15, 659);  //mi --E3
    delay(100);
    tone(15, 0);
    delay(100);
  }
  if (mymac == 1) {
    /*
    Serial.end();
    pinMode(2, OUTPUT);
    ledshow();
    Keyboard.begin(&Serial, 10, 9600);
    delay(100);
    Keyboard.releaseAll();
    */
  }
  ticker.attach(1, countime);
}
int b = 0;

void loop() {
  if (mymac == 0) {
    VAo0();
    if (analogValue > 550){
      colorshow("Yellow");
    } 
    if (analogValue < 550) {
      colorshow("Green");
      if (abs(analogValue - b) < 100 and abs(analogValue - b) > 3) {
        tone(15, 659);  //mi --E3
        colorshow("Red");
        delay(100);
        tone(15, 0);
      }
      b = analogValue;
      countT = 0;
    }
    //myData.a = "space";
    //myData.b = analogValue;
    //sendone(1);
    if (countT > 10) {
      colorsoon("Black");
      digitalWrite(16, LOW);
    }
  }
  if (mymac == 1) {
  }
  delay(100);
}
