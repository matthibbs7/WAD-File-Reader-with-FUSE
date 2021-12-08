#include <fuse.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/types.h>
#include "libWad.h"

Wad* fileWad;

static int do_getattr(const char *path, struct stat *st){
	if (strcmp(path, "/") == 0) {
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2;
	} else {
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	return 0;
}
