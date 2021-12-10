#include "libWad.h"

int main(){
    Wad* new_file = Wad::loadWad("sample1.wad");
    TreeNode* root = new_file->my_root;
 	cout << "Deleting stuff" << endl;
	//new_file->~Wad();
	delete(new_file);   
//	cout << filler[i] << endl;
  //  }
    return 0;
}
