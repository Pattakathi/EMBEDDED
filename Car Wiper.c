#include <Servo.h>

#define Rain_Analog A0  // Raindrop sensor connected to A0

#define RAIN_THRESHOLD 400  // Threshold for detecting rain

Servo myservo;  // Create a Servo object

int pos = 0;    // Variable to store servo position



void setup() {

  Serial.begin(115200);  // Start serial communication

  myservo.attach(2);  // Connect the servo to D9 (PWM pin)

}



void loop() {

  int rainAnalog = analogRead(Rain_Analog);  // Read raindrop sensor value

  

  Serial.print("Rain Sensor Analog Value: ");

  Serial.println(rainAnalog);  // Print the value to Serial Monitor



  if (rainAnalog <= RAIN_THRESHOLD) {  // If rain detected

    Serial.println("RAIN DROP DETECTED");

    

    // Activate Sweep motion

    for (pos = 0; pos <= 180; pos += 1) {

      myservo.write(pos);  // Move servo to 'pos'

      delay(15);

    }

    for (pos = 180; pos >= 0; pos -= 1) {

      myservo.write(pos);

      delay(15);

    }

  } else {

    Serial.println("NO RAIN DETECTED");

    myservo.write(0);  // Keep servo at 0° (Sweep OFF)

  }



  delay(1000);  // Delay before next reading

}

