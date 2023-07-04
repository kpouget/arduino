#include <SoftwareSerial.h>

SoftwareSerial hc06(2, 3);

void setup(){
  //Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  //Initialize Bluetooth Serial Port
  hc06.begin(115200);
}
int cpt = 0;
void loop(){
  while (hc06.available()) {
    Serial.write(hc06.read());
 }
  char buffer[40];
  sprintf(buffer, "%d", cpt);
  hc06.write("Hello: ");
  hc06.write(buffer);
  hc06.write("\n");
  delay(1000);
  cpt += 1;
}
