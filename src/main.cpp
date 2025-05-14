#include <Arduino.h>
#include <RCSwitch.h>

// 定义LED引脚
const int ledPin = 2;  // ESP32开发板上的板载LED通常连接到GPIO 2
const int receiverPin1 = 27;  // 433MHz接收模块DATA1连接到GPIO 27
const int receiverPin2 = 26;  // 433MHz接收模块DATA2连接到GPIO 26

RCSwitch mySwitch1 = RCSwitch();
RCSwitch mySwitch2 = RCSwitch();

// 用于调试的计时器
unsigned long lastBlinkTime = 0;
const long blinkInterval = 2000;  // 每2秒闪烁一次

void setup() {
  // 初始化串口通信，使用更稳定的设置
  Serial.begin(9600);  // 降低波特率到9600
  delay(2000);  // 等待更长时间让串口稳定
  
  // 清空串口缓冲区
  while(Serial.available()) {
    Serial.read();
  }
  
  Serial.println();
  Serial.println();
  Serial.println("ESP32 433MHz Dual Receiver Test");
  Serial.println("Program started...");
  
  // 设置LED引脚为输出模式
  pinMode(ledPin, OUTPUT);
  
  // 初始化两个433MHz接收模块
  mySwitch1.enableReceive(digitalPinToInterrupt(receiverPin1));
  mySwitch2.enableReceive(digitalPinToInterrupt(receiverPin2));
  Serial.println("433MHz Receivers initialized");
  
  // 显示引脚配置
  Serial.print("Receiver 1 connected to GPIO ");
  Serial.println(receiverPin1);
  Serial.print("Receiver 2 connected to GPIO ");
  Serial.println(receiverPin2);
  
  // 初始LED闪烁
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void loop() {
  // 定期闪烁LED以显示程序正在运行
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = currentMillis;
    digitalWrite(ledPin, HIGH);
    Serial.println("Program is running...");
    delay(50);
    digitalWrite(ledPin, LOW);
  }
  
  // 检查第一个接收器
  if (mySwitch1.available()) {
    digitalWrite(ledPin, HIGH);
    
    Serial.print("Receiver 1 - Received ");
    Serial.print(mySwitch1.getReceivedValue());
    Serial.print(" / ");
    Serial.print(mySwitch1.getReceivedBitlength());
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println(mySwitch1.getReceivedProtocol());
    
    mySwitch1.resetAvailable();
    delay(100);
    digitalWrite(ledPin, LOW);
  }
  
  // 检查第二个接收器
  if (mySwitch2.available()) {
    digitalWrite(ledPin, HIGH);
    
    Serial.print("Receiver 2 - Received ");
    Serial.print(mySwitch2.getReceivedValue());
    Serial.print(" / ");
    Serial.print(mySwitch2.getReceivedBitlength());
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println(mySwitch2.getReceivedProtocol());
    
    mySwitch2.resetAvailable();
    delay(100);
    digitalWrite(ledPin, LOW);
  }
  
  // 短暂延时以减少CPU负载
  delay(10);
}