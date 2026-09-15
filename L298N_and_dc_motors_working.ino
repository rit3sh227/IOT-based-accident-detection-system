/* connection of L298N motor driver with arduino
| L298N | Arduino     |
| ----- | ----------- |
| ENA   | D5          |
| ENB   | D6          |
| IN1   | D7          |
| IN2   | D8          |
| IN3   | D9          |
| IN4   | D4          |
| GND   | Arduino GND |
*/

/* connection of L298N with dc motors
| L298N | Motor              |
| ----- | ------------------ |
| OUT1  | Left motor wire 1  |
| OUT2  | Left motor wire 2  |
| OUT3  | Right motor wire 1 |
| OUT4  | Right motor wire 2 |
*/

// code to show the working of dc motors and L298N when connected with arduino
int ENA = 5;
int ENB = 6;

int IN1 = 7;
int IN2 = 8;
int IN3 = 9;
int IN4 = 4;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();
}

void loop() {

  forward();
  delay(2000);

  stopCar();
  delay(1000);

  backward();
  delay(2000);

  stopCar();
  delay(1000);

  left();
  delay(1500);

  stopCar();
  delay(1000);

  right();
  delay(1500);

  stopCar();
  delay(2000);
}

// L298N commands to dc motors  
void forward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void backward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void left() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void right() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void stopCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
