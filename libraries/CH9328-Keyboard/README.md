======English ver.======

USB HID Keyboard using WCH's CH9328 chip. It is almost compatible with Arduino Keyboard library.

You need to connect CH9328's IO1 and IO2 to GND.

You can use some Chinese datasheet and Windows configure tools which are in the extra folder, if you want to change the default 9600 baud to another or change some of its USB describtions.

Tested on ESP8266

I wish my English skill is good enough to translate this document.  

=========中文版==========  

在Arduino上使用CH9328 USB转HID键盘芯片，除了初始化外与Arduino Keyboard完全兼容.    
你需要把CH9328的IO1和IO2接地（IO1可选，如果不需要全速USB则不需要接地，IO2接地表示HID透传模式）.  
配置软件和datasheet在extra目录里面，用来修改波特率或描述等参数。(默认波特率为9600).  
在ESP8266上测试通过（Arduino官方板没试过，因为没有。哪位有时间试一下，如果有问题可以在GitHub上提交issue）  
ESP8266的IO9可能与看门狗有关，解决方案：  https://github.com/lxydiy/CH9328-Keyboard/issues/3#issue-1120479945  

作者b站主页：https://space.bilibili.com/213123506   

//GitHub的adoc格式好奇怪，还是用md吧
  
== License ==

Copyright (c) Lxy <1452206376@qq.com>. All right reserved.  
Original code (pre-library): Copyright (c) 2011, Peter Barrett

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
