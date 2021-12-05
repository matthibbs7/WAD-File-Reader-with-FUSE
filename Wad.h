#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

class Wad {
	public:
		static Wad* loadWad(const string &path);
		string getMagic();
		bool isContent(const string &path);
		bool isDirectory(const string &path);
		int getSize(const string &path);
		int getContents(const string &path, char *buffer, int length, int offset = 0);
		int getDirectory(const string &path, vector<string> *directory);
		Wad(uint8_t *pdata);
		~Wad();
};
