#include "configure.h"

void setup() {
  bt_setup();
  serial_setup();
  log_info("Initializing the motors ...\n");
  step_setup();
  log_info("Initializing the sonar ...\n");
  dist_setup();
  log_info("Ready!\n");
}

void log_info(char *msg) {
  bt_info(msg);
  serial_info(msg);
}

void update_distance() {
  return;
  long distance = dist_read();

  if (distance >= 2000 ) {
    log_info("dist: out of range\n");

    return;
  }

  sprintf(buffer, "dist: %d cm\n", distance);
  log_info(buffer);
}

static int current_step = 0;
static char *program = "";

void update_bt() {
  String msg = bt_read();
  if (msg.length() == 0) {
    return;
  }

  sprintf(buffer, "New instruction: %s\n", msg.c_str());
  log_info(buffer);

  static String program_str;

  program_str = msg;
  current_step = 0;
  program = program_str.c_str();

  if (configure()) {
    program = "";
  } else if (cfg_mode == CONFIG_MODE_KEEP_DISTANCE) {
    distance_new_program();
  }
}

void process_step() {
  step_do();

  if (cfg_mode == CONFIG_MODE_KEEP_DISTANCE) {
    distance_process_step();
    return;
  }

  if (!step_is_done()) {
    return;
  }

  if (current_step >= strlen(program)) {
    if (current_step == strlen(program)) {
      log_info("Finished, waiting for new instructions ...\n");
      current_step++ ;
    }
    delay(100);
    return;
  }

  delay(cfg_delay);

  char action = program[current_step];
  int debug = 0;
  if (debug) {
    sprintf(buffer, "Program: %s[%d] = %c\n",
            program, current_step, action);
    log_info(buffer);
  }

  switch (cfg_mode) {
  case CONFIG_MODE_CALIBRATE:
    loop_calibrate(action);
    break;
  case CONFIG_MODE_PROGRAM:
    loop_calibrate(action);
    break;

  default:
    sprintf(buffer, "Unknown mode: '%d'. Panicing\n", cfg_mode);
    log_info(buffer);
    delay(1000);
  }

  current_step++ ;
}

void loop() {
  unsigned long current_time = millis();

  if (current_time - last_bt_read_time > bt_read_delay) {
    last_bt_read_time = current_time;
    /* --- */
    update_bt();
  }

  if (dist_read_delay > 0 && current_time - last_dist_read_time > dist_read_delay) {
    last_dist_read_time = current_time;
    /* --- */
    update_distance();
  }

  process_step();
}
