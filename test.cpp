#include "libWad.h"

int main(){
    Wad* new_file = Wad::loadWad("sample1.wad");
    TreeNode* root = new_file->my_root;
    
    //cout << root->children[0]->children[0]->descriptor_path << endl;
    cout << new_file->isDirectory("/") << endl;
    cout << "TESTING getSize()" << endl;
    cout << new_file->getSize("/E1M0/01.txt") << endl;
    //cout << "TESTING isContent()" << endl;
    //cout << new_file->isContent("/M_THERMM") << endl;
    cout << "TESTING getContent()" << endl;
    char* buffer = (char*) malloc(17);
    cout << new_file->getContents("/mp.txt", buffer, 398, 0) << endl;
    //printf("%s", buffer);
	//cout << buffer[0] << endl;
	std::string Str = std::string(buffer);
    cout << Str << endl;

    //cout << "TESTING getDirectory()" << endl;
   // vector<string> filler;
    //cout << endl << new_file->getDirectory("/", &filler) << endl;
   //cout << "PRINTING DIR VECTOR CONTENTS:" << endl;
    //for (int i = 0; i < filler.size(); i++) {
//	cout << filler[i] << endl;
  //  }
    return 0;
}
