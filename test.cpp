#include "libWad.h"

int main(){
    Wad* new_file = Wad::loadWad("DOOM1.WAD");
    TreeNode* root = new_file->my_root;
    
    cout << "FILE TREE" << endl;
    //cout << root->children[0]->children[0]->descriptor_path << endl;
    cout << "GET MAGIC" << endl;
    cout << new_file->getMagic() << endl;
    return 0;
}
