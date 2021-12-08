all: clean libWad wadfs #default
	#umount mount
	sudo chmod 666 /dev/fuse
	#./test
	./wadfs DOOM1.WAD mount/
	mount | tail -n1
libWad: libWad.cpp  #library
	g++ -Wall -c libWad.cpp
	ar rcs libWad.a libWad.o
wadfs: wadfs.cpp    #wadfs fuse
	g++ -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -Wall -o wadfs wadfs.cpp libWad.a -lfuse
test: test.cpp      #test
	g++ -Wall -o waddump waddump.cpp libWad.a -lfuse
clean: 
	rm -f libWad.a libWad.o wadfs test
