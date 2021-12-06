#include "Wad.h"

int main(){
    Wad* new_file = Wad::loadWad("sample1.wad");
    TreeNode* root = new_file->my_root;
    
    cout << "FILE TREE" << endl;
    cout << root->children[0]->children[0]->descriptor_path << endl;
    cout << "GET MAGIC" << endl;
    cout << new_file->getMagic();
    return 0;
}
