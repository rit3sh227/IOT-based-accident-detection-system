/* connection of HC-05 with arduino
| HC-05 | Arduino |
| ----- | ------- |
| VCC   | 5V      |
| GND   | GND     |
| TXD   | D10     |
| RXD   | D11     |
*/

// code to check the working of HC-05 when connected with arduino 
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

void setup() {

  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("HC-05 TEST");
  Serial.println("Connect phone to HC-05");
  Serial.println("Send any character");
}

void loop() {

  if (BT.available()) {

    char c = BT.read();

    Serial.print("Received from HC-05: ");
    Serial.println(c);
  }
}
