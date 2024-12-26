#include <SoftwareSerial.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Pin konfigurasi
const int pHSensorPin = A0;
SoftwareSerial gpsSerial(4, 3); // RX, TX untuk GPS

// WiFi konfigurasi
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// ThingSpeak konfigurasi
const char* server = "api.thingspeak.com";
unsigned long channelID = YOUR_CHANNEL_ID;
const char* apiKey = "YOUR_API_KEY";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  pinMode(pHSensorPin, INPUT);

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Menghubungkan ke ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  float pHValue = readPH();
  String gpsData = readGPS();

  Serial.print("pH Value: ");
  Serial.println(pHValue);
  Serial.print("GPS Data: ");
  Serial.println(gpsData);

  // Mengirim data ke ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, pHValue);
    ThingSpeak.setField(2, gpsData);

    int x = ThingSpeak.writeFields(channelID, apiKey);
    if (x == 200) {
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Failed to send data to ThingSpeak");
    }
  }

  delay(20000); // Delay 20 detik
}

float readPH() {
  int sensorValue = analogRead(pHSensorPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float pH = 7.0 + ((2.5 - voltage) / 0.18); // Kalibrasi sesuai sensor
  return pH;
}

String readGPS() {
  String gpsData = "";
  while (gpsSerial.available()) {
    gpsData += char(gpsSerial.read());
  }
  return gpsData;
}