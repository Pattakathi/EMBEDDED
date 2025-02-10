#define TRIG_PIN 5   // GPIO5 is connected to the Trig pin

#define ECHO_PIN 18  // GPIO18 is connected to the Echo pin

#define BUZZER_PIN 19 // GPIO19 is connected to the buzzer



#define THRESHOLD 10 // Threshold distance in cm



void setup() {

  pinMode(TRIG_PIN, OUTPUT);   // Set Trig pin as output

  pinMode(ECHO_PIN, INPUT);    // Set Echo pin as input

  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output

  digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off initially



  Serial.begin(9600); // Initialize serial communication for debugging

}



void loop() {

  long duration;  // Variable to store the duration of the echo

  float distance; // Variable to store the calculated distance



  // Send a 10-microsecond pulse to trigger the ultrasonic sensor

  digitalWrite(TRIG_PIN, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);



  // Measure the duration of the echo signal

  duration = pulseIn(ECHO_PIN, HIGH);



  // Calculate the distance in centimeters

  distance = (duration * 0.0343) / 2;



  // Print the distance to the serial monitor

  Serial.print("Distance: ");

  Serial.print(distance);

  Serial.println(" cm");



  // Check if the distance is less than or equal to the threshold

  if (distance <= THRESHOLD && distance > 0) { // Avoid errors for no signal

    digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer

    Serial.println("Buzzer ON!");

  } else {

    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer

    Serial.println("Buzzer OFF");

  }



  // Delay before the next measurement

  delay(100);

}

