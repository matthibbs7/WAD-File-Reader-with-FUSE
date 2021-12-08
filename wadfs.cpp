#include <fuse.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/types.h>
#include "libWad.h"

Wad* fileWad;

static int getattr_callback(const char *path, struct stat *st){
	memset(st, 0, sizeof(struct stat));
	std::string copy_path = std::string(path);
	//void * buf = NULL;
	//filler(buf, copy_path.c_str(), NULL, 0);
	
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

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	vector<string> entries;
	fprintf(stdout, "test");
	string copy_path(path);
	if (copy_path.length() >= 1 && copy_path.substr(copy_path.length() - 1) != "/") {
		copy_path = copy_path + "/";
	}
	if (copy_path.size() == 0) {
		copy_path = "/";
	}
	fileWad->getDirectory(copy_path, &entries);
	string test = "hello world";
	//filler(buf, test.c_str(), NULL, 0);
	for (string entry : entries) {
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

static struct fuse_operations fuse_example_operations;

int main(int argc, char *argv[]) {
	fuse_example_operations.getattr = getattr_callback;
	fuse_example_operations.open = open_callback;
	fuse_example_operations.read = read_callback;
	fuse_example_operations.readdir = readdir_callback;
	fuse_example_operations.opendir = opendir_callback;
	fuse_example_operations.release = release_callback;
	fuse_example_operations.releasedir = releasedir_callback;

	fileWad = Wad::loadWad(argv[1]);
	argv[1] = argv[2];
	argv[2] = NULL;
	cout << "finished delete arg 2" << endl;
	cout << "directory " << fileWad->isDirectory("/") << endl;
	
	return fuse_main(--argc, argv, &fuse_example_operations, NULL);
}

