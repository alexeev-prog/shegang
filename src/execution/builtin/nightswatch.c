/**
 * @title SheGang Linux Shell | Builtin nightswatch util
 * @filename builtin/nightswatch.c
 * @brief Module with nightswatch module
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "colors.h"
#include "userinput.h"

/**
 * @brief      Get newborn from /proc/loadavg
 *
 * @return     status code
 */
int nightwatch_newborn(char** args) {
	FILE *fd = fopen("/proc/loadavg", "r");

	if (fd == NULL) {
		print_message("Loading file error /proc/loadavg", ERROR);
		return 1;
	} else {
		char *line = r_line("/proc/loadvg", 2);
		char *token = strtok(line, " ");
		char *children;

		while (token != NULL) {
			children = token;
			token = strtok(NULL, " ");
		}

		printf("Children born: %s\n", children);
		free(line);
	}
	fclose(fd);

	return 1;
}
