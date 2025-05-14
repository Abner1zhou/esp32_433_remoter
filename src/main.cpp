#include <Arduino.h>
#include <RCSwitch.h>

// 定义LED引脚
const int ledPin = 2;  // ESP32开发板上的板载LED通常连接到GPIO 2
const int transmitterPin = 27;  // 433MHz发射模块连接到GPIO 27
const int stopButtonPin = 25;   // 停止按钮连接到GPIO 25
const int startButtonPin = 26;  // 启动按钮连接到GPIO 26

RCSwitch mySwitch = RCSwitch();

// 定义信号值
const unsigned long STOP_SIGNAL = 8543488;   // 停止信号
const unsigned long START_SIGNAL = 8543489;  // 启动信号

// 按钮状态变量
bool lastStopButtonState = HIGH;
bool lastStartButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // 消抖延时

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  delay(2000);
  
  // 清空串口缓冲区
  while(Serial.available()) {
    Serial.read();
  }
  
  Serial.println();
  Serial.println("ESP32 433MHz Transmitter Test");
  Serial.println("Program started...");
  
  // 设置LED引脚为输出模式
  pinMode(ledPin, OUTPUT);
  
  // 设置按钮引脚为输入模式，启用内部上拉电阻
  pinMode(stopButtonPin, INPUT_PULLUP);
  pinMode(startButtonPin, INPUT_PULLUP);
  
  // 初始化433MHz发射模块
  mySwitch.enableTransmit(transmitterPin);
  Serial.println("433MHz Transmitter initialized");
  
  // 显示引脚配置
  Serial.print("Transmitter connected to GPIO ");
  Serial.println(transmitterPin);
  Serial.print("Stop button connected to GPIO ");
  Serial.println(stopButtonPin);
  Serial.print("Start button connected to GPIO ");
  Serial.println(startButtonPin);
  
  // 初始LED闪烁
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void sendSignal(unsigned long signal) {
  digitalWrite(ledPin, HIGH);  // 发送时LED亮起
  mySwitch.send(signal, 24);   // 发送24位信号
  Serial.print("Sending signal: ");
  Serial.println(signal);
  delay(100);
  digitalWrite(ledPin, LOW);   // 发送完成LED熄灭
}

void loop() {
  // 读取按钮状态
  bool currentStopButtonState = digitalRead(stopButtonPin);
  bool currentStartButtonState = digitalRead(startButtonPin);
  
  // 检查停止按钮
  if (currentStopButtonState != lastStopButtonState) {
    if (currentStopButtonState == LOW) {  // 按钮按下
      sendSignal(STOP_SIGNAL);
    }
    lastStopButtonState = currentStopButtonState;
    delay(debounceDelay);  // 消抖延时
  }
  
  // 检查启动按钮
  if (currentStartButtonState != lastStartButtonState) {
    if (currentStartButtonState == LOW) {  // 按钮按下
      sendSignal(START_SIGNAL);
    }
    lastStartButtonState = currentStartButtonState;
    delay(debounceDelay);  // 消抖延时
  }
  
  // 短暂延时以减少CPU负载
  delay(10);
}