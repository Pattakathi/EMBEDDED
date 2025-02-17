#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT.h"

// DHT11 sensor setup
#define DHT11_PIN 13
DHT dht11(DHT11_PIN, DHT11);

// Pulse Oximeter setup
PulseOximeter pox;
uint32_t tsLastReport = 0;
uint32_t lastDHTRead = 0;

// Callback function for pulse detection
void onBeatDetected() {
  Serial.println("Beat detected!");
}

void setup() {
  // Start Serial Communication
  Serial.begin(9600);

  // Initialize DHT11 sensor
  dht11.begin(); 
  Serial.println("DHT11 Sensor Initialized.");

  // Initialize Pulse Oximeter
  if (!pox.begin()) {
    Serial.println("FAILED to initialize MAX30100. Check your wiring!");
    while (1);
  } else {
    Serial.println("MAX30100 Initialized.");
  }

  // Set up callback for pulse detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

  // Set the IR LED current (adjust for better performance if needed)
  pox.setIRLedCurrent(MAX30100_LED_CURR_50MA); // Options: 24MA, 27MA, 50MA, etc.
}

void loop() {
  // Read DHT11 data every 2 seconds
  if (millis() - lastDHTRead > 2000) {
    lastDHTRead = millis();
    float humi = dht11.readHumidity();
    float tempC = dht11.readTemperature();
    float tempF = dht11.readTemperature(true);

    if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } else {
      Serial.print("DHT11 Humidity: ");
      Serial.print(humi);
      Serial.print("%");
      Serial.print(" | Temperature: ");
      Serial.print(tempC);
      Serial.print("°C ~ ");
      Serial.print(tempF);
      Serial.println("°F");
    }
  }

  // Update Pulse Oximeter readings
  pox.update();

  // Report Pulse Oximeter data every 1 second
  if (millis() - tsLastReport > 1000) {
    tsLastReport = millis();
    float heartRate = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    if (isnan(heartRate) || isnan(spo2)) {
      Serial.println("Failed to read SpO2 or Heart Rate!");
    } else {
      Serial.print("Heart Rate: ");
      Serial.print(heartRate);
      Serial.print(" BPM, SpO2: ");
      Serial.print(spo2);
      Serial.println(" %");
    }
  }
}
