#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu; // Declare the MPU6050 object

// Define I2C pins
#define SDA_PIN D1 // Replace with your SDA pin
#define SCL_PIN D2 // Replace with your SCL pin

float alpha = 0.98; // Complementary filter coefficient
float dt = 0.01; // Sampling time in seconds
float filtered_roll = 0, filtered_pitch = 0;

void setup() {
  Serial.begin(115200); // Initialize serial communication

  // Initialize I2C communication with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  // Configure MPU6050 settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp; // Declare sensor event objects
  mpu.getEvent(&a, &g, &temp); // Read sensor data

  // Calculate roll and pitch from accelerometer
  float roll_acc = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float pitch_acc = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;

  // Integrate gyroscope data using complementary filter
  filtered_roll = alpha * (filtered_roll + g.gyro.x * dt) + (1 - alpha) * roll_acc;
  filtered_pitch = alpha * (filtered_pitch + g.gyro.y * dt) + (1 - alpha) * pitch_acc;

  // Send data to Processing
  Serial.print(filtered_roll);
  Serial.print(",");
  Serial.println(filtered_pitch);

  delay(10); // Adjust delay based on your sampling rate
}
