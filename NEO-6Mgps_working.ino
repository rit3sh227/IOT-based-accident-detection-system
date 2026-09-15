/* connection of NEO-6M with arduino
| NEO-6M | Arduino |
| ------ | ------- |
| VCC    | 5V*     |
| GND    | GND     |
| TX     | A2      |
| RX     | A3      |
*/

// code to check the working of NEO-6M when connected with arduino
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial GPS(A2, A3);

TinyGPSPlus gps;

void setup() {

  Serial.begin(9600);
  GPS.begin(9600);

  Serial.println("NEO-6M GPS TEST");
  Serial.println("Take GPS outdoors.");
}

void loop() {

  while (GPS.available()) {

    char c = GPS.read();

    gps.encode(c);
  }

  if (gps.location.isUpdated()) {

    Serial.println("--------------------");

    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    Serial.print("Google Maps: ");
    Serial.print("https://maps.google.com/?q=");
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.println(gps.location.lng(), 6);
  }
}
