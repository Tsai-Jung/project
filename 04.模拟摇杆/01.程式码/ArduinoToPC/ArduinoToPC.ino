
void setup() {
  Serial.begin(9600);  // 启动串行通讯
  Serial.println("开机了");
}
String result;
// 创建空白字符串对象
void resultAdd(){
  result += String("LJoystick,"); // 将数字转换为字符串并追加到结果中
  result += String(800);
  result += String(",");
  result += String(1000);
}
// 将文本转换为字符串并追加到结果中
void loop() {
  resultAdd();
  Serial.println(result);
  result="";
  delay(100);
  Serial.println("A");
  delay(100);
  Serial.println("LJoystick,-800,900");
  delay(100);
  Serial.println("LJoystick,300,-500");
  delay(100);
  
}
