/*
  ESP32_WakeOnLan.ino  (for ESP32)

  Copyright (c) 2019 KLab Inc.

  Released under the MIT license
  https://opensource.org/licenses/mit-license.php

  Wake On Wan the PC by using ESP32 with the Smart Plug.

   [Smart Speaker] or [Meross App] or [IoT Device]
    --->  [Meross Smart Plug Mini] - [ESP32 Board]

  Description

  1. The power supply from the smart plug causes the ESP32 to wake up.
  2. ESP32 will connect to the WiFi AP.
  3. ESP32 send a Magic Packet on UDP.
  4. ESP32 will try pinging continuously to target PC.
  5. ESP32 will turn off the smart plug via IFTTT when receiving the pong.

  Thanks for 'ESP32_ping' Library by pbecchi.
  https://github.com/pbecchi/ESP32_ping

 */

#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include "ESP32_WakeOnLan.h"
#include "ping.h"

#define MAGIC_PACKET_LENGTH 102
#define PORT_WAKEONLAN 9

WiFiClientSecure client;
WiFiUDP udp;

// UDP broadcast address
IPAddress bcastAddr(192, 168, 0, 255);
// Target PC
IPAddress targetIpAddr(192,168,0,5);
byte macAddr[6] = {0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee};

byte magicPacket[MAGIC_PACKET_LENGTH];
unsigned int localPort = 8761;
boolean mpSent = false;
unsigned long stime;
unsigned long TIMEOUT = 180000; // 180sec

void setup() {
  Serial.begin(115200);
  stime = millis();
}

void loop() {
  wl_status_t wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED) {
    dbg("loop: WiFi.status=%d\r\n", wifiStatus);
    if (connectWiFi()) {
      stime = millis();
      sendMagicPacket();
    }
  }
  unsigned long past = millis() - stime;
  dbg("past=%lu\r\n", past);
  if (past > TIMEOUT) {
    dbg("timeout..\r\n");
    if (wifiStatus == WL_CONNECTED) {
#ifdef USE_MAIL_NOTIFICATION
      doPost(HOSTNAME_SENDGRID, 443, HEADER_SENDGRID, BODY_SENDGRID03);
#endif
#ifdef USE_AUTO_SHUTDOWN
      // Shutdown the plug via IFTTT
      doPost(HOSTNAME_IFTTT_WEBHOOK, 443, HEADER_IFTTT_WEBHOOK, BODY_IFTTT_WEBHOOK);
#endif
    }
    esp_deep_sleep_start();
  } else if (mpSent) {
    if (doPing(targetIpAddr) == true) { // ping OK
#ifdef USE_MAIL_NOTIFICATION
      doPost(HOSTNAME_SENDGRID, 443, HEADER_SENDGRID, BODY_SENDGRID02);
#endif
#ifdef USE_AUTO_SHUTDOWN
      doPost(HOSTNAME_IFTTT_WEBHOOK, 443, HEADER_IFTTT_WEBHOOK, BODY_IFTTT_WEBHOOK);
#endif
      esp_deep_sleep_start();
    }
  }
  delay(1000);
}

boolean connectWiFi() {
  int count = 0; 
  dbg("connectWiFi: connecting to AP");
  WiFi.mode(WIFI_STA);
  WiFi.begin(MYSSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    dbg(".");
    if (count++ > 10) {
      return false;
    }
  }
  dbg("\r\nWiFi connected\r\n");
  dbg("connectWiFi: IP address: %s\r\n", WiFi.localIP().toString().c_str());
#ifdef USE_MAIL_NOTIFICATION
  doPost(HOSTNAME_SENDGRID, 443, HEADER_SENDGRID, BODY_SENDGRID01);
#endif
  return true;
}

boolean sendMagicPacket() {
  if (udp.begin(localPort) == 1) {
    dbg("udp begin OK\r\n");
    buildMagicPacket();
    udp.beginPacket(bcastAddr, PORT_WAKEONLAN);
    udp.write(magicPacket, MAGIC_PACKET_LENGTH);
    udp.endPacket();
    mpSent = true;
    return true;
  }
  dbg("udp begin NG\r\n");
#ifdef USE_MAIL_NOTIFICATION
  doPost(HOSTNAME_SENDGRID, 443, HEADER_SENDGRID, BODY_SENDGRID03);
#endif
#ifdef USE_AUTO_SHUTDOWN
  // smart plug off
  doPost(HOSTNAME_IFTTT_WEBHOOK, 443, HEADER_IFTTT_WEBHOOK, BODY_IFTTT_WEBHOOK);
#endif
  esp_deep_sleep_start();
  return false; // never comes
}

int doPost(const char *host, uint16_t port, const char *header, const char *body) {
  char buf[512];
  dbg("connecting to %s:%u\r\n", host, port);
  if (!client.connect(host, port)) {
    dbg("connect error\r\n");
    return -1;
  }
  dbg("connected!\r\n");
  snprintf(buf, sizeof(buf), header, strlen(body));
  client.print(buf);
  client.print(body);
  // timeout
  unsigned long now = millis();
  while (client.available() == 0) {
    if (millis() - now > 10000) {
      dbg("ERR: reqest timeout\r\n");
      client.stop();
      return -2;
    }
  }
  while (client.available()) {
    char c = client.read();
    dbg("%c", c);
  }
  dbg("closing..\r\n");
  client.stop();
}

bool doPing(IPAddress ia) {
  IPAddress adr = IPAddress(ia[0], ia[1], ia[2], ia[3]);
  dbg("Ping : %d.%d.%d.%d : ", ia[0], ia[1], ia[2], ia[3]);
  if (ping_start(adr, 4, 0, 0, 5)) {
    dbg("OK\r\n");
    return true;
  } else {
    dbg("Request time out\r\n");
    return false;
  }
}

void buildMagicPacket() {
  memset(magicPacket, 0xFF, 6);
  for (int i = 0; i < 16; i++) {
    int ofs = i * sizeof(macAddr) + 6;
    memcpy(&magicPacket[ofs], macAddr, sizeof(macAddr));
  }
}

void dbg(const char *format, ...) {
  //return;
  char b[512];
  va_list va;
  va_start(va, format);
  vsnprintf(b, sizeof(b), format, va);
  va_end(va);
  Serial.print(b);
  Serial.flush();  
}
