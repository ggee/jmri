
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TM1637Display.h>
#include <ArduinoOTA.h>

// Arduino Libraries to load
// MQTT.h - "MQTT" by Joel Gaehwiler
// TM1637Display.h - "TM1637" by Avishay Orpaz

const char* ssid = "my_ssid";
const char* password = "my_pwd";
const char* mqtt_server = "192.168.10.51";
const char* node_id = "3997";
const char* node_name = "node3997";

WiFiClient net;
MQTTClient client;

String myIp;
int hour = 0;
int minute = 0;

#define CLK 5 // Connected to D1 on board
#define DIO 4 // Connected to D2 on board
TM1637Display display(CLK, DIO);

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  myIp = WiFi.localIP().toString();
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + myIp);

  Serial.print("\nconnecting to broker " + String(mqtt_server) + "...");
  while (!client.connect(node_name)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  
  client.publish("myLayout/clock/" + String(node_id) + "/IPA", myIp + " is here!");
  client.subscribe("myLayout/jmri/fastclock/hour");
  client.subscribe("myLayout/jmri/fastclock/minute");
}

void setTime() {

  int t = hour * 100 + minute;
  display.showNumberDecEx(t, 0b11100000, true);  
}

void messageReceived(String &topic, String &payload) {

  if (String(topic) == "myLayout/jmri/fastclock/minute") {
    Serial.println("Minute updating to " + payload);
    minute = payload.toInt();
    setTime();
  }
  if (String(topic) == "myLayout/jmri/fastclock/hour") {
    Serial.println("Hour updating to " + payload);
    hour = payload.toInt();
    setTime();
  }

}

void setup_ota() {
    // Initialize ATO
  ArduinoOTA.setHostname(node_name);
  ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
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
  ArduinoOTA.begin();

}

void setup() {
  Serial.begin(115200);
  // Initialize wifi
  WiFi.begin(ssid, password);

  // Set clock LED brightness
  display.setBrightness(0xA);

  // Setup MQTT client settings
  client.begin(mqtt_server, net);
  client.onMessage(messageReceived);

  // Connect to wifi and MQTT server
  connect();

  // Initialize OTA
  setup_ota();

}

void loop() {

  // OTA loop
  ArduinoOTA.handle();
  // MQTT loop
  client.loop();
  // Check connection to MQTT server
  if (!client.connected()) {
    connect();
  }

}
