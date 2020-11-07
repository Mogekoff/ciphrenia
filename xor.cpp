#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int xorBytes(const string path,const string key) {	
	fstream file_in(path, ios::in | ios::binary);
	fstream file_out;
	string newPath;
	char byte[1];

	if(path.substr(path.length()-3,3) == "xor")
		newPath = path.substr(0, path.length() - 4);
	else
		newPath = path + ".xor";
	
	file_out.open(newPath, ios::out | ios::binary);
	if (!(file_in.is_open() && file_out.is_open())) {
		cout << "Can't open file " << path;
		return -2;
	}
		
	cout<<"Input file:\t"<<path<<"\nOutput file:\t"<<newPath<<endl;
	
	for(int i = 0; true; i++) {
		if(i==key.length()) i=0;
		
		file_in.read(byte,1);
		if(file_in.eof())
			break;
		*byte^=key[i];
		file_out.write(byte,1);
	}

	file_in.close();
	file_out.close();
	return 0;
}

int main(int argc, char* argv[])
{
	string msg = "Xor byte encrypt/decrypt\nHow to use:\n\txorbytes <file path> <key>\t-\tencrypt/decrypt file";
	if (argc != 3) {
		cout << msg;
		return -1;
	}
	string path = argv[1];
	string key = argv[2];

	return xorBytes(path,key);
}
