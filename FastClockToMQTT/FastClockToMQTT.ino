
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <TM1637Display.h>

const char* ssid = "my_ssid";
const char* password = "my_pwd";
const char* mqtt_server = "192.168.10.2";
const char* node_id = "3997";

WiFiClient net;
MQTTClient client;

String myIp;
int hour = 0;
int minute = 0;

#define CLK 2 //can be any digital pin
#define DIO 3 //can be any digital pin
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
  while (!client.connect("node3997")) {
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

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  display.setBrightness(0xA);

  client.begin(mqtt_server, net);
  client.onMessage(messageReceived);

  connect();

}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

}
