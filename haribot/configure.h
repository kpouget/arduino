#define CONFIG_HELP_PREFIX "@help"

#define CONFIG_MODE_PREFIX "@mode="
#define CONFIG_MODE_CALIBRATE 1
#define CONFIG_MODE_PROGRAM 2
#define CONFIG_MODE_DEFAULT CONFIG_MODE_PROGRAM
extern int cfg_mode;

#define CONFIG_DELAY_PREFIX "@delay="
#define CONFIG_DELAY_DEFAULT 500
extern int cfg_delay;

extern char buffer[80];
