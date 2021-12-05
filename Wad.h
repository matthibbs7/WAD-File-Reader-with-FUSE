#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

class Descriptor {
	public:
		uint32_t eOffset;
		uint32_t length;
		std::string name;
		std::string path;
		Descriptor(uint32_t eOffset, uint32_t length, std::string name, std::string path);
		uint32_t getOffset();
		uint32_t getLength();
		std::string getName();
		std::string getPath();
};

class Wad {
	public:
		static Wad* loadWad(const string &path);
		uint8_t* pData;
		uint8_t numD;
		uint8_t dOffset;
		string magic;
		vector<Descriptor> disList;	
		string getMagic();
		bool isContent(const string &path);
		bool isDirectory(const string &path);
		int getSize(const string &path);
		int getContents(const string &path, char *buffer, int length, int offset = 0);
		int getDirectory(const string &path, vector<string> *directory);
		Wad(uint8_t *pdata);
		~Wad();
};
