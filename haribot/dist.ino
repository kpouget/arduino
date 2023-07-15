#define trigPin 13
#define echoPin 12

void dist_setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

static long dist_read() {
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

long dist_last_read = 0;

void update_distance() {
  dist_last_read = dist_read();
}

/***/

int distance_to_keep = 0;

void distance_new_instruction() {
  if (prefix("stopped", program.c_str())) {
    delay(1000);
  } else if (prefix("stop", program.c_str())) {
    log_info(F("Stopping.\n"));
    program = "stoppped";
    step_stop();

  } else if (prefix("dist", program.c_str())) {
    distance_to_keep = atoi(&program.c_str()[strlen("dist")]);
    sprintf(buffer, "%dcm.\n", distance_to_keep);
    log_info(F("Distance to keep: "));
    log_info(buffer);

    if (distance_to_keep < 5) {
      log_info(F("Dist is too short.\n"));
      distance_to_keep = 0;
    }

    if (distance_to_keep > 50) {
      log_info(F("Dist is too long.\n"));
      distance_to_keep = 0;
    }

  } else if (prefix("help", program.c_str())) {
    log_info(F("Keep distance mode. Available commands:\n"));
    log_info(F("# dist<cm> --> keep a distance of N cm\n"));
  log_info(F("# stop     --> stop and wait\n"));
  log_info(F("# help     --> show some help\n"));
  } else {
    log_info(F("dist: Unknown command: "));
    log_info(program.c_str());
    log_info(F("\nStopping\n"));
    distance_to_keep = 0;
  }
}

void distance_process_step() {
  if (distance_to_keep == 0) {
    return;
  }
  static unsigned long seen_dist_read_time = 0;

  if (seen_dist_read_time == last_dist_read_time) {
    // distance already processed
    return;
  }
  seen_dist_read_time = last_dist_read_time;

  sprintf(buffer, "dist: %d cm\n", dist_last_read);
  log_info(buffer);


  static bool distance_too_big = false;

  if (dist_last_read > 100) {
    if (!distance_too_big) {
      sprintf(buffer, "Distance of %dcm is to big!.\n", dist_last_read);
      log_info(buffer);
    }
    distance_too_big = true;
    return;
  }
  distance_too_big = false;

  int distance_to_move = dist_last_read - distance_to_keep;


  sprintf(buffer, "move: %dcm\n", distance_to_move);
  log_info(buffer);

  step_move(distance_to_move, distance_to_move);
}
