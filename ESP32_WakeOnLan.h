#ifndef ESP32_WAKEONLAN_H
#define ESP32_WAKEONLAN_H

// enable mail notification
#define USE_MAIL_NOTIFICATION
// enable smart plug shutdown via IFTTT
#define USE_AUTO_SHUTDOWN

// Wi-Fi AP
#define MYSSID   "ssid"
#define PASS     "pass"

// for SendGrid
#ifdef USE_MAIL_NOTIFICATION
#define USER_SENDGRID     "*********"
#define KEY_SENDGRID      "*********"
#define FROM_SENDGRID     "*********@gmail.com"
#define FROMNAME_SENDGRID "ALARM"
#define TO_SENDGRID       FROM_SENDGRID
#define TONAME_SENDGRID   FROMNAME_SENDGRID
#define SUBJECT_SENDGRID01  "PC: Smart Plug ON"
#define TEXT_SENDGRID01     SUBJECT_SENDGRID01
#define SUBJECT_SENDGRID02  "PC: Ping OK, Plug OFF.."
#define TEXT_SENDGRID02     SUBJECT_SENDGRID02
#define SUBJECT_SENDGRID03  "Wake on Lan ERROR"
#define TEXT_SENDGRID03     SUBJECT_SENDGRID03

#define HOSTNAME_SENDGRID "api.sendgrid.com"
#define URI_SENDGRID      "/api/mail.send.json"

#define HEADER_SENDGRID \
  "POST " URI_SENDGRID " HTTP/1.0\r\n" \
  "Content-Type: application/x-www-form-urlencoded\r\n" \
  "Host: " HOSTNAME_SENDGRID "\r\n" \
  "Content-Length: %d\r\n\r\n"

#define BODY_SENDGRID01 \
  "api_user=" USER_SENDGRID      "&" \
  "api_key="  KEY_SENDGRID       "&" \
  "to="       TO_SENDGRID        "&" \
  "toname="   TONAME_SENDGRID    "&" \
  "subject="  SUBJECT_SENDGRID01 "&" \
  "text="     TEXT_SENDGRID01    "&" \
  "from="     FROM_SENDGRID      "&" \
  "fromname=" FROMNAME_SENDGRID

#define BODY_SENDGRID02 \
  "api_user=" USER_SENDGRID      "&" \
  "api_key="  KEY_SENDGRID       "&" \
  "to="       TO_SENDGRID        "&" \
  "toname="   TONAME_SENDGRID    "&" \
  "subject="  SUBJECT_SENDGRID02 "&" \
  "text="     TEXT_SENDGRID02    "&" \
  "from="     FROM_SENDGRID      "&" \
  "fromname=" FROMNAME_SENDGRID

#define BODY_SENDGRID03 \
  "api_user=" USER_SENDGRID      "&" \
  "api_key="  KEY_SENDGRID       "&" \
  "to="       TO_SENDGRID        "&" \
  "toname="   TONAME_SENDGRID    "&" \
  "subject="  SUBJECT_SENDGRID03 "&" \
  "text="     TEXT_SENDGRID03    "&" \
  "from="     FROM_SENDGRID      "&" \
  "fromname=" FROMNAME_SENDGRID 
#endif // USE_MAIL_NOTIFICATION

// for IFTTT
#ifdef USE_AUTO_SHUTDOWN
#define HOSTNAME_IFTTT_WEBHOOK   "maker.ifttt.com"
#define EVENTNAMEE_IFTTT_WEBHOOK "plug01off"
#define MYKEY__IFTTT_WEBHOOK     "*************************"
#define URI_IFTTT_WEBHOOK        "/trigger/" EVENTNAMEE_IFTTT_WEBHOOK "/with/key/" MYKEY__IFTTT_WEBHOOK

#define HEADER_IFTTT_WEBHOOK \
  "POST " URI_IFTTT_WEBHOOK " HTTP/1.0\r\n" \
  "Content-Type: application/json\r\n" \
  "Host: " HOSTNAME_IFTTT_WEBHOOK "\r\n" \
  "Content-Length: %d\r\n\r\n"

#define BODY_IFTTT_WEBHOOK \
  "{\"value1\" : \"\", \"value2\" : \"\", \"value3\" : \"\"}"
#endif // USE_AUTO_SHUTDOWN

#endif // ESP32_WAKEONLAN_H
 
