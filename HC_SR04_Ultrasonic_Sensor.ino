/*
 *  HC-SR04 sensor measuring
 *
 *  Non-blocking HC-SR04 sensor measuring
 *
 *  The circuit:
 *  - Echo pin connected to pin 2
 *  - Trigger pin connected to pin 3
 *  - VCC pin connected to VCC (5v)
 *  - GND gger pin connected to GND
 *
 *  created 3 Jul 2021
 *  by alessandromrc
 */

#define trigPin 3
#define echoPin 2

unsigned long timerStart = 0;
unsigned long StartTime = micros();
bool timer = false;
const unsigned long HIGH_TRIGGER = 10;
const unsigned long LOW_TRIGGER = 2;

float timeDuration, distance;

enum SensorStatus {
  TRIG_LOW,
  TRIG_HIGH,
  ECHO_HIGH
};

SensorStatus sensorStatus = TRIG_LOW;

bool isTimerReady(const unsigned long Sec) {
  return (micros() - timerStart) < Sec;
}

void setup(void) {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop(void) {
  switch (sensorStatus) {
  case TRIG_LOW: {
    digitalWrite(trigPin, LOW);
    timerStart = micros();
    if (isTimerReady(LOW_TRIGGER)) {
      sensorStatus = TRIG_HIGH;
    }
  }
  break;

  case TRIG_HIGH: {
    digitalWrite(trigPin, HIGH);
    timerStart = micros();
    if (isTimerReady(HIGH_TRIGGER)) {
      sensorStatus = ECHO_HIGH;
    }
  }
  break;

  case ECHO_HIGH: {
    if (!timer) {
      Serial.print("Microseconds Passed in initialization: ");
      Serial.println(micros() - StartTime);
      timer = !timer;
      Serial.println("Starting to Measure");
    }
    digitalWrite(trigPin, LOW);
    timeDuration = pulseIn(echoPin, HIGH);
    Serial.print("Measured: ");
    Serial.print(timeDuration * 0.034 / 2);
    Serial.println(" cm");
    sensorStatus = TRIG_LOW;
  }
  break;
  }
}
