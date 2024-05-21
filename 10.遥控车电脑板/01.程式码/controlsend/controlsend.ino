#include <CH9328Keyboard.h>
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


uint8_t broadcastAddress[2][6] = { { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },    //电脑接收器
                                   { 0x68, 0xC6, 0x3A, 0xFC, 0x33, 0x0C } };  //遥控器

//接收数据保存的结构体和发送方一致
typedef struct struct_message {
  String a;
  int f;
  int b;
  int l;
  int r;
} struct_message;

//创建结构体变量
struct_message myData;


//接收和发送
//在这种情况下，无论是否成功发送该消息，都会简单地打印出来
String cheakData = "good";
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    //Serial.println("Delivery success");
    cheakData = "good";
  } else {
    //Serial.println("Delivery fail");
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
  while (cheakData == "Bad" and a < 3) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(100);
  }
  //Serial.print(i);
  //Serial.println("号");
}


//IO2闪灯
void ledshow() {
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
}


#define PINRST 10
#define BAUDRATE 9600              //Default is 9600.

void setup() {
  Serial.begin(9600);  // 启动串行通讯
  delay(100);
  espnowinit();
  delay(100);
  if (mymac == 0) {
    Serial.end();
    pinMode(2, OUTPUT);
    Keyboard.begin(&Serial, PINRST, BAUDRATE);
    delay(100);
    Keyboard.releaseAll();
  }
  if (mymac == 1) {
    
    pinMode(12, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  }
  ticker.attach(1, countime);
}


int have=0;
void loop() {
  if (mymac == 0) {
    if (typeword == 1) {
      if (myData.f == 1) {
        Keyboard.press('w');
      }
      if (myData.b == 1) {
        Keyboard.press('s');
      }
      if (myData.l == 1) {
        Keyboard.press('a');
      }
      if (myData.r == 1) {
        Keyboard.press('d');
      }
      if (myData.l == 0) {
        Keyboard.release('a');
      }
      if (myData.r == 0) {
        Keyboard.release('d');
      }
      if (myData.f == 0) {
        Keyboard.release('w');
      }
      if (myData.b == 0) {
        Keyboard.release('s');
      }
      typeword = 0;
    }
    delay(10);
  }
  if (mymac == 1) {
    if (digitalRead(14) != 1 ) {
      //Serial.println("F");
      myData.f=1;
      have=0;
    }
    if (digitalRead(14) == 1 ) {
      myData.f=0;
      have=0;
    }
    if (digitalRead(12) != 1 ) {
      //Serial.println("B");
      myData.b=1;
      have=0;
    }
    if (digitalRead(12) == 1 ) {
      myData.b=0;
      have=0;
    }
    if (digitalRead(5) != 1 ) {
      //Serial.println("L");
      myData.l=1;
      have=0;
    }
    if (digitalRead(5) == 1 ) {
      myData.l=0;
      have=0;
    }
    if (digitalRead(4) != 1 ) {
      //Serial.println("R");
      myData.r=1;
      have=0;
    }
    if (digitalRead(4) == 1 ) {
      myData.r=0;
      have=0;
    }
    if(digitalRead(4) == 1 and digitalRead(5) == 1 and digitalRead(12) == 1 and digitalRead(14) == 1 and have==0){
      myData.f=0;
      myData.b=0;
      myData.r=0;
      myData.l=0;
      have=1;
    }
    sendone(0);
    delay(50);
  }
  
}
