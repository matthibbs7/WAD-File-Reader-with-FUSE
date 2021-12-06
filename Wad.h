#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>
#include <list>
#include <iterator>

using namespace std;

class TreeNode {
	public:
		int element_offset;
		int element_length;
		string name;
		string descriptor_path;
	
		vector<TreeNode*> children;

		TreeNode(int offset, int length, string title, string path) {
			element_offset = offset;
			element_length = length;
			name = title;
			descriptor_path = path;
		}

		TreeNode(int offset, int length, string title, string path, vector<TreeNode*> child) {
			element_offset = offset;
			element_length = length;
			name = title;
			descriptor_path = path;
			children = child;
		}
};

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

		TreeNode* my_root;
		string magic;
		string root_path;
};
