# ESP32_WakeOnLan

### Wake On Wan the PC by using ESP32 with the Smart Plug.

 [Smart Speaker] or [Meross App] or [IoT Device]
    --->  [Meross Smart Plug Mini] - [ESP32 Board]

### Description

1. The power supply from the smart plug causes the ESP32 to wake up.
1. ESP32 will connect to the WiFi AP.
1. ESP32 send a Magic Packet on UDP.
1. ESP32 will try pinging continuously to target PC.
1. ESP32 will turn off the smart plug via IFTTT when receiving the pong.

### Demonstration

**"OK Google, Turn on the PC."**

[![demo](http://livedoor.blogimg.jp/klab_gijutsu2/imgs/b/8/b8deb115.jpg)](https://www.youtube.com/watch?time_continue=51&v=GjL66kVmaro)

### Blog

http://dsas.blog.klab.org/ (in Japanese)

Thanks for 'ESP32_ping' Library by pbecchi.

https://github.com/pbecchi/ESP32_ping


Copyright (c) 2019 KLab Inc.

