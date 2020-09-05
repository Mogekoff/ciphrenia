#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int invertBytes(const string path) {	
	fstream file_in(path, ios::in | ios::binary);
	fstream file_out;
	string newPath;
	char byte[1];

	if(path.substr(path.length()-3,3) == "enc")
		newPath = path.substr(0, path.length() - 4);
	else
		newPath = path + ".enc";
	
	file_out.open(newPath, ios::out | ios::binary);
	if (!(file_in.is_open() && file_out.is_open())) {
		cout << "Can't open file " << path;
		return -2;
	}
		
	cout<<"Input file:\t"<<path<<"\nOutput file:\t"<<newPath<<endl;
	
	while(true) {

		file_in.read(byte,1);
		if(file_in.eof())
			break;
		*byte=~*byte;
		file_out.write(byte,1);
	}

	file_in.close();
	file_out.close();
	return 0;
}

int main(int argc, char* argv[])
{
	string msg = "Invert byte encoder/decoder\nHow to use:\n\tinvert <file path>\t-\tencode/decode text";
	if (argc != 2) {
		cout << msg;
		return -1;
	}
	string path = argv[1];

	return invertBytes(path);
}
