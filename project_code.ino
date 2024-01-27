// Note: for my beam to be horizontal, Servo Motor angle should be 102 degrees.
int last_reading;
#include <Servo.h>
#include <PID_v1.h>

// Define Pin constants
const int servoPin = 5;   // Servo Pin
const int TrigPin = 9;    // Ultrasonic Sensor Trigger Pin
const int EchoPin = 10;   // Ultrasonic Sensor Echo Pin

// PID Constants
float Kp = 3.5;   // Initial Proportional Gain
float Ki = 2;     // Initial Integral Gain
float Kd = 0.5;   // Initial Derivative Gain

// PID Variables
double Setpoint, Input, Output, ServoOutput;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); // Initialize PID object

// Servo Object
Servo myServo;

void setup() {
  // Initialize variables and set up serial communication
  last_reading = 5;
  Serial.begin(9600);

  // Set up Ultrasonic Sensor pins
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Attach Servo
  myServo.attach(servoPin);

  // Set initial conditions for PID
  Input = readPosition();
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 180); // Set Output limits to 0 and 180 degrees
}

void loop() {
  // Setpoint for the desired position
  Setpoint = 14;

  // Read current position using the Ultrasonic Sensor
  Input = readPosition();

  // Compute PID control
  myPID.Compute();

  // Store the current position
  last_reading = Input;

  // Calculate ServoOutput for the servo motor
  ServoOutput = (50 + Output); // 50 degrees is considered horizontal

  // Move the servo to the calculated position
  myServo.write(ServoOutput);

  // Print the ServoOutput for debugging
  Serial.println(ServoOutput);
}

// Function to read the position using Ultrasonic Sensor
float readPosition() {
  // Ensure a delay before reading to avoid overlapping echoes
  delay(40);

  // Ultrasonic Sensor Variables
  long duration, cm;
  unsigned long now = millis();

  // Trigger ultrasonic sensor to get distance measurement
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // Measure the duration of the pulse from the sensor
  duration = pulseIn(EchoPin, HIGH);

  // Convert duration to distance in centimeters
  cm = duration / (29 * 2);

  // Ensure the distance does not exceed the maximum position for the ball
  if (cm > 30) {
    cm = 30;
  }

  // Print the distance for debugging
  Serial.println(cm);

  // Return the distance value
  return cm;
}