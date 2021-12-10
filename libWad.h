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
		bool destroyed;	
		vector<TreeNode*> children;

		TreeNode(int offset, int length, string title, string path) {
			element_offset = offset;
			element_length = length;
			name = title;
			descriptor_path = path;
			destroyed = false;
		}

		TreeNode(int offset, int length, string title, string path, vector<TreeNode*> child) {
			element_offset = offset;
			element_length = length;
			name = title;
			descriptor_path = path;
			children = child;
			destroyed = false;
		}
		~TreeNode() {
			destroyed = true;
			for (int i = 0; i < children.size(); i++) {
				if (!children[i]->destroyed)
					delete(children[i]);
			}
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
		uint8_t* Data;
		TreeNode* my_root;
		vector<TreeNode*> all_nodes;
		string magic;
		string root_path;
};
