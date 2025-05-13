#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

// const int analogTemp = 4; // temperature input
int tempData = 1; // temperature data

void setupWifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void setupMqtt() {
  client.setServer(mqtt_server, 1883);
}

void publishData() {
  String pesan = String(tempData);
  client.publish("pubTemp", pesan.c_str());
  Serial.println("temperature sent: " + pesan);
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupMqtt();
  pinMode(34, INPUT);
}

void loop() {
  // Serial.println(tempData);
  // delay(10);
  // tempData = analogRead(34);
  // delay(10);
  // publishData();
  // delay(10); // this speeds up the simulation

  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("connecting MQTT...");
      if (client.connect("ESP32Publisher")) {
        Serial.println("connected to MQTT");
      } else {
        delay(5000);
      }
    }
  }
  tempData = analogRead(34);
  Serial.println(tempData);
  publishData();
  delay(300); 
}
