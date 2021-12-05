#include "Wad.h"

Wad::Wad(uint8_t *pData){
    unsigned int    header_desc_num,    //header descriptor number
                    header_desc_off,    //header descriptors offset
                    i;                  //counter variables
    string          header_magic;       //header magic (file type)

    //Magic (File Type)
    for (i = 0; i < 4; i++){
        //debug
        cout << "debug> pData[" << i <<  "]: " << pData[i] << endl;

        header_magic += pData[i];     //store magic header
    }

    //Number of Descriptiors
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

    //Descriptors 
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
	cout << name << endl;
	offset += 16;
    }
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
    return 0;
}

bool Wad::isContent(const string &path){
    return 0;
}

bool Wad::isDirectory(const string &path){
    return 0;
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
