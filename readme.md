### 说明

1、原由

本项目是[kid-blockly]的一个附属项目，用于在web和pico之间，搭建一个websocket到uart的透传模块。因为市面上大部分都是基于socket或者mqtt的透传，没有办法兼容websocket，自己实现一个可用的版本更方便；

2、硬件

我使用是一个安信可的esp-01s模块，但应该很容易就套用于其他的模块；

3、平台




### 使用

1、安装arduino，可以在这里下载：[arduino IDE](https://downloads.arduino.cc/arduino-1.8.15-windows.zip)，直接解压即可，绿色版；


2、安装esp8266的开发板支持，参考[这里](https://arduino-esp8266.readthedocs.io/en/latest/installing.html#boards-manager)，这会下载esp8266有关开发板的描述文件，以及sdk；

3、安装依赖的库，在arduino的工具栏下面有库管理，打开后可以在线的安装第三方库，当前我们需要的有：

- [ArduinoWebsockets](https://github.com/gilmaimon/ArduinoWebsockets), 用于实现websocket的server和client；

4、下载代码，编译，下载；