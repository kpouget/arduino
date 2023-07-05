#define trigPin 13
#define echoPin 12

void dist_setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long dist_read() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) / 29.1;

  return distance;
}

int distance_to_keep = 0;

void distance_new_program() {
  if (prefix("stopped", program)) {
    delay(1000);
  } else if (prefix("stop", program)) {
    log_info("Stopping.\n");
    program = "stoppped";
  } else if (prefix("dist", program)) {

  } else if (prefix("help", program)) {
    log_info("Keep distance mode. Available commands:\n");
    log_info("- stop -> stop and wait\n");
    log_info("- dist<cm> -> keep a distance of N cm\n");
  } else {
    log_info("Unknown command: ");
    log_info(program);
    log_info("\nStopping\n");
    program = "stoppped";
  }
}

void distance_process_step() {
  if (distance_to_keep <= 5) {
    sprintf(buffer, "Distance of %dcm is to short to keep.\n", distance_to_keep);
    log_info(buffer);
    delay(1000);
    return;
  }

  sprintf(buffer, "Don't know how to keep a distance  of %dcm at the moment.\n", distance_to_keep);
  log_info(buffer);
  delay(1000);
}
