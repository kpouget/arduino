#include "configure.h"

static int current_step = 0;
static String program = "";

void setup() {
  log_info(F("Initializing the bluetooth ...\n"));
  bt_setup();
  log_info(F("Initializing the serial link ...\n"));
  serial_setup();
  log_info(F("Initializing the motors ...\n"));
  step_setup();
  log_info(F("Initializing the sonar ...\n"));
  dist_setup();


  program = CONFIG_HELP_PREFIX;
  process_new_instruction();
  log_info(F("---\n"));
  process_new_mode(CONFIG_MODE_DEFAULT);
  process_new_instruction();
  log_info(F("Ready!\n"));
  log_info(F("---\n"));
}

void log_info(const __FlashStringHelper *msg) {
  bt_info(msg);
  serial_info(msg);
}


void log_info(char *msg) {
  bt_info(msg);
  serial_info(msg);
}

void check_instruction_update() {

  String msg = bt_read();
  if (msg.length() == 0) {

    msg = serial_read();

    if (msg.length() == 0) {
      return;
    }
  }

  log_info(F("New instruction: "));
  sprintf(buffer, "|%s|\n", msg.c_str());
  log_info(buffer);

  current_step = 0;
  program = msg;

  process_new_instruction();
}

void process_new_instruction() {
  if (configure()) {
    program = "";
  } else if (cfg_mode == CONFIG_MODE_KEEP_DISTANCE) {
    distance_new_instruction();
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

  if (current_step >= program.length()) {
    if (current_step == program.length()) {
      log_info(F("Finished, waiting for new instructions ...\n"));
      current_step++ ;
    }
    delay(100);
    return;
  }

  delay(cfg_delay);

  char action = program.c_str()[current_step];
  int debug = 0;
  if (debug) {
    log_info(F("Program: "));
    sprintf(buffer, "%s[%d] = %c\n",
            program.c_str(), current_step, action);
    log_info(buffer);
  }

  switch (cfg_mode) {
  case CONFIG_MODE_CALIBRATE:
    loop_calibrate(action);
    break;
  case CONFIG_MODE_PROGRAM:
    loop_program(action);
    break;

  default:
    log_info(F("Unknown mode: "));
    sprintf(buffer, "'%d'", cfg_mode);
    log_info(buffer);
    log_info(F(". Panicking\n"));
    delay(1000);
  }

  current_step++ ;
}

void loop() {
  unsigned long current_time = millis();

  if (current_time - last_bt_read_time > bt_read_delay) {
    last_bt_read_time = current_time;
    /* --- */

    check_instruction_update();
  }

  if (dist_read_enabled) {
    if (dist_read_delay > 0 && current_time - last_dist_read_time > dist_read_delay) {
      last_dist_read_time = current_time;
      /* --- */
      update_distance();
    }
  }

  process_step();
}
