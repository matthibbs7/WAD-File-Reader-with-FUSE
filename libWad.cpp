#include "libWad.h"

Wad::Wad(uint8_t *pData){
    unsigned int    header_desc_num,    //header descriptor number
                    header_desc_off,    //header descriptors offset
                    check_desc_num = 1, //compare descriptor number to read value
                    i;                  //counter variables
    string          header_magic;       //header magic (file type)
    Data = pData;
    //Magic (File Type)
    for (i = 0; i < 4; i++){
        header_magic += pData[i];     //store magic header
    }
    magic = header_magic;

    //Number of Descriptors
    header_desc_num = int((unsigned char)(pData[4]) |   
                          (unsigned char)(pData[5]) << 8 | 
                          (unsigned char)(pData[6]) << 16 |
                          (unsigned char)(pData[7]) << 24);

    //Descriptor Offset
    header_desc_off = int((unsigned char)(pData[8]) |
                          (unsigned char)(pData[9]) << 8 |
                          (unsigned char)(pData[10]) << 16 |
                          (unsigned char)(pData[11]) << 24);

    int offset = header_desc_off;

    string mapMarker = "";
    int ten_elements = 0, level = 0;
    string currPath = "/";
    TreeNode* root = new TreeNode(0,0,"root","/");
    TreeNode* curr_node = root;

    //Descriptors
    int curr_level; 
    for (i = 0; i < header_desc_num; i++){   //loop through number of descriptors
        int curr_element_offset = 0;
        int curr_element_length = 0;
        string name = "";

        // Element Offset
        curr_element_offset = int((unsigned char)(pData[offset]) |
                      (unsigned char)(pData[offset+1]) << 8 |
                      (unsigned char)(pData[offset+2]) << 16 |
                      (unsigned char)(pData[offset+3]) << 24);

        // Element Length
        curr_element_length = int((unsigned char)(pData[offset+4]) |
                      (unsigned char)(pData[offset+5]) << 8 |
                      (unsigned char)(pData[offset+6]) << 16 |
                      (unsigned char)(pData[offset+7]) << 24);

        // Descriptor Name
        for (int j = 0; j < 8; j++) {
            name += pData[offset+j+8];
        }

        //debug
 	//cout << currPath + name << endl;       
        // If it is a marker element
        if (ten_elements > 0) {
            //int counter = count(currPath.begin(), currPath.end(), '/');
            curr_level = 0;
            curr_node = root;

            while (curr_level < level) {
                curr_node = curr_node->children[curr_node->children.size()-1];
                curr_level++;
            }

	    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath + name);
            curr_node->children.push_back(temp);
            all_nodes.push_back(temp);
	    ten_elements--;
            if (ten_elements == 0) {
                level--;
                int pathSize = currPath.length();
                for (int k = pathSize-2; k > -1; k--) {
                    if (currPath[k] == '/') {
                        currPath.pop_back();
                        break;
                    }
                    currPath.pop_back();
                }
                offset += 16;
                continue;
            }
        }
	string name_copy = name;
	//name_copy = replaceAll(name_copy, {'\0'}, "");
	//cout << "NAME LENGTH2 : " << name.length() << "LAST CHAR DETECTED: " << name_copy[name_copy.length()-1] << endl;
        // If it is directory
        if (curr_element_length == 0) {
            // Start 10 element Dir
            if (name[0] == 'E' && isdigit(name[1]) && name[2] == 'M' && isdigit(name[3])) {
                curr_level = 0;
                curr_node = root;
                
                // at root '/'
                if (level == 0) {
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath += (name + '/'));
                    curr_node->children.push_back(temp);
	  	    all_nodes.push_back(temp);
                }
                // create root in subdirectory ex: '/example/HERE'
                else {
                    while (curr_level < level) {
                        curr_node = curr_node->children[curr_node->children.size()-1];
                        curr_level++;
                    }
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath += (name + '/'));
                    curr_node->children.push_back(temp);
		    all_nodes.push_back(temp);
                    
		}
            
                ten_elements = 10;
                mapMarker = name;
                level++;
            } // Open Dir
            else if ((name[1] == '_' && name[2] == 'S' && name[3] == 'T' && name[4] == 'A' && name[5] == 'R' && name[6] == 'T') || (name[2] == '_' && name[3] == 'S' && name[4] == 'T' && name[5] == 'A' && name[6] == 'R' && name[7] == 'T')) {
                curr_level = 0;
                curr_node = root;
		
		string name2 = name;
		name2.replace(name2.find("_START"),6, "");
		//cout << "New dir name is: " << endl;
		//cout << name2 << endl;
		
                if (level == 0) {
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name2, currPath += (name2 + '/'));
		    curr_node->children.push_back(temp);
               	    all_nodes.push_back(temp);
		} else {
                    while (curr_level < level) {
                        curr_node = curr_node->children[curr_node->children.size()-1];
                        curr_level++;
                    }
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name2, currPath += (name2 + '/'));
                    curr_node->children.push_back(temp);
                    all_nodes.push_back(temp);
		}

                level++;
            } // End Dir
            else if ((name[1] == '_' && name[2] == 'E' && name[3] == 'N' && name[4] == 'D') || (name[2] == '_' && name[3] == 'E' && name[4] == 'N' && name[5] == 'D')) {
                level--;
                int pathSize = currPath.length();
                for (int k = pathSize-2; k > -1; k--) {
                    if (currPath[k] == '/') {
                        currPath.pop_back();
                        break;
                    }
                    currPath.pop_back();
                }
            }
        } else if (ten_elements==0) {
            curr_level = 0;
            curr_node = root;
            if (level == 0) {
		TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath + name);
                curr_node->children.push_back(temp);
		all_nodes.push_back(temp);
            } else {
                while (curr_level < level) {
                    curr_node = curr_node->children[curr_node->children.size()-1];
                    curr_level++;
                }
		TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath +name);
                curr_node->children.push_back(temp);
		all_nodes.push_back(temp);
            }
        }
        offset += 16;
        check_desc_num++;   //increment desc num check
    }

    my_root = root;
}

Wad* Wad::loadWad(const string &path){
    ifstream wadFile(path, std::ios::binary | std::ios::ate);
    streamsize wadFilesize = wadFile.tellg();
    uint8_t *pData = new uint8_t[(unsigned int)wadFilesize];
    wadFile.seekg(0, std::ios::beg);
    wadFile.read((char *)pData, wadFilesize);
    return new Wad(pData);
}

string Wad::getMagic(){
    return magic;
}

bool Wad::isContent(const string &path){
    	// /dir1/sample1.txt
	if (isDirectory(path)) return false;
	string path_copy;
	for (int i = 0; i < all_nodes.size(); i++) {
		path_copy = all_nodes[i]->descriptor_path;
		path_copy.erase(remove(path_copy.begin(),path_copy.end(),'\0'), path_copy.end());
		if (path_copy.compare(path) == 0 && all_nodes[i]->element_length != 0) return true;
	}
	return false;
}

bool Wad::isDirectory(const string &path){
	if (path.compare("/") == 0) return true;
	string path_copy;
	for (int i = 0; i < all_nodes.size(); i++) {
		// store copy of current element path
		path_copy = all_nodes[i]->descriptor_path;
		// remove trailing null chars from path
		path_copy.erase(remove(path_copy.begin(),path_copy.end(), '\0'), path_copy.end());
		// this will allow the directory format to be either /dir or /dir/
		if (path_copy.length() - 1 > path.length()) continue;
		// compress string so compare will be accurate
		path_copy = path_copy.substr(0, path.length());
		if (path_copy.compare(path) == 0 && all_nodes[i]->element_length == 0) {
			return true;
		}
	}
    return false;
}

int Wad::getSize(const string &path){
    	if (isDirectory(path)) return -1;
	string path_copy;
	for (int i = 0; i < all_nodes.size(); i++) {
		path_copy = all_nodes[i]->descriptor_path;
		path_copy.erase(remove(path_copy.begin(), path_copy.end(), '\0'), path_copy.end());
		if (path_copy.compare(path) == 0 && all_nodes[i]->element_length !=0) {
			return all_nodes[i]->element_length;
		}
	}

	return -1;
}



int Wad::getContents(const string &path, char *buffer, int length, int offset){
	cout << "GET CONTENTS CALLED" << endl;
	if (isDirectory(path)) return -1;
	string path_copy;
	for (int i = 0; i < all_nodes.size(); i++) {
		path_copy = all_nodes[i]->descriptor_path;
		path_copy.erase(remove(path_copy.begin(), path_copy.end(), '\0'), path_copy.end());
		// found file
		///cout << path_copy << endl;
		if (path.compare(path_copy.substr(0,path.length())) == 0) {
			//cout << "found file" << endl;
			//buffer = (char*)malloc(length*(sizeof(char)));
			for (int k = offset; k < length; k++) {
				buffer[k] = Data[all_nodes[i]->element_offset + k];
			}
			//if (all_nodes[i]->element_length < offset + length) {
			//	length = all_nodes[i]->element_length - offset;
			//}
			
			return length;
		}
	}
	return -1;
}

//int Wad::getDirectory(const string &path, vector<string> *directory) {

//}

int Wad::getDirectory(const string &path, vector<string> *directory){
    	string path_copy;
	int num_elements = 0;
	if (path.compare("/") == 0) {
		int level = 0;
		for (int i = 0; i < all_nodes.size(); i++) {
			path_copy = all_nodes[i]->descriptor_path;
			path_copy.erase(remove(path_copy.begin(), path_copy.end(), '\0'), path_copy.end());
			if (std::count(path_copy.begin(), path_copy.end(), '/') > (level+2)) continue;
			if (!isDirectory(path_copy) && std::count(path_copy.begin(), path_copy.end(), '/') > (level+1)) continue;
			num_elements++;
			path_copy.erase(0,1);
			if (path_copy[path_copy.length()-1] == '/') path_copy.pop_back();
			//cout << "Added: " << path_copy << " " << path_copy.length() << " to vector" << endl;
			//cout << "Compared with name: " << all_nodes[i]->name << " " << all_nodes[i]->name.length() << endl;
			directory->push_back(path_copy);
		}
		return num_elements;
	}
	if (isDirectory(path)) {
		int level = std::count(path.begin(), path.end(), '/');
		for (int i = 0; i < all_nodes.size(); i++) {
			path_copy = all_nodes[i]->descriptor_path;
			path_copy.erase(remove(path_copy.begin(), path_copy.end(), '\0'), path_copy.end());
			string path_test = path_copy;
			path_test.pop_back();
			if (path_copy.compare(path) != 0 && path_copy.substr(0, path.length()).compare(path) == 0) {
				if (path_test.compare(path) == 0) continue;
				if (std::count(path_copy.begin(), path_copy.end(), '/') > (level+1)) continue;
				if (!isDirectory(path_copy) && std::count(path_copy.begin(), path_copy.end(), '/') > (level)) continue;
				num_elements++;
				//cout << "Added: " << path_copy << " " << path_copy.length() << " to vector compared" << endl;
				
				string title = all_nodes[i]->name;
				title.erase(remove(title.begin(), title.end(), '\0'), title.end());
				//cout << "With name: " << all_nodes[i]->name << " " << title.length() << endl;
				directory->push_back(title);

			}
		}
		return num_elements;
	}
	return -1;
}
/*
int Wad::getDirectory(const string &path, vector<string> *directory) {
	if (isContent(path)) return -1;
	cout << "test" << endl;
	string copy_path;
	if (path.compare("/") != 0) {
	for (int i = 0; i < all_nodes.size(); i++) {
		copy_path = all_nodes[i]->descriptor_path;
		copy_path.erase(remove(copy_path.begin(), copy_path.end(), '\0'), copy_path.end());
		cout << copy_path << endl;
		if (path.compare(copy_path.substr(0,path.length())) == 0) {
			string title;
			for (int k = 0; k < all_nodes[i]->children.size(); k++) {
				title = all_nodes[i]->children[k]->name;
				title.erase(remove(title.begin(), title.end(), '\0'), title.end());
				directory->push_back(title);
			}
		}
	}
	}
}
*/
