
// 定义电机控制用常量

// A4988连接Arduino引脚号
const int dirPin = 4;   // 方向引脚
const int stepPin = 5;  // 步进引脚
//const int sleepPin = 2;

// 电机每圈步数
const int STEPS_PER_REV = 200;

void setup() {
  Serial.begin(9600);
  Serial.println("启动了");
  // Arduino控制A4988步进和方向的引脚为输出模式
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  //pinMode(sleepPin,OUTPUT);
  //digitalWrite(sleepPin,HIGH);
  digitalWrite(dirPin, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
}
void loop() {
  // 设置电机顺时针旋转
  digitalWrite(dirPin, LOW);

  // 电机慢速旋转
  for (int x = 0; x < STEPS_PER_REV; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
    if (digitalRead(15) == 1) {
      // 设置电机逆时针旋转
      digitalWrite(dirPin, HIGH);
    }
  }

  // 等待一秒
  delay(1000);

  // 设置电机逆时针旋转
  digitalWrite(dirPin, HIGH);

  // 电机快速旋转
  for (int x = 0; x < (STEPS_PER_REV * 2); x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  // 等待一秒
  delay(1000);
}
