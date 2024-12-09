#include <LiquidCrystal.h>

// Pin connections for the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin definitions
const int trig = 9;         // Trigger pin for ultrasonic sensor
const int echo = 8;         // Echo pin for ultrasonic sensor
const int motorPin1 = 10;   // Motor control pin 1
const int motorPin2 = 7;    // Motor control pin 2
const int led1 = 13;        // Green LED to indicate motor state

// Variables to store sensor readings and distance
long duration = 0;          // Time duration for echo pulse
int distance = 0;           // Calculated distance in centimeters
String lastMessage = "";    // To track the last displayed message on the LCD

void setup() {
    // Initialize the LCD
    lcd.begin(16, 2);

    // Set up the pins as input or output
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(led1, OUTPUT);
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);

    // Start serial communication for debugging
    Serial.begin(9600);
}

void loop() {
    // Trigger the ultrasonic sensor
    digitalWrite(trig, LOW);         // Ensure trigger pin is low
    delayMicroseconds(2);            // Small delay before the pulse
    digitalWrite(trig, HIGH);        // Send a 10-microsecond pulse
    delayMicroseconds(10);           // Pulse duration
    digitalWrite(trig, LOW);         // End the pulse

    // Measure the duration of the echo pulse
    duration = pulseIn(echo, HIGH);  // Read the time for the echo signal
    distance = (duration / 2) / 29.1; // Convert duration to distance in cm

    // Check the distance and control the motor and LED accordingly
    if (distance > 0 && distance < 20) {  // Valid distance check and obstacle within 20 cm
        if (lastMessage != "MOTOR OFF") {
            lcd.clear();                  // Clear LCD screen
            lcd.print("MOTOR OFF");       // Display "MOTOR OFF"
            lastMessage = "MOTOR OFF";    // Update the last message
        }
        digitalWrite(motorPin1, LOW);     // Turn off the motor
        digitalWrite(motorPin2, LOW);
        digitalWrite(led1, HIGH);         // Turn on the green LED
        Serial.println(String(distance) + " cm MOTOR STOP"); // Debug message
    } else if (distance >= 20) {  // No obstacle or safe distance
        if (lastMessage != "MOTOR ON") {
            lcd.clear();                  // Clear LCD screen
            lcd.print("MOTOR ON");        // Display "MOTOR ON"
            lastMessage = "MOTOR ON";     // Update the last message
        }
        digitalWrite(motorPin1, HIGH);    // Turn on the motor (forward direction)
        digitalWrite(motorPin2, LOW);
        digitalWrite(led1, LOW);          // Turn off the green LED
        Serial.println(String(distance) + " cm MOTOR START"); // Debug message
    } else { // Handle invalid or out-of-range distance
        lcd.clear();
        lcd.print("INVALID DIST");
        Serial.println("Invalid distance!");
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(led1, LOW);
    }

    delay(100); // Short delay to stabilize the loop
}
