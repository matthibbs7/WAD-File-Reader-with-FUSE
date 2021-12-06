wad: Wad.cpp test.cpp 
	g++ -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -Wall -o wad Wad.cpp test.cpp -lfuse
clean: 
	rm wad 

