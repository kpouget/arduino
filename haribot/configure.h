#ifndef CONFIGURE_H
#define CONFIGURE_H

#define CONFIG_HELP_PREFIX "@help"

#define CONFIG_MODE_PREFIX "@mode="
#define CONFIG_MODE_CALIBRATE 1
#define CONFIG_MODE_PROGRAM 2
#define CONFIG_MODE_KEEP_DISTANCE 3
#define CONFIG_MODE_DEFAULT CONFIG_MODE_KEEP_DISTANCE
extern int cfg_mode;

#define CONFIG_DELAY_PREFIX "@delay="
#define CONFIG_DELAY_DEFAULT 500
extern int cfg_delay;

extern char buffer[40];

#define DISTANCE_READ_DELAY 500

extern unsigned int bt_read_delay; // in ms
extern unsigned long last_bt_read_time;
extern unsigned int dist_read_delay; // in ms
extern unsigned long last_dist_read_time;
extern bool dist_read_enabled;

inline static bool prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

#endif
