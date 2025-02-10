#include <Wire.h>

#include <LiquidCrystal_I2C.h>



// Define the pins for DOWN TANK (Ultrasonic Sensor 1)

const int trigPin1 = 5;

const int echoPin1 = 4;



// Define the pins for TOP TANK (Ultrasonic Sensor 2)

const int trigPin2 = 7;

const int echoPin2 = 6;



// Define the pin for the relay (Motor)

const int relayPin = 12;



// Define the pin for the buzzer

const int buzzerPin = 13;



// Initialize the I2C LCD (I2C address is typically 0x27 for many LCDs)

LiquidCrystal_I2C lcd(0x27, 16, 2);



// Variables to store the distance

long duration1, distance1;

long duration2, distance2;



void setup() {

  // Start serial communication

  Serial.begin(9600);



  // Set the pins as outputs and inputs for ultrasonic sensors

  pinMode(trigPin1, OUTPUT);

  pinMode(echoPin1, INPUT);



  pinMode(trigPin2, OUTPUT);

  pinMode(echoPin2, INPUT);



  // Set the relay pin as an output

  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, LOW); // Start with the relay OFF



  // Set the buzzer pin as an output

  pinMode(buzzerPin, OUTPUT);

  digitalWrite(buzzerPin, LOW); // Ensure the buzzer is OFF initially



  // Initialize the LCD

  lcd.begin(16, 2); // 16 columns, 2 rows

  lcd.print("DOWN TANK: ");

  lcd.setCursor(0, 1);

  lcd.print("TOP TANK: ");

}



void loop() {

  // DOWN TANK (Sensor 1): Trigger the pulse and measure the duration

  digitalWrite(trigPin1, LOW); // Ensure the trigger pin is LOW before sending pulse

  delayMicroseconds(2);

  digitalWrite(trigPin1, HIGH); // Send a pulse

  delayMicroseconds(10);

  digitalWrite(trigPin1, LOW); // End pulse



  // Measure the time it takes for the echo to return

  duration1 = pulseIn(echoPin1, HIGH);



  // Calculate the distance in cm (Speed of sound = 34300 cm/s)

  distance1 = (duration1 / 2) * 0.0343;



  // TOP TANK (Sensor 2): Trigger the pulse and measure the duration

  digitalWrite(trigPin2, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin2, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin2, LOW);



  // Measure the time for echo return

  duration2 = pulseIn(echoPin2, HIGH);



  // Calculate the distance in cm

  distance2 = (duration2 / 2) * 0.0343;



  // Display the distance for DOWN TANK (T1)

  lcd.setCursor(12, 0); // Move cursor to position 12 on the first line

  lcd.print(distance1);

  lcd.print("cm "); // Add a space to overwrite any leftover characters



  // Display the distance for TOP TANK (T2)

  lcd.setCursor(12, 1); // Move cursor to position 12 on the second line

  lcd.print(distance2);

  lcd.print("cm "); // Add a space to overwrite any leftover characters



  // Control motor based on DOWN TANK (T1) distance

  if (distance1 > 10 && distance1 < 1000) { // If T1 is between 10 cm and 1000 cm

    digitalWrite(relayPin, HIGH); // Turn relay ON

    Serial.println("MOTOR turned ON (DOWN TANK is between 10cm and 1000cm)");

    lcd.setCursor(0, 0);

    lcd.print("MOTOR: ON "); // Display motor status

  } else { // If T1 is not in range

    digitalWrite(relayPin, LOW); // Turn relay OFF

    Serial.println("MOTOR turned OFF (DOWN TANK is out of range)");

    lcd.setCursor(0, 0);

    lcd.print("MOTOR: OFF"); // Display motor status

  }



  // Control motor based on TOP TANK (T2) distance

  if (distance2 < 10) { // If T2 is less than 10 cm

    digitalWrite(relayPin, LOW); // Turn relay OFF

    Serial.println("MOTOR turned OFF (TOP TANK is less than 10cm)");

    lcd.setCursor(0, 1);

    lcd.print("MOTOR: OFF "); // Display motor status

  } else { // If T2 is not less than 10 cm

    digitalWrite(relayPin, HIGH); // Turn relay ON

    Serial.println("MOTOR turned ON (TOP TANK is greater than or equal to 10cm)");

    lcd.setCursor(0, 1);

    lcd.print("MOTOR: ON "); // Display motor status

  }



  // If either T1 or T2 is out of the valid range, turn on the buzzer for 5 seconds

  if (distance1 <= 10 || distance1 >= 1000 || distance2 < 10) {

    digitalWrite(buzzerPin, HIGH); // Turn buzzer ON

    delay(5000); // Buzzer stays ON for 5 seconds

    digitalWrite(buzzerPin, LOW); // Turn buzzer OFF

  }



  // Delay before the next reading

  delay(1000); // Wait for 1 second before the next measurement

}



