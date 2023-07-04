#include <SoftwareSerial.h>

SoftwareSerial hc06(2, 3);

void bt_setup() {
  //Initialize Bluetooth Serial Port
  hc06.begin(115200);
}

void bt_info(char *msg) {
  hc06.write(msg);
}

String bt_read() {
  if (! hc06.available()) {
    return String();
  }

  String msg = hc06.readString();
  msg.trim();

  return msg;
}
