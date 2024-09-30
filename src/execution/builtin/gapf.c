/**
 * @title SheGang Linux Shell | Built-in util `gapf` (cat analogue)
 * @filename gapf.c
 * @brief Module with gapf (cat) util
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "colors.h"

/**
 * @brief      Prints file details.
 *
 * @param[in]  filename  The filename
 */
void printFileDetails(const char* filename) {
	struct stat fileStat;
	if (stat(filename, &fileStat) < 0) {
		perror("stat");
		return;
	}

	printf("\nFile Details:\n");
	printf("File Permissions: ");
	printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf("(%o)\n", fileStat.st_mode & 0777);
	printf("Last modified: %s", ctime(&fileStat.st_mtime));
}

/**
 * @brief      Prints a file type.
 *
 * @param[in]  filename  The filename
 */
void printFileType(const char* filename) {
	const char* extension = strrchr(filename, '.');
	if (extension != NULL) {
		if (strcmp(extension, ".md") == 0) {
			printf("File Type: Markdown\n");
		} else if (strcmp(extension, ".c") == 0) {
			printf("File Type: C Code\n");
		} else if (strcmp(extension, ".cpp") == 0) {
			printf("File Type: C++ Code\n");
		} else if (strcmp(extension, ".py") == 0) {
			printf("File Type: Python Code\n");
		} else if (strcmp(extension, ".sh") == 0) {
			printf("File Type: Shell code\n");
		} else {
			printf("File Type: Unknown\n");
		}
	} else {
		printf("File Type: Unknown\n");
	}
}

/**
 * @brief      Get And Print File - cat analogue
 *
 * @param      args  The arguments
 *
 * @return     Status Code
 */
int gapf(char** args) {
	if (args[1] == NULL) {
		printf("Usage: %s <filename>\n", args[0]);
		return 1;
	}

	FILE* file = fopen(args[1], "r");
	if (file == NULL) {
		perror("fopen");
		return 1;
	}

	char line[256];
	int lineNumber = 1;

	while (fgets(line, sizeof(line), file)) {
		printf("%s%3d: %s%s", BLUE, lineNumber, RESET, line);
		lineNumber++;
	}

	fclose(file);

	printFileDetails(args[1]);
	printFileType(args[1]);

	return 1;
}
