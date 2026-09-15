// code to see the working when all the components are connected
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// =====================================================
// BLUETOOTH
// =====================================================

// SoftwareSerial(RX, TX)
// HC-05 TX -> Arduino D10
// HC-05 RX -> Arduino D11

SoftwareSerial BT(10, 11);


// =====================================================
// GPS
// =====================================================

// NEO-6M TX -> Arduino A2
// NEO-6M RX -> Arduino A3

SoftwareSerial GPSserial(A2, A3);

TinyGPSPlus gps;


// =====================================================
// L298N MOTOR DRIVER
// =====================================================

const int ENA = 5;
const int ENB = 6;

const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 4;


// =====================================================
// HC-SR04
// =====================================================

const int trigPin = 2;
const int echoPin = 3;


// =====================================================
// BUZZER
// =====================================================

const int buzzer = 12;


// =====================================================
// SETTINGS
// =====================================================

// Normal obstacle warning distance
const int WARNING_DISTANCE = 15;

// Very close distance = collision/emergency condition
const int COLLISION_DISTANCE = 5;


// Motor speed
int Speed = 180;


// Current movement
char currentCommand = 'S';


// Emergency state
bool emergencyMode = false;


// Buzzer timing
unsigned long lastBeep = 0;


// GPS timing
unsigned long lastGPSCheck = 0;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  BT.begin(9600);
  GPSserial.begin(9600);


  // Motor pins

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  // HC-SR04

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  // Buzzer

  pinMode(buzzer, OUTPUT);


  // Initial state

  stopCar();

  noTone(buzzer);


  Serial.println();
  Serial.println("====================================");
  Serial.println("   SMART IoT CAR SYSTEM");
  Serial.println("====================================");

  Serial.println("Bluetooth : READY");
  Serial.println("HC-SR04   : READY");
  Serial.println("GPS       : READY");
  Serial.println("Motors    : READY");
  Serial.println("Buzzer    : READY");

  Serial.println("------------------------------------");
  Serial.println("Bluetooth Commands:");
  Serial.println("F = Forward");
  Serial.println("B = Backward");
  Serial.println("L = Left");
  Serial.println("R = Right");
  Serial.println("S = Stop");
  Serial.println("0-9 = Speed");
  Serial.println("Q = Maximum Speed");
  Serial.println("------------------------------------");

  Serial.println("System ready.");
  Serial.println();

  delay(1000);
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ---------------------------------------------------
  // CHECK BLUETOOTH
  // ---------------------------------------------------

  BT.listen();

  while (BT.available()) {

    char command = BT.read();

    // Ignore newline/carriage return
    if (command == '\n' || command == '\r') {
      continue;
    }

    // Convert lowercase to uppercase
    if (command >= 'a' && command <= 'z') {
      command = command - 32;
    }

    Serial.print("Bluetooth Command: ");
    Serial.println(command);

    handleCommand(command);
  }


  // ---------------------------------------------------
  // CHECK GPS
  // ---------------------------------------------------

  GPSserial.listen();

  while (GPSserial.available()) {

    char c = GPSserial.read();

    gps.encode(c);
  }


  // ---------------------------------------------------
  // CONTINUOUS OBSTACLE MONITORING
  // ---------------------------------------------------

  if (!emergencyMode && currentCommand == 'F') {

    int distance = getDistance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");


    // Very close object
    if (distance > 0 && distance <= COLLISION_DISTANCE) {

      emergencyStop();

    }


    // Normal obstacle warning
    else if (distance > 0 && distance <= WARNING_DISTANCE) {

      stopCar();

      currentCommand = 'S';

      beepWarning();

      Serial.println("WARNING: OBSTACLE DETECTED");
      Serial.println("Vehicle stopped.");
    }
  }


  // ---------------------------------------------------
  // GPS CONTINUOUSLY PARSE
  // ---------------------------------------------------

  GPSserial.listen();

  while (GPSserial.available()) {

    char c = GPSserial.read();

    gps.encode(c);
  }


  delay(20);
}


// =====================================================
// HANDLE BLUETOOTH COMMAND
// =====================================================

void handleCommand(char cmd) {

  // If emergency mode is active,
  // only S is accepted.
  if (emergencyMode) {

    if (cmd == 'S') {

      noTone(buzzer);

      emergencyMode = false;

      currentCommand = 'S';

      stopCar();

      Serial.println("Emergency mode cleared.");
    }

    else {

      Serial.println("EMERGENCY MODE ACTIVE.");
      Serial.println("Press S to reset.");
    }

    return;
  }


  switch (cmd) {

    // -----------------------------------------------
    // FORWARD
    // -----------------------------------------------

    case 'F':

      currentCommand = 'F';

      {

        int distance = getDistance();

        Serial.print("Front Distance: ");
        Serial.print(distance);
        Serial.println(" cm");


        if (distance > 0 && distance <= COLLISION_DISTANCE) {

          emergencyStop();

        }

        else if (distance > 0 && distance <= WARNING_DISTANCE) {

          stopCar();

          currentCommand = 'S';

          beepWarning();

          Serial.println("OBSTACLE TOO CLOSE");
          Serial.println("Car stopped.");

        }

        else {

          forward();

          Serial.println("Motors: FORWARD");
        }
      }

      break;


    // -----------------------------------------------
    // BACKWARD
    // -----------------------------------------------

    case 'B':

      currentCommand = 'B';

      backward();

      noTone(buzzer);

      Serial.println("Motors: BACKWARD");

      break;


    // -----------------------------------------------
    // LEFT
    // -----------------------------------------------

    case 'L':

      currentCommand = 'L';

      left();

      noTone(buzzer);

      Serial.println("Motors: LEFT");

      break;


    // -----------------------------------------------
    // RIGHT
    // -----------------------------------------------

    case 'R':

      currentCommand = 'R';

      right();

      noTone(buzzer);

      Serial.println("Motors: RIGHT");

      break;


    // -----------------------------------------------
    // STOP
    // -----------------------------------------------

    case 'S':

      currentCommand = 'S';

      stopCar();

      noTone(buzzer);

      Serial.println("Motors: STOP");

      break;


    // -----------------------------------------------
    // SPEED
    // -----------------------------------------------

    case '0':
      Speed = 100;
      Serial.println("Speed = 100");
      break;

    case '1':
      Speed = 120;
      Serial.println("Speed = 120");
      break;

    case '2':
      Speed = 140;
      Serial.println("Speed = 140");
      break;

    case '3':
      Speed = 155;
      Serial.println("Speed = 155");
      break;

    case '4':
      Speed = 170;
      Serial.println("Speed = 170");
      break;

    case '5':
      Speed = 185;
      Serial.println("Speed = 185");
      break;

    case '6':
      Speed = 200;
      Serial.println("Speed = 200");
      break;

    case '7':
      Speed = 215;
      Serial.println("Speed = 215");
      break;

    case '8':
      Speed = 230;
      Serial.println("Speed = 230");
      break;

    case '9':
      Speed = 245;
      Serial.println("Speed = 245");
      break;

    case 'Q':
      Speed = 255;
      Serial.println("Speed = 255");
      break;


    default:

      Serial.println("Unknown command.");

      break;
  }
}


// =====================================================
// HC-SR04 DISTANCE
// =====================================================

int getDistance() {

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  long duration = pulseIn(echoPin, HIGH, 25000);


  if (duration == 0) {

    return 400;
  }


  int distance = duration * 0.0343 / 2;


  if (distance > 400) {

    distance = 400;
  }


  return distance;
}


// =====================================================
// MOTOR FORWARD
// =====================================================

void forward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
}


// =====================================================
// MOTOR BACKWARD
// =====================================================

void backward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
}


// =====================================================
// LEFT
// =====================================================

void left() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
}


// =====================================================
// RIGHT
// =====================================================

void right() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
}


// =====================================================
// STOP
// =====================================================

void stopCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


// =====================================================
// OBSTACLE WARNING
// =====================================================

void beepWarning() {

  unsigned long currentTime = millis();

  if (currentTime - lastBeep >= 500) {

    tone(buzzer, 1000, 200);

    lastBeep = currentTime;
  }
}


// =====================================================
// EMERGENCY STOP
// =====================================================

void emergencyStop() {

  stopCar();

  currentCommand = 'S';

  emergencyMode = true;


  Serial.println();
  Serial.println("====================================");
  Serial.println("       COLLISION DETECTED");
  Serial.println("====================================");

  Serial.println("Vehicle stopped.");

  tone(buzzer, 1500);


  // Get GPS location
  getCrashLocation();
}


// =====================================================
// GET GPS CRASH LOCATION
// =====================================================

void getCrashLocation() {

  Serial.println();
  Serial.println("Getting GPS location...");
  Serial.println("Please wait...");


  GPSserial.listen();


  unsigned long startTime = millis();


  while (millis() - startTime < 10000) {

    while (GPSserial.available()) {

      char c = GPSserial.read();

      gps.encode(c);
    }


    if (gps.location.isValid()) {

      Serial.println();
      Serial.println("******** CRASH LOCATION ********");

      Serial.print("Latitude: ");

      Serial.println(
        gps.location.lat(),
        6
      );


      Serial.print("Longitude: ");

      Serial.println(
        gps.location.lng(),
        6
      );


      Serial.print("Satellites: ");

      Serial.println(
        gps.satellites.value()
      );


      Serial.println();

      Serial.print("Google Maps: ");

      Serial.print(
        "https://maps.google.com/?q="
      );

      Serial.print(
        gps.location.lat(),
        6
      );

      Serial.print(",");

      Serial.println(
        gps.location.lng(),
        6
      );


      Serial.println("********************************");
      Serial.println();

      return;
    }
  }


  Serial.println();
  Serial.println("GPS LOCATION NOT AVAILABLE.");
  Serial.println("Check GPS connection or move outdoors.");
  Serial.println();
}
