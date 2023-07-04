#include <SoftwareSerial.h>

SoftwareSerial hc06(2, 3);

#define trigPin 13
#define echoPin 12

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Initialize Bluetooth Serial Port
  hc06.begin(115200);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if (distance >= 2000 ) {
    Serial.println("Out of range");

    hc06.write("Out of range");
    hc06.write("\n");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");

    char buffer[40];
    sprintf(buffer, "%d cm\n", distance);
    hc06.write(buffer);
  }
  delay(1000);
}
