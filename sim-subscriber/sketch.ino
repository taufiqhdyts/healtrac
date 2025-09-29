#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("temperature received: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char msg[length + 1];
  memcpy(msg,payload,length);
  msg[length] = '\0';
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature :");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("connecting MQTT...");
    if (client.connect("ESP32Subscriber")) {
      Serial.println("connected");
      client.subscribe("pubTemp");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupMqtt();
  client.setCallback(callback);

  lcd.init();
  lcd.backlight();

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  delay(300);
}
