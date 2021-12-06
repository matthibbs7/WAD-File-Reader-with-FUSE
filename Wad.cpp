#include "Wad.h"

Wad::Wad(uint8_t *pData){
    unsigned int    header_desc_num,    //header descriptor number
                    header_desc_off,    //header descriptors offset
                    check_desc_num = 1, //compare descriptor number to read value
                    i;                  //counter variables
    string          header_magic;       //header magic (file type)

    //Magic (File Type)
    for (i = 0; i < 4; i++){
        //debug
        cout << "debug> pData[" << i <<  "]: " << pData[i] << endl;

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

    //debug
    cout << "debug> header_magic: " << header_magic << endl;
    cout << "debug> header_desc_num: " << header_desc_num << endl;
    cout << "debug> header_desc_off: " << header_desc_off << endl;

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
        cout << currPath + name << endl;
        
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
            else if (name[name.length()-6] == '_' && name[name.length()-5] == 'S' && name[name.length()-4] == 'T' && name[name.length()-3] == 'A' && name[name.length()-2] == 'R' && name[name.length()-1] == 'T') {
                curr_level = 0;
                curr_node = root;
                if (level == 0) {
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath += (name + '/'));
                    curr_node->children.push_back(temp);
               	    all_nodes.push_back(temp);
		} else {
                    while (curr_level < level) {
                        curr_node = curr_node->children[curr_node->children.size()-1];
                        curr_level++;
                    }
		    TreeNode* temp = new TreeNode(curr_element_offset, curr_element_length, name, currPath += (name + '/'));
                    curr_node->children.push_back(temp);
                    all_nodes.push_back(temp);
		}

                level++;
            } // End Dir
            else if (name[name.length()-6] == '_' && name[name.length()-5] == 'E' && name[name.length()-4] == 'N' && name[name.length()-3] == 'D') {
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

    //debug
    if (check_desc_num != header_desc_num)
        cout << "ERROR descriptor number: " << check_desc_num << " != " << header_desc_num << endl;
    else
        cout << "debug> descriptor number: " << check_desc_num << " = " << header_desc_num << endl;

    my_root = root;
}

Wad* Wad::loadWad(const string &path){
    ifstream wadFile(path, std::ios::binary | std::ios::ate);
    streamsize wadFilesize = wadFile.tellg();
    uint8_t *pData = new uint8_t[(unsigned int)wadFilesize];
    wadFile.seekg(0, std::ios::beg);
    wadFile.read((char *)pData, wadFilesize);

    //debug
    cout << "debug> filesize: " << wadFilesize << endl;

    return new Wad(pData);
}

string Wad::getMagic(){
    return magic;
}

bool Wad::isContent(const string &path){
    	// /dir1/sample1.txt

	return 0;
}

bool Wad::isDirectory(const string &path){

	cout << all_nodes[6]->descriptor_path << endl;
	if ((path).compare(all_nodes[6]->descriptor_path.substr(0, path.length())) == 0) {
		cout << "success" << endl;
	}
	cout << "Sizes: "<< endl;
	cout << (path + '/').length() << endl;
	cout << all_nodes[6]->descriptor_path.substr(0, path.length()+1).length() << endl;
	cout << "Original path + / : " << (path) << endl;
	cout << "New Path: " << all_nodes[6]->descriptor_path.substr(0, path.length()) << endl;
	cout << "New Path without subtr: " << all_nodes[6]->descriptor_path << endl;	
	//for (int i = 0; i < all_nodes.size(); i++) {
	//	cout << "PATH " << all_nodes[i]->descriptor_path << endl;
	//	if (path + '/' == all_nodes[i]->descriptor_path) {
	//		cout << "PATH FOUND" << endl;
	//		if (all_nodes[i]->element_length == 0) {
	//			return true;
	//		}
	//	}
	//}

    return false;
}

int Wad::getSize(const string &path){
    return 0;
}

int Wad::getContents(const string &path, char *buffer, int length, int offset){
    return 0;
}

int Wad::getDirectory(const string &path, vector<string> *directory){
    return 0;
}
