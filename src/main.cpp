#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <RCSwitch.h>
#include <Blinker.h>
#include "config.h"

// 新建组件对象
BlinkerButton Button1("btn-stop");
BlinkerButton Button2("btn-start");

// 定义LED引脚
const int ledPin = 2;  // ESP32开发板上的板载LED通常连接到GPIO 2
const int transmitterPin = 27;  // 433MHz发射模块连接到GPIO 27

RCSwitch mySwitch = RCSwitch();

// 定义信号值
const unsigned long STOP_SIGNAL = 8543488;   // 停止信号
const unsigned long START_SIGNAL = 8543489;  // 启动信号

bool oState = false;

// 发送信号函数
void sendSignal(unsigned long signal) {
    digitalWrite(ledPin, HIGH);  // 发送时LED亮起
    mySwitch.send(signal, 24);   // 发送24位信号
    Serial.print("Sending signal: ");
    Serial.println(signal);
    delay(100);
    digitalWrite(ledPin, LOW);   // 发送完成LED熄灭
    if (signal == STOP_SIGNAL) {
        BlinkerMIOT.powerState("off");
        oState = false;
    }
    else if (signal == START_SIGNAL) {
        BlinkerMIOT.powerState("on");
        oState = true;
    }
}

// 按钮回调函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if (state == "tap") {
        sendSignal(STOP_SIGNAL);
    }
}

void button2_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if (state == "tap") {
        sendSignal(START_SIGNAL);
    }
}


void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        sendSignal(START_SIGNAL);
        // 使用异步定时器，不会阻塞主循环
        static unsigned long stopTimer = 0;
        stopTimer = millis() + 180000;  // 设置3分钟后的时间点
        
        // 在loop函数中检查时间并发送停止信号
        static bool stopScheduled = true;
        if (stopScheduled && millis() >= stopTimer) {
            sendSignal(STOP_SIGNAL);  // 3分钟后发送停止信号
            stopScheduled = false;
        }
    }
    else if (state == BLINKER_CMD_OFF) {
        sendSignal(STOP_SIGNAL);
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}


void setup() {
    // 初始化串口通信
    Serial.begin(9600);
    delay(2000);
    
    // 初始化Blinker
    Blinker.begin(BLINKER_AUTH, WIFI_SSID, WIFI_PASSWORD);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    
    // 清空串口缓冲区
    while(Serial.available()) {
        Serial.read();
    }
    
    Serial.println();
    Serial.println("ESP32 433MHz Transmitter Test");
    Serial.println("Program started...");
    
    // 设置LED引脚为输出模式
    pinMode(ledPin, OUTPUT);
    
    // 初始化433MHz发射模块
    mySwitch.enableTransmit(transmitterPin);
    Serial.println("433MHz Transmitter initialized");
    
    // 显示引脚配置
    Serial.print("Transmitter connected to GPIO ");
    Serial.println(transmitterPin);

    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    
    // 初始LED闪烁
    for(int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
        delay(100);
    }

    
}

void loop() {
    Blinker.run();
    delay(10);  // 短暂延时以减少CPU负载
}