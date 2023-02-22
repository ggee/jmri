
#include <ESP8266WiFi.h>
#include <MQTT.h> // MQTT.h - "MQTT" by Joel Gaehwiler (lwmqtt in Libraries)
#include <TM1637Display.h> // TM1637Display.h - "TM1637" by Avishay Orpaz
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

const char* mqtt_server = "192.168.10.51";
const char* node_id = "3997";
const char* node_name = "node3997";
const char* topicBase = "myLayout";

WiFiClient net;
MQTTClient client;

String myIp;
int hour = 0;
int minute = 0;

#define CLK 5 // Connected to D1 on board
#define DIO 4 // Connected to D2 on board
TM1637Display display(CLK, DIO);

void connect() {

  myIp = WiFi.localIP().toString();
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + myIp);

  Serial.print("\nconnecting to broker " + String(mqtt_server) + "...");
  while (!client.connect(node_name)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  
  client.publish(String(topicBase) + "/clock/" + String(node_id) + "/IPA", myIp + " is here!");
  client.subscribe(String(topicBase) + "/jmri/fastclock/hour");
  client.subscribe(String(topicBase) + "/jmri/fastclock/minute");
}

void setTime() {

  int t = hour * 100 + minute;
  display.showNumberDecEx(t, 0b11100000, true);  
}

void messageReceived(String &topic, String &payload) {

  if (String(topic) == String(topicBase) + "/jmri/fastclock/minute") {
    Serial.println("Minute updating to " + payload);
    minute = payload.toInt();
    setTime();
  }
  if (String(topic) == String(topicBase) + "/jmri/fastclock/hour") {
    Serial.println("Hour updating to " + payload);
    hour = payload.toInt();
    setTime();
  }

}

void setup() {
  Serial.begin(115200);
  // Initialize wifi
  WiFiManager wm;
  if(!wm.autoConnect("AutoConnectAP")) {
    Serial.println("Failed to connect");
    ESP.restart();
  } 
  else {
    Serial.println("WiFi connected");
  }
  Serial.println("After Wifi setup");

  // Set clock LED brightness
  display.setBrightness(0xA);

  // Setup MQTT client settings
  client.begin(mqtt_server, net);
  client.onMessage(messageReceived);

  // Connect to MQTT server
  connect();

}

void loop() {

  // MQTT loop
  client.loop();
  // Check connection to MQTT server
  if (!client.connected()) {
    connect();
  }

}
