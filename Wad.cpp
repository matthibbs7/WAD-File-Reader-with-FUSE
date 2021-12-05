#include "Wad.h"

Wad* Wad::loadWad(const string &path)
string Wad::getMagic()
bool Wad::isContent(const string &path)
bool Wad::isDirectory(const string &path) 
int Wad::getSize(const string &path)
int Wad::getContents(const string &path, char *buffer, int length, int offset = 0)
int Wad::getDirectory(const string &path, vector<string> *directory) 
