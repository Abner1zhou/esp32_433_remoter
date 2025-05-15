# ESP32 433MHz 遥控发射器

这是一个基于ESP32的433MHz遥控发射器项目，通过Blinker物联网平台实现远程控制功能。该项目可以将ESP32开发板转换为一个可以通过手机APP远程控制的433MHz遥控发射器。

## 功能特点

- 通过Blinker APP远程控制
- 支持发送自定义433MHz信号
- 内置LED状态指示
- 支持启动和停止两种控制信号
- 使用WiFi连接，无需额外网关

## 硬件要求

- ESP32开发板
- 433MHz发射模块
- USB数据线
- 面包板和连接线（可选）

## 接线说明

- 433MHz发射模块 VCC -> ESP32 3.3V
- 433MHz发射模块 GND -> ESP32 GND
- 433MHz发射模块 DATA -> ESP32 GPIO 27
- ESP32板载LED -> GPIO 2（默认）

## 软件依赖

- PlatformIO IDE
- Arduino框架
- 依赖库：
  - RCSwitch
  - Blinker-iot

## 配置说明

在使用前，需要在`config.h`文件中配置以下信息：

```cpp
#define BLINKER_AUTH "你的Blinker密钥"
#define WIFI_SSID "你的WiFi名称"
#define WIFI_PASSWORD "你的WiFi密码"
```

## 使用方法

1. 使用PlatformIO打开项目
2. 配置`config.h`文件中的WiFi和Blinker信息
3. 编译并上传代码到ESP32
4. 打开Blinker APP，添加设备并绑定
5. 在APP中使用按钮控制发射信号

## 信号说明

- 停止信号：8543488
- 启动信号：8543489

## 注意事项

- 确保ESP32和手机在同一WiFi网络下
- 发射模块需要正确连接到ESP32的GPIO 27引脚
- 首次使用时需要配置Blinker APP并绑定设备

## 许可证

MIT License
