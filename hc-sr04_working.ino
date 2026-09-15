/* connection of HC-SR04 with arduino
HC-SR04 |	Arduino
VCC	    | 5V
GND	    | GND
TRIG	  | D2
ECHO	  | D3
*/

// code to check working of hc-sr04 when connected with arduino
int trigPin = 2;
int echoPin = 3;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No echo - CHECK HC-SR04");
  }
  else {

    float distance = duration * 0.0343 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(500);
}
