#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>

//舵机
#include <Servo.h>


//舵机部份
Servo myservo;
int servonow = 90;
int servoOn = 0;
void servogo(int angle) {
  myservo.attach(14, 500, 2500);  //舵机激活设定脚位4和修正脉冲宽度
  if (angle > 180) angle = 180;
  else if (angle < 0) angle = 0;
  myservo.write(angle);  //舵机角度控制0~180
}


//计时
Ticker ticker;
int countT = 0;
void countime() {
  countT = countT + 1;
}
//接收方MAC地址 根据自己的板子修改

uint8_t broadcastAddress[3][6] = { { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //振动器
  { 0x98, 0xCD, 0xAC, 0x23, 0xD6, 0xDE },
  { 0xA8, 0x48, 0xFA, 0xFF, 0x75, 0xAC }
};  //舵机

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

int movemotor = 0;
//创建一个回调函数作为接收数据后的串口显示
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.a);
  if (myData.a == "Go") {
    movemotor = 1;
  }
  if (myData.a == "Back") {
    movemotor = 2;
  }
  if (myData.a == "Stop") {
    movemotor = 0;
  }
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
  Serial.print("板子数量");
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
    }
    if (macnum != WiFi.macAddress()) {
      //与另一个ESP-NOW设备配对以发送数据
      esp_now_add_peer(broadcastAddress[j], ESP_NOW_ROLE_COMBO, 1, NULL, 0);
      Serial.print("配对的板子");
      Serial.println(macnum);
    }
  }

  //先前创建的功能。
  esp_now_register_send_cb(OnDataSent);

  //先前创建的功能 测试ESP-NOW通信
  esp_now_register_recv_cb(OnDataRecv);
}


//发送数据

void sendall() {
  for (int j = 0; j < sizelen; j++) {
    //与另一个ESP-NOW设备配对以发送数据
    if (mymac != j) {
      esp_now_send(broadcastAddress[j], (uint8_t *)&myData, sizeof(myData));
      Serial.println("发送全部");
    }
  }
}

void sendone(int i) {
  esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
  delay(100);
  int a = 0;
  while (cheakData == "Bad" and a < 10) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(100);
  }
  Serial.print(i);
  Serial.println("号");
}






void setup() {
  Serial.begin(9600);  // 启动串行通讯
  delay(100);          // 稳定性等待
  espnowinit();
  delay(100);
  ticker.attach(1, countime);
  if (mymac == 0) {
    pinMode(12, OUTPUT);
    pinMode(13, INPUT_PULLUP);
    digitalWrite(12, LOW);
  }
  if (mymac == 1) {
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
  }
  if (mymac == 2) {
    pinMode(12, OUTPUT);
    pinMode(13, INPUT_PULLUP);
    digitalWrite(12, LOW);
  }
  if (mymac == 99) {  //未绑定
    pinMode(2, OUTPUT);
    myservo.attach(14, 500, 2500);  //舵机激活设定脚位4和修正脉冲宽度
    myservo.write(70);              //舵机角度控制0~180
  }
}

void loop() {
  if (mymac == 0) {
    if (digitalRead(13) != 1) {
      digitalWrite(12, HIGH);
      myData.a = "Back";
      sendone(1);
      while (digitalRead(13) != 1) {
        delay(100);
      }
      digitalWrite(12, LOW);
    }
  }
  if (mymac == 1) {
    if (movemotor == 1) {
      servonow = servonow + 20;
      movemotor = 0;
    }
    if (movemotor == 2) {
      servonow = servonow - 20;
      movemotor = 0;
    }
    if (servonow > 180) servonow = 180;
    if (servonow < 0) servonow = 0;
    servogo(servonow);
  }
  if (mymac == 2) {
    if (digitalRead(13) != 1) {
      digitalWrite(12, HIGH);
      myData.a = "Go";
      sendone(1);
      while (digitalRead(13) != 1) {
        delay(100);
      }
      digitalWrite(12, LOW);
    }
  }
  if (mymac == 99) {
    if (digitalRead(0) != 1) {
      digitalWrite(2, LOW);
      delay(3000);
      myservo.write(130);
      delay(2000);
      myservo.write(70);
      delay(1000);
    }


  }
}
