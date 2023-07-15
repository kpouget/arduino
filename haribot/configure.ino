#include "configure.h"

char buffer[40];

bool cfg_instruction_mode = true;
int cfg_mode = CONFIG_MODE_DEFAULT;
int cfg_delay = CONFIG_DELAY_DEFAULT;


unsigned int bt_read_delay = 1000; // in ms
unsigned long last_bt_read_time = 0;

bool dist_read_enabled = false;
unsigned int dist_read_delay = DISTANCE_READ_DELAY; // in ms
unsigned long last_dist_read_time = 0;

void process_new_mode(int new_mode) {
  switch (new_mode) {
  case CONFIG_MODE_CALIBRATE:
    log_info(F("Calibrate mode activated.\n"));
    dist_read_enabled = false;
    break;

  case CONFIG_MODE_PROGRAM:
    log_info(F("Program mode activated.\n"));
    dist_read_enabled = false;
    break;

  case CONFIG_MODE_KEEP_DISTANCE:
    log_info(F("Keep distance mode activated.\n"));
    dist_read_enabled = true;
    program = "help";
    break;

  default:
    log_info(F("Unknown mode requested: '"));
    sprintf(buffer, "%d'. Current mode: %d.\n", new_mode);
    log_info(buffer);
    log_info(F(". Current mode: "));
    sprintf(buffer, "%d.\n", cfg_mode);
    log_info(buffer);
    new_mode = cfg_mode;
  }

  cfg_mode = new_mode;
}

bool configure() {
  if (prefix(CONFIG_HELP_PREFIX, program.c_str())) {
#define str(s) _str(s)
#define _str(s) #s
    log_info(F("# " CONFIG_HELP_PREFIX " --> show some help\n"));
    log_info(F("# " CONFIG_MODE_PREFIX str(CONFIG_MODE_CALIBRATE) " --> calibration mode\n"));
    log_info(F("# " CONFIG_MODE_PREFIX str(CONFIG_MODE_PROGRAM) " --> program mode\n"));
    log_info(F("# " CONFIG_MODE_PREFIX str(CONFIG_MODE_KEEP_DISTANCE) " --> keep distance mode\n"));

    sprintf(buffer, "Current mode: %d\n", cfg_mode);
    log_info(buffer);

    log_info(F(CONFIG_DELAY_PREFIX "<delay> --> delay between the execution of the commands\n"));
    sprintf(buffer, "%dms\n", cfg_delay);
    log_info(F("Current delay: "));
    log_info(buffer);
#undef str
  } else if (prefix(CONFIG_MODE_PREFIX, program.c_str())) {
    int new_mode = atoi(&program.c_str()[strlen(CONFIG_MODE_PREFIX)]);
    process_new_mode(new_mode);

  } else if (prefix(CONFIG_DELAY_PREFIX, program.c_str())) {
    int new_delay = atoi(program.c_str()[strlen(CONFIG_DELAY_PREFIX)]);
    sprintf(buffer, "%dms\n", new_delay);
    log_info(F("New delay: "));
    log_info(buffer);
    cfg_delay = new_delay;

  } else if (prefix("@", program.c_str())) {
    log_info(F("Invalid configuration prefix: "));
    log_info(program.c_str());

  } else {
    return false;
  }

  return true;
}
