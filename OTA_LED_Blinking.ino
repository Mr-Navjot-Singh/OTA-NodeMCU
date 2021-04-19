#include <ESP8266WiFi.h>         //provides ESP8266 specific Wi-Fi routines we are calling to connect to network.
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>  // OTA library

#ifndef STASSID
#define STASSID "HP2939"
#define STAPSK  "singhisking2000"
#endif

const char* ssid = STASSID; 
const char* password = STAPSK;

const int blink_led = D0;  // LED pin on NodeMCU ESP8266

void setup() {
  pinMode(blink_led,OUTPUT); 
  
  Serial.begin(115200);           //Set Baud Rate
  Serial.println("Booting");
                                                   // Step to connect ESP with the Wi-Fi
  WiFi.mode(WIFI_STA);               //Set ESP as station mode
  WiFi.begin(ssid, password);      //Wi-Fi Credentials

  while (WiFi.waitForConnectResult() != WL_CONNECTED)      //Connecting  ESP to wi-fi takes some time, so wait till it gets connected
{
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();          //OTA initialization 
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // Display the IP address of the ESP on the serial monitor
}

void loop() {
  ArduinoOTA.handle();

 // code to blink led every 1 second
  digitalWrite(blink_led,HIGH);
  delay(1000);  
  digitalWrite(blink_led,LOW);
  delay(1000);
}
