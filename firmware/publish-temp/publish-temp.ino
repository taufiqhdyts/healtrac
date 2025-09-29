// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====

#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// wifi
const char* ssid = "taufiqorbit";         
const char* password = "abcde123"; 

// ===== mqtt setup =====
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 8084; // wss emqx
const char* mqtt_client_id = "ESPClient-DS18B20"; // client id
const char* mqtt_user = ""; // 
const char* mqtt_pass = ""; // 

// ===== mqtt topic =====
const char* mqtt_publish_topic = "patient/vital_data/patient1";

// ===== temp sensor pin =====
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== wifi obj, mqtt obj =====
WiFiClient espClient;
PubSubClient client(espClient);

// ===== connect wifi =====
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ===== reconnect to mqtt broker =====
void reconnect_mqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // client.subscribe("test/topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 
      delay(5000);
    }
  }
}

// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  // stup temperature sensor
  sensors.begin();
  Serial.println("DallasTemperature sensors initialized.");

  // setup wifi
  setup_wifi();

  // setup mqtt server and port
  client.setServer(mqtt_server, mqtt_port);

  Serial.println("Setup complete.");
}

// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====
// ===== MASI BERANTAKAN JGN LANGSUNG MASUKIN CODE SENSOR PPG =====

void loop() {
  // check mqtt connection
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // ===== read temperature =====
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    tempC = 0.0; // if sensor is disconnected
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
  }

  // ===== json payload =====
  StaticJsonDocument<200> doc; // allocate a json document
  doc["patient_id"] = "patient1";
  doc["patient_name"] = "Taufiq Hidayat";
  doc["heart_rate"] = 0; // dummy data
  doc["spo2"] = 0;       // dummy data
  // convert temperature to a string one decimal
  doc["temperature"] = String(tempC, 1);
  doc["timestamp"] = String(millis() / 1000); // unix timestamp (appx)

  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer); // serialize json to a char array

  Serial.print("Publishing message: ");
  Serial.println(jsonBuffer);

  // ===== publish mqtt =====
  if (client.publish(mqtt_publish_topic, jsonBuffer)) {
    Serial.println("Message published successfully.");
  } else {
    Serial.print("Failed to publish message. Client state: ");
    Serial.println(client.state());
  }

  delay(250); // publish every 0.25 seconds
}







