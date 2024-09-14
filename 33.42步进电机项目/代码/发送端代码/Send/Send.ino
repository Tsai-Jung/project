//发送
#include <ESP8266WiFi.h>
#include <espnow.h>

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




//发送数据的结构体
typedef struct struct_message {
  String a;
  int b;
} struct_message;

//创建一个新的类型变量
struct_message myData;


//接收方MAC地址 根据自己的板子修改
uint8_t broadcastAddress[2][6] = {
  { 0xE0, 0x98, 0x06, 0xAF, 0x8E, 0x4C },  //电机1
  { 0x80, 0x7D, 0x3A, 0x1A, 0xB6, 0x78 },  //电机2

};



//这是一个回调函数，将在发送消息时执行。
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


int sizelen = 0;  //全部有几块板子
void setup() {
  //初始化串行监视器以进行调试
  Serial.begin(9600);

  //将设备设置为Wi-Fi站点
  WiFi.mode(WIFI_STA);

  //立即初始化ESP
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //设置ESP8266角色  ESP_NOW_ROLE_CONTROLLER， ESP_NOW_ROLE_SLAVE，
  //ESP_NOW_ROLE_COMBO， ESP_NOW_ROLE_MAX。
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  //先前创建的功能。
  esp_now_register_send_cb(OnDataSent);

  //计算板子有几个
  sizelen = sizeof(broadcastAddress) / sizeof(broadcastAddress[0]);
  Serial.print("板子数量有");
  Serial.println(sizelen);

  //与另一个ESP-NOW设备配对以发送数据
  for (int j = 0; j < sizelen; j++) {
    esp_now_add_peer(broadcastAddress[j], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  }

  //打印自己的版子ip
  Serial.print("我的IP地址: ");
  Serial.println(WiFi.macAddress());
}

//发送数据
void sendone(int i) {
  esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
  delay(50);
  int a = 0;  //临时计算重新传送次数
  while (cheakData == "Bad" and a < 10) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(50);
  }
  //Serial.print("已经发送给");
  //Serial.print(i);
  //Serial.println("号");
}

//正常顺序发送
void sendAll() {
  for (int j = 0; j < sizelen; j++) {
    sendone(j);
  }
}
//纵向发送
void sendAll2() {
  for (int i = 1; i < 7; i++) {
    for (int j = 1; j <= 10; j++) {
      int num = i * 10 + j % 10;
      if (num <= sizelen) {
        Serial.print(num);
      }
    }
  }
}

//回字形
void sendaroung(int k){
  int numbers[k];
  for (int i = 0; i < k; i++) {
    numbers[i] = i + 1;
  }
  int outputIndex = 0;
  int currentNumber = 1;
  int step = 1;
  int upperLimit = k / 10;
  int lowerLimit = k / 10 * 9;
  int rowLength = 10;

  while (outputIndex < k) {
    Serial.print(numbers[currentNumber - 1]);
    Serial.print(", ");
    outputIndex++;

    if (currentNumber >= upperLimit && step > 0) {
      step = -1;
      currentNumber += rowLength - 1;
    } else if (currentNumber <= lowerLimit && step < 0) {
      step = 1;
      currentNumber -= rowLength - 1;
    } else {
      currentNumber += step;
    }
  }
  Serial.println();
  delay(1000);
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

String receive() {
  String data;
  if (Serial.available()) {
    delay(10);
    data = (char)Serial.read();
    data += receive();
  }
  return data;
}


void loop() {
  //配置要发送的值
  myData.a = "go";
  myData.b = 1;
  sendAll();
  delay(2000);
}
