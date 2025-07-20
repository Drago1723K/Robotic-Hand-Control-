#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

#define SERVOMIN 150  // 0 degrees
#define SERVOMAX 600  // 180 degrees

const uint8_t numberOfServos = 5;
const uint8_t servoChannels[numberOfServos] = {5, 6, 7, 8, 9};

// Flex sensor pins (ensure they’re all different)
const int f1 = A2;
const int f2 = A2;
const int f3 = A3;
const int f4 = A3;
const int f5 = A3;

// Threshold values (tune these after calibration)
const int flexThresholds[5] = {519, 519, 526, 526, 526}; // Example thresholds

// Function to convert angle to PWM pulse
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

// Simple averaging function to reduce fluctuations
int smoothAnalogRead(int pin) {
  const int samples = 10;
  int total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delay(2);
  }
  return total / samples;
}

void setup() {
  Serial.begin(9600);
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);

  // Initialize all servos to 0°
  for (int i = 0; i < numberOfServos; i++) {
    myServo.setPWM(servoChannels[i], 0, angleToPulse(0));
  }
}

void loop() {
  // Read flex sensor values
  int flexValues[5] = {
    smoothAnalogRead(f2),
    flexValues[0],
    smoothAnalogRead(f3),
    flexValues[2],
    flexValues[2]
  };

  // Check threshold and move servo to 0° or 180°
  for (int i = 0; i < numberOfServos; i++) {
    int angle = (flexValues[i] > flexThresholds[i]) ? 180 : 0;
    myServo.setPWM(servoChannels[i], 0, angleToPulse(angle));
  }

  // Optional debug output
  for (int i = 0; i < 5; i++) {
    Serial.print("F"); Serial.print(i+1); Serial.print(": ");
    Serial.print(flexValues[i]); Serial.print("  ");
  }
  Serial.println();

  delay(100);
}
