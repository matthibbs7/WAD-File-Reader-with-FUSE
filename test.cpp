#include "libWad.h"

int main(){
    Wad* new_file = Wad::loadWad("sample1.wad");
    TreeNode* root = new_file->my_root;
    
    //cout << root->children[0]->children[0]->descriptor_path << endl;
    cout << new_file->isDirectory("/") << endl;
    cout << "TESTING getSize()" << endl;
    cout << new_file->getSize("/") << endl;
    //cout << "TESTING isContent()" << endl;
    //cout << new_file->isContent("/M_THERMM") << endl;
	

    //cout << "TESTING getDirectory()" << endl;
    vector<string> filler;
    cout << endl << new_file->getDirectory("/", &filler) << endl;
   cout << "PRINTING DIR VECTOR CONTENTS:" << endl;
    for (int i = 0; i < filler.size(); i++) {
	cout << filler[i] << endl;
    }
    return 0;
}
