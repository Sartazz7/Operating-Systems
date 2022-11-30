#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_FILE_HANDLES 20
#define MAX_DIR_HANDLES 20
#define MAX_MOUNT_POINTS 10
#define MAX_ENTITY_NAME 8

/*-----------DEVICE------------*/
int opendevice(char *device_name);
int closedevice(int mount_point);

// /*-----------FILE SYSTEM API------------*/
void fsdump(int mount_point);
void filesystem_check(int mount_point);