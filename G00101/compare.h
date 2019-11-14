#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

enum compare_result{CMP_IDENTICAL,CMP_DIFFERENT};

int compare_dirs(char *path1,char *path2);
