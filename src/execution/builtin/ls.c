/**
 * @title SheGang Linux Shell | Built-in util `sgls` (ls analogue)
 * @filename ls.c
 * @brief Module with sgls (ls) util
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "colors.h"
#include "shell_api.h"

/**
 * @brief      Prints a styled.
 *
 * @param[in]  color  The color
 * @param[in]  text   The text
 * @param[in]  spch   The spch
 */
void print_styled(const char* color, const char* text, char* spch) {
	if (strcmp(spch, " ") == 1) { // if special char is not empty
		printf("%s%s%s%s\t", spch, color, text, RESET);
	} else {
		printf("%s%s%s\t", color, text, RESET);
	}
}

/**
 * @brief      check permissions of file
 *
 * @param[in]  fileStat  The file stat
 */
void check_permissions(struct stat fileStat) {
	/* Function for checking and outputting permissions, file or directory
	 * group. If st_mode fileStat is equal to d, then d is displayed in yellow,
	 * otherwise just -. And so on, we list write read and run permissions and
	 * output them in different colors:
	 * (user - green, group - blue, other - cyan)
	 *
	 * Input:
	 *	struct stat fileStat
	 *
	 * Output: void*/
	printf(YELLOW);
	printf((S_ISDIR(fileStat.st_mode)) ? "d"
			: ((S_ISLNK(fileStat.st_mode)) ? : "-"));
	printf(GREEN);
	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf(BLUE);
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf(MAGENTA);
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf(RESET);
}

/**
 * @brief      Prints a grid.
 *
 * @param      items   The items
 * @param[in]  count   The count
 * @param      colors  The colors
 */
void print_grid(char** items, int count, char** colors) {
	/* Function for printing items in a grid */
	int max_length = 0;
	int length = 0;

	for (int i = 0; i < count; i++) {
		length = strlen(items[i]) + strlen(colors[i]);
		if (length > max_length) {
			max_length = length;
		}
	}

	int columns = MAX_GRID_COLUMNS;
	int rows = (count + columns - 1) / columns;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			int index = r + c * rows;
			if (index < count) {
				printf("%s%-*s", colors[index], max_length, items[index]);
			}
		}
		printf("\n");
	}
}

/**
 * @brief      list files
 *
 * @param[in]  dir_path          The dir path
 * @param[in]  show_permissions  The show permissions
 * @param[in]  show_time         The show time
 * @param[in]  show_hidden       The show hidden
 * @param[in]  list_show         The list show
 */
void list_files(const char* dir_path, bool show_permissions, bool show_time,
				bool show_hidden, bool list_show) {
	/* Function for listing files and directories in a specific directory */
	DIR* dir;
	struct dirent* entry;
	struct stat file_stat;
	char** items = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
	char** colors = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
	int count = 0;

	// Open the directory
	dir = opendir(dir_path);
	if (dir == NULL) {
		print_message("Error opening the directory", ERROR);
		return;
	}

	// Read the directory entries
	while ((entry = readdir(dir)) != NULL) {
		char full_path[MAX_FILE_NAME_LENGTH];
		char formattedString[MAX_FILE_NAME_LENGTH];
		char color[MAX_FILE_NAME_LENGTH];
		stat(full_path, &file_stat);
		snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

		// Skip hidden files and current/parent directories
		if (entry->d_name[0] == '.' && !show_hidden) {
			continue;
		}

		// Get the file stats
		if (lstat(full_path, &file_stat) < 0) {
			print_message("Error get file stats", ERROR);
			continue;
		}

		if (show_permissions) {
			list_show = true;
			check_permissions(file_stat);
			printf("%s(%lo)%s ", YELLOW,
					(unsigned long)file_stat.st_mode & 0777, RESET);
		}

		if (show_time) {
			list_show = true;
			char time_str[100];
			strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S",
					localtime(&file_stat.st_ctime));
			printf("%s[%s]%s ", CYAN, time_str, RESET);
		}

		if (S_ISDIR(file_stat.st_mode)) {
			if (!list_show) {
				sprintf(formattedString, "  %s", entry->d_name);
				sprintf(color, "%s", BLUE);
			} else {
				print_styled(BLUE, entry->d_name, "  ");
			}
		} else if (S_ISLNK(file_stat.st_mode)) {
			if (!list_show) {
				sprintf(formattedString, "  %s", entry->d_name);
				sprintf(color, "%s", CYAN);
			} else {
				print_styled(CYAN, entry->d_name, "  ");
			}
		} else if ((file_stat.st_mode & S_IXUSR) ||
					(file_stat.st_mode & S_IXGRP)
					|| (file_stat.st_mode & S_IXOTH)) {
			if (!list_show) {
				sprintf(formattedString, "  %s", entry->d_name);
				sprintf(color, "%s", GREEN);
			} else {
				print_styled(GREEN, entry->d_name, "  ");
			}
		} else if (access(full_path, R_OK) == 0 &&
					(strstr(entry->d_name, ".png") != NULL
					|| strstr(entry->d_name, ".jpg") != NULL
					|| strstr(entry->d_name, ".jpeg") != NULL
					|| strstr(entry->d_name, ".svg") != NULL
					|| strstr(entry->d_name, ".bmp") != NULL)) {
			if (!list_show) {
				sprintf(formattedString, "  %s", entry->d_name);
				sprintf(color, "%s", MAGENTA);
			} else {
				print_styled(MAGENTA, entry->d_name, "  ");
			}
		} else {
			if (!list_show) {
				sprintf(formattedString, "  %s", entry->d_name);
				sprintf(color, "%s", WHITE);
			} else {
				print_styled(BOLD, entry->d_name, "  ");
			}
		}

		if (!list_show) {
			// Get the file/directory name
			items[count] = strdup(formattedString);
			colors[count] = strdup(color);
		} else {
			printf("\n");
		}
		count++;
	}

	if (!list_show)
		print_grid(items, count, colors);

	// Clean up resources
	closedir(dir);
	// for (int i = 0; i < count; i++) {
	// 	free(items[i]);
	// }
	free(items);
}

/**
 * @brief      Gets the array size.
 *
 * @param      array  The array
 *
 * @return     The array size.
 */
int get_array_size(char** array) {
	int count = 0;

	if (array != NULL) {
		while (array[count] != NULL) {
			count++;
		}
	}

	return count;
}

/**
 * @brief      Main sgls command
 *
 * @param[in]  argc  The count of arguments
 * @param      args  The arguments array
 *
 * @return     Status Code
 */
int sgls(char** args) {
	char *dir_path = ".";
	bool show_permissions = false;
	bool show_time = false;
	bool show_hidden = false;
	bool show_list = false;

	for (int i=0; args[i] != NULL; i++) {
		if (strcmp(args[i], "-p") == 0) {
			show_permissions = true;
		} else if (strcmp(args[i], "-t") == 0) {
			show_time = true;
		} else if (strcmp(args[i], "-a") == 0) {
			show_hidden = true;
		} else if (strcmp(args[i], "-l") == 0) {
			show_list = true;
		} else if (strcmp(args[i], "-h") == 0) {
			println("Built-in function `sgls`\n");
			printf(
				"SGLS - A `ls` command analogue from shegang\n\n"
				"-t				Show time\n"
				"-a				Show hidden\n"
				"-l				Show as list\n"
				"-p				Show permissions\n"
			);
			return 1;
		} else {
			dir_path = args[i];
		}
	}

	if (args[1] == NULL) {
		dir_path = ".";
	}

	list_files(dir_path, show_permissions, show_time,
						show_hidden, show_list);

	return 1;
}
