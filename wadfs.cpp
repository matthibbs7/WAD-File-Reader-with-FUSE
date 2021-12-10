#include <fuse.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/types.h>
#include "libWad.h"

Wad* fileWad;
static struct fuse_operations my_fuse;

static int getattr_callback(const char *path, struct stat *st){
	// allocate memory for the fuse struct
	memset(st, 0, sizeof(struct stat));
	// create string obj from char*
	std::string copy_path = std::string(path);
	
	// if we have a directory set permissions
	if (fileWad->isDirectory(copy_path)) {
		st->st_mode = S_IFDIR | 0555;
		st->st_nlink = 2;
		return 0;
	}

	if (fileWad->isContent(copy_path)) {
		st->st_mode = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size = fileWad->getSize(copy_path);
		return 0;
	}
	return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	(void) offset;
	(void) fi;

	vector<string> elements;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	
	string copy_path(path);
	if (copy_path.length() >= 1 && copy_path.substr(copy_path.length() - 1) != "/") {
		copy_path = copy_path + "/";
	}
	if (copy_path.size() == 0) {
		copy_path = "/";
	}
	fileWad->getDirectory(copy_path, &elements);
	for (string entry : elements) {
		filler(buf, entry.c_str(), NULL, 0);
	}

	return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
	return 0;
}

static int opendir_callback(const char *path, struct fuse_file_info *fi) {
	return 0;
}

static int release_callback(const char *path, struct fuse_file_info *fi) {
	return 0;
}

static int releasedir_callback(const char *path, struct fuse_file_info *fi) {
	return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	if (fileWad->isContent(path)) {
		return fileWad->getContents(path, buf, size, offset); 
	}

	return -ENOENT;
}
int main(int argc, char *argv[]) {
	my_fuse.getattr = getattr_callback;
	my_fuse.open = open_callback;
	my_fuse.read = read_callback;
	my_fuse.readdir = readdir_callback;
	my_fuse.opendir = opendir_callback;
	my_fuse.release = release_callback;
	my_fuse.releasedir = releasedir_callback;

	fileWad = Wad::loadWad(argv[1]);
	argv[1] = argv[2];
	argv[2] = NULL; 
	fuse_main(--argc, argv, &my_fuse, NULL);
	delete(fileWad);
}

