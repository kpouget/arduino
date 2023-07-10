void serial_setup() {
  Serial.begin (9600);
}

void serial_info(char *msg) {
  Serial.print(msg);
}

String serial_read() {
  if (! Serial.available()) {
    return String();
  }

  String msg = Serial.readString();
  msg.trim();

  return msg;
}
