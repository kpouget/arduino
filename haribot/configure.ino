#include "configure.h"

char buffer[40];

bool cfg_instruction_mode = true;
int cfg_mode = CONFIG_MODE_PROGRAM;
int cfg_delay = CONFIG_DELAY_DEFAULT;


unsigned int bt_read_delay = 100; // in ms
unsigned long last_bt_read_time = 0;

unsigned int dist_read_delay = 0; // in ms
unsigned long last_dist_read_time = 0;

bool configure() {
  if (prefix(CONFIG_HELP_PREFIX, program)) {
#define str(s) #s
    log_info(CONFIG_MODE_PREFIX str(CONFIG_MODE_CALIBRATE) " --> calibration mode\n");
    log_info(CONFIG_MODE_PREFIX str(CONFIG_MODE_PROGRAM) " --> program mode\n");
    log_info(CONFIG_MODE_PREFIX str(CONFIG_MODE_KEEP_DISTANCE) " --> keep distance mode\n");

    sprintf(buffer, "Current mode: %d\n", cfg_mode);
    log_info(buffer);

    log_info(CONFIG_DELAY_PREFIX "<delay> --> delay between the execution of the commands\n");
    sprintf(buffer, "Current delay: %dms\n", cfg_delay);
    log_info(buffer);
#undef str
  } else if (prefix(CONFIG_MODE_PREFIX, program)) {
    int new_mode = atoi(program[strlen(CONFIG_MODE_PREFIX)]);
    switch (new_mode) {
    case CONFIG_MODE_CALIBRATE:
      log_info("Calibrate mode activated.\n");
      break;
    case CONFIG_MODE_PROGRAM:
      log_info("Program mode activated.\n");
      break;
    case CONFIG_MODE_KEEP_DISTANCE:
      log_info("Keep distance mode activated.\n");
      break;

    default:
      sprintf(buffer, "Unknown mode requested: '%d'. Ignoring\n", new_mode);
      log_info(buffer);
      new_mode = cfg_mode;
    }
    cfg_mode = new_mode;
  } else if (prefix(CONFIG_DELAY_PREFIX, program)) {
    int new_delay = atoi(program[strlen(CONFIG_DELAY_PREFIX)]);
    sprintf(buffer, "New delay: %dms\n", new_delay);
    log_info(buffer);
    cfg_delay = new_delay;
  } else if (prefix("@", program)) {
    sprintf(buffer, "Invalid configuration prefix: %s\n", program);
    log_info(buffer);
  } else {
    return false;
  }

  return true;
}
