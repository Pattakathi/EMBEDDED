#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define TRIG_PIN 14  // GPIO14 (D5)
#define ECHO_PIN 12  // GPIO12 (D6)
#define DHT_PIN 26   // Ensure this pin is valid for your board
#define DHT_TYPE DHT11 // Change to DHT22 if needed

#define BUZZER_PIN 13  // GPIO13 - Buzzer
#define RELAY_PIN 18   // GPIO18 - Relay

#define THRESHOLD 33 // Temperature threshold in °C

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize LCD
#define I2C_ADDR 0x27  
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2); // 16x2 LCD

// Wi-Fi credentials
const char* ssid = "Firmware";
const char* password = "Solutions@12345";

WebServer server(80);

void handleRoot() {
  String html = R"=====(  
  <!DOCTYPE html>
  <html>
  <head>
    <title>TRANSFORMER HEALTH MONITOR</title>
    <style>
      body {
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: linear-gradient(135deg, #1e3c72, #2a5298);
        color: #ffffff;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
      }
      .container {
        background: rgba(0, 0, 0, 0.7);
        padding: 30px;
        border-radius: 15px;
        box-shadow: 0 4px 30px rgba(0, 0, 0, 0.2);
        backdrop-filter: blur(10px);
        border: 1px solid rgba(255, 255, 255, 0.2);
        max-width: 600px;
        width: 100%;
      }
      h1 {
        text-align: center;
        margin-bottom: 20px;
        font-size: 2.5em;
        color: #ffdd57;
      }
      table {
        width: 100%;
        border-collapse: collapse;
        margin: 0 auto;
      }
      th, td {
        padding: 15px;
        text-align: center;
        border: 1px solid rgba(255, 255, 255, 0.2);
      }
      th {
        background-color: rgba(255, 255, 255, 0.1);
        font-size: 1.2em;
      }
      td {
        background-color: rgba(255, 255, 255, 0.05);
        font-size: 1.1em;
      }
      #temp, #oilDistance, #coolerFan {
        font-weight: bold;
        color: #ffdd57;
      }
      .status {
        font-weight: bold;
        color: #ffdd57;
      }
    </style>
    <script>
      async function fetchData() {
        const response = await fetch('/data');
        const data = await response.json();
        document.getElementById('temp').innerText = data.temp + " °C";
        document.getElementById('oilDistance').innerText = data.oilDistance + " cm";
        document.getElementById('coolerFan').innerText = data.coolerFan ? 'ON' : 'OFF';
        document.getElementById('coolerFan').style.color = data.coolerFan ? '#4caf50' : '#f44336';
      }

      setInterval(fetchData, 2000);
      window.onload = fetchData;
    </script>
  </head>
  <body>
    <div class="container">
      <h1>TRANSFORMER HEALTH MONITOR</h1>
      <table>
        <tr>
          <th>TEMP (°C)</th>
          <th>OIL DISTANCE (cm)</th>
          <th>COOLER FAN</th>
        </tr>
        <tr>
          <td id="temp">--</td>
          <td id="oilDistance">--</td>
          <td id="coolerFan" class="status">--</td>
        </tr>
      </table>
    </div>
  </body>
  </html>
  )=====";
  server.send(200, "text/html", html);
}

void handleData() {
  float temp = dht.readTemperature();
  float oilDistance = getOilDistance();
  bool coolerFan = digitalRead(RELAY_PIN);

  if (temp > THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("OVER TEMP ");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }

  String json = "{";
  json += "\"temp\":" + String(temp) + ",";
  json += "\"oilDistance\":" + String(oilDistance) + ",";
  json += "\"coolerFan\":" + String(coolerFan ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

float getOilDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.begin();
  lcd.backlight();

  // Display "TRANS HEALTH" and "IP ADDRESS" for 30 seconds
  lcd.setCursor(0, 0);
  lcd.print("TRANS HEALTH");
  lcd.setCursor(0, 1);
  lcd.print("IP ADDRESS:");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Display IP address on LCD
  lcd.setCursor(0, 1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());

  delay(30000); // Wait for 30 seconds

  // Clear LCD and set up for normal operation
  lcd.clear();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Update LCD display
  float temp = dht.readTemperature();
  float oilDistance = getOilDistance();
  bool coolerFan = digitalRead(RELAY_PIN);

  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.print(temp);
  lcd.print("C");

  lcd.setCursor(10, 0);
  lcd.print("FAN:");
  lcd.print(coolerFan ? "ON " : "OFF");

  lcd.setCursor(0, 1);
  lcd.print("OIL DIST:");
  lcd.print(oilDistance);
  lcd.print("cm");
}
