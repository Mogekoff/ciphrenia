#include <iostream>
#include <string>
using namespace std;

const string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

string base64encode(string text) {
	string result;
	int a,b,c;
	int len = text.length();
	int nullbytes = 3-len%3;
	if(nullbytes==3) 
		nullbytes=0;
	
	for(int i = 0; i<nullbytes; i++)
		text+='\0';

	for (int i = 0; i < len; i+=3) {
		a=text[i];
		b=text[i+1];
		c=text[i+2];
		result += base64_chars[a >> 2];
		result += base64_chars[(((a & 0x3) << 6) + (b >> 2)) >> 2];
		result += base64_chars[((b & 0xF) << 2) + (c >> 6)];
		result += base64_chars[c & 0x3F];		
	}

	result.resize(result.length()-nullbytes);
	for (int i = 0; i < nullbytes; i++)
		result += '=';

	return result;
}
string base64decode(string text) {
	string result;
	int a, b, c, d;
	int len = text.length();
	int nullbytes = 0;

	for (int i = 0; i<len; i++) {
		if (text[i] == '=')
			nullbytes++;
		text[i]=char(base64_chars.find(text[i]));			
	}
		
	for (int i = 0; i < len; i += 4) {
		a = text[i];
		b = text[i + 1];
		c = text[i + 2];
		d = text[i + 3];
		result += (a << 2) + (b >> 4);
		result += ((b&0xF)<<4) + (c>>2);
		result += ((c&0x3)<<6)+d;		
	}

	result.resize(result.length() - nullbytes);
	return result;
}

int main(int argc, char* argv[]) {
	
	string msg = "Base64 encoder/decoder\nHow to use:\n\t-e <text>\t-\tencode text\n\t-d <text>\t-\tdecode text";

	if (argc != 3) {
		cout<<msg;
		return -1;
	}	
	string flag = argv[1];
	string text = argv[2];

    if(flag =="-e")
        cout<<"base64: "<<base64encode(text);
	else if(flag =="-d")
		cout<<"decoded text:"<<base64decode(text);
	else {
		cout<<msg;
		return -2;	
	}
	return 0;
}