void serial_setup() {
  Serial.begin (9600);
}

void serial_info(char *msg) {
  Serial.print(msg);
}
