#include <iostream>
#include "Wad.h"

using namespace std;

void exploreDirectory(Wad *data, const string path, int level)
{
    for (int index = 0; index < level; index++)
        cout << " ";

    vector<string> entries;
    cout << "[Objects at this level:" << data->getDirectory(path, &entries) << "]" << endl;

    for (string entry : entries)
    {
        string entryPath = path + entry;

        for (int index = 0; index < level; index++)
            cout << " ";

        if (data->isDirectory(entryPath))
        {
            cout << level << ". DIR: " << entry << endl;
            exploreDirectory(data, entryPath + "/", level + 1);
        }
        else if (data->isContent(entryPath))
            cout << level << ". CONTENT: " << entry << "; Size: " << data->getSize(entryPath) << endl;
        else
            cout << "***WARNING: entry " << entry << " has invalid type!***" << endl;
    }
}

void exploreDirectory(Wad *data, const string path)
{
    cout << "EXPLORING: " << path << endl;
    exploreDirectory(data, path, 1);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "No file specified. Exiting." << endl;
        exit(EXIT_SUCCESS);
    }

    Wad *myWad = Wad::loadWad(argv[1]);
    exploreDirectory(myWad, "/");
}
