/*
 * EMailSender library for Arduino, Arduino SAMD boards (WiFiNINA), esp8266 and esp32
 * Simple Arduino SAMD boards (WiFiNINA) Gmail send attachments
 * to a single email
 *
 * The base64 encoding of the image is slow, so be patient
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <EMailSender.h>
#include <WiFi.h>

WiFiClient optaClient;

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

const char* ssid = "Can_Saguer_Wifi_EXT";
const char* password = "cansaguer2";

//EMailSender emailSend("smtp.testgizantech@gmail.com", "GizAnTech4567");
EMailSender emailSend("testgizantech@gmail.com", "GizAnTech4567", "Test", "smtp.gmail.com", 465);

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50) {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits() {
  uint32_t ts = millis();
  while (!connection_state) {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Started");

  connection_state = WiFiConnect(ssid, password);
  if (!connection_state)  // if not connected to WIFI
    Awaits();             // constantly trying to connect

  EMailSender::EMailMessage message;
  message.subject = "Soggetto";
  message.message = "gizantech.com";


  Serial.println("Sending Mail....");
  EMailSender::Response resp = emailSend.send("md.masrursaqib@gmail.com", message);

  Serial.println("Sending status: ");

  Serial.println(resp.status);
  Serial.println(resp.code);
  Serial.println(resp.desc);
}

void loop() {
  EMailSender::EMailMessage message;
  message.subject = "Soggetto";
  message.message = "gizantech.com";


  Serial.println("Sending Mail....");
  EMailSender::Response resp = emailSend.send("md.masrursaqib@gmail.com", message);

  Serial.println("Sending status: ");

  Serial.println(resp.status);
  Serial.println(resp.code);
  Serial.println(resp.desc);
  delay(20000);
}