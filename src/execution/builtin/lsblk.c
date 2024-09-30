/**
 * @title SheGang Linux Shell | Built-in util `sglsblk` (lsblk analogue)
 * @filename lsblk.c
 * @brief Module with sglsblk (lsblk) util
 * @authors Alexeev Bronislav
 * @license MIT License
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <linux/kdev_t.h>
#include "colors.h"
#include "shell_api.h"

/**
 * @brief      Disk struct
 */
struct Disk {
	char name[DEFAULT_BUFFER_SIZE];
	int maj_min;
	int rm;
	long long size;
	bool ro;
	char type[DEFAULT_BUFFER_SIZE];
	char mountpoint[DEFAULT_BUFFER_SIZE];
};

/**
 * @brief      Prints a disk.
 *
 * @param[in]  disk  The disk
 */
void print_disk(struct Disk disk) {
	printf("%-6s %-7d %-2d %-6lld %-2d %-6s %-12s\n", disk.name, disk.maj_min, disk.rm, disk.size, disk.ro, disk.type, disk.mountpoint);
}


/**
 * @brief      sglsblk (lsblk)
 *
 * @return     Status code
 */
int sglsblk(char** args) {
	DIR *dir;
	struct dirent *entry;
	struct stat st;
	char path[DEFAULT_BUFFER_SIZE];
	struct Disk disk;
	FILE *mounts;

	printf("%-6s %-7s %-3s %-7s %-3s %-7s %-12s\n", "NAME", "MAJ:MIN", "RM", "SIZE", "RO", "TYPE", "MOUNTPOINTS");

	dir = opendir("/sys/block");
	if (dir == NULL) {
		print_message("Error opening the dir", ERROR);
		return 1;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.')
			continue;

		snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/removable", entry->d_name);
		if (stat(path, &st) == -1)
			continue;

		if (S_ISREG(st.st_mode) && st.st_size > 0) {
			disk.rm = 1;
		} else {
			disk.rm = 0;
		}

		snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/ro", entry->d_name);
		if (stat(path, &st) == -1)
			continue;

		if (S_ISREG(st.st_mode) && st.st_size > 0) {
			disk.ro = true;
		} else {
			disk.ro = false;
		}

		snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/size", entry->d_name);

		if (stat(path, &st) == -1)
			continue;

		disk.size = st.st_size * 512ULL;

		disk.maj_min = makedev(st.st_rdev >> 8, st.st_rdev & 0xFF);

		strcpy(disk.name, entry->d_name);

		snprintf(path, DEFAULT_BUFFER_SIZE, "/dev/%s", entry->d_name);

		if ((disk.maj_min != 0 || disk.size != 0) && stat(path, &st) == 0 && S_ISBLK(st.st_mode)) {
			strcpy(disk.type, "disk");
			strcpy(disk.mountpoint, "-");
		} else {
			strcpy(disk.type, "part");
			sprintf(path, "/sys/block/%s/%s/mountpoint", entry->d_name, disk.name);

			mounts = fopen(path, "r");

			if (mounts == NULL) {
				print_message("Error get mountpoint", ERROR);
				strcpy(disk.mountpoint, "-");
			} else {
				fgets(disk.mountpoint, DEFAULT_BUFFER_SIZE, mounts);
				strtok(disk.mountpoint, "\n");
				fclose(mounts);
			}
		}

		print_disk(disk);
	}

	closedir(dir);

	return 1;
}

