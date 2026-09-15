/* connection of buzzer to arduino
| Buzzer | Arduino |
| ------ | ------- |
| +      | D12     |
| -      | GND     |
*/

// code to show the working of buzzer when connected with arduino
int buzzer = 12;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  tone(buzzer, 1000);
  delay(1000);

  noTone(buzzer);
  delay(1000);
}
