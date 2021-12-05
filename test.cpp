#include "Wad.h"

int main(){
    Wad::loadWad("sample1.wad");

    TreeNode* root = new TreeNode(1,2,"matt", "/");
    cout << root->name << endl;
    root->children.push_back(new TreeNode(3,4,"blake","/matt/"));
    root->children.push_back(new TreeNode(5,6, "tony","/matt/"));
    cout << root->children[0]->name << endl;
    TreeNode* curr_node = root->children[0];
    curr_node->children.push_back(new TreeNode(5,6, "jeff", "/matt/blake/"));
    cout << root->children[1]->name << endl;
    cout << curr_node->children[0]->name << endl;
    return 0;
}
