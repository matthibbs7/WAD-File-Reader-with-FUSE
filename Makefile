all: clean libWad test wadfs #default
	#umount mount
	sudo chmod 666 /dev/fuse
	./waddump DOOM1.WAD
	#./test
	./wadfs sample1.wad mount/
	mount | tail -n1
libWad: libWad.cpp  #library
	g++ -Wall -c libWad.cpp
	ar rcs libWad.a libWad.o
wadfs: wadfs.cpp    #wadfs fuse
	g++ -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -Wall -o wadfs wadfs.cpp libWad.a -lfuse
test: test.cpp      #test
	g++ -Wall -o waddump wad_dump.cpp libWad.a -lfuse
clean: 
	rm -f libWad.a libWad.o wadfs test
