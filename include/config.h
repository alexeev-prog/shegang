#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE				"~/.shegangrc"
#define CONFIG_ALIASES 			"/.sgaliases"
#define DEFAULT_USERNAME_COLOR  "\033[0;33m"
#define DEFAULT_PWD_COLOR		"\033[0;32m"

#define MAX_LINE_LENGTH				256
#define MAX_ALIASES					128

typedef struct {
	char name[64];
	char command[256];
} Alias;

void load_config_aliases(void);

void load_main_config(void);

#endif // CONFIG_H
