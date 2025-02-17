#include <dht11.h>

#define DHT11PIN 15   // Pin for DHT11 sensor
#define TRIG_PIN 5    // Pin for the Trig pin of the ultrasonic sensor
#define ECHO_PIN 18   // Pin for the Echo pin of the ultrasonic sensor

dht11 DHT11;  // Initialize DHT11 object

void setup() {
  // Start the serial monitor
  Serial.begin(9600);

  // Initialize the Trig pin as output
  pinMode(TRIG_PIN, OUTPUT);

  // Initialize the Echo pin as input
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Send a pulse to trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // Wait for 2 ms
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // Send the pulse for 10 ms
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the echo pulse in microseconds

  // Calculate the distance in cm (Speed of sound = 34300 cm/s)
  long distance = (duration / 2) / 29.1;  // Formula to calculate distance

  // Read temperature and humidity from the DHT11 sensor
  int chk = DHT11.read(DHT11PIN);
  float humidity = (float)DHT11.humidity;
  float temperature = (float)DHT11.temperature;

  // Print the values to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Humidity (%): ");
  Serial.println(humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println(temperature, 2);

  delay(2000); // Wait for 2 seconds before the next reading
}

