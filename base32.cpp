#include <iostream>
#include <string>
using namespace std;
#define release

const string base32_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

string base32encode(string text) {
	string result;
	int a, b, c, d, e;
	int len = text.length();
	int nullbytes = (5-(len%5))*8/5;
	if (nullbytes == 8)
		nullbytes = 0;

	for (int i = 0; i < nullbytes; i++)
		text += '\0';

	for (int i = 0; i < len; i += 5) {
		a = text[i];
		b = text[i + 1];
		c = text[i + 2];
		d = text[i + 3];
		e = text[i + 4];
		result += base32_chars[a >> 3];
		result += base32_chars[((a & 0x7) << 2) + (b >> 6)];
		result += base32_chars[(b & 0x3F) >> 1];
		result += base32_chars[((b & 0x1)<<4)+((c&0xF0)>>4)];
		result += base32_chars[((c & 0xF)<<1)+(d>>7)];
		result += base32_chars[(d & 0x7C)>>2];
		result += base32_chars[((d & 0x3)<<3)+(e>>5)];
		result += base32_chars[e & 0x1F];
	}

	result.resize(result.length() - nullbytes);
	for (int i = 0; i < nullbytes; i++)
		result += '=';

	return result;
}
string base32decode(string text) {
	string result;
	int a, b, c, d, e, f, g, h;
	int len = text.length();
	int nullbytes = 0;

	for (int i = 0; i < len; i++) {
		if (text[i] == '=') {
			text[i] ='\0';	
			nullbytes++;
		}
		else text[i] = char(base32_chars.find(text[i]));
	}
	
	for (int i = 0; i < len; i += 8) {
		a = text[i];	
		b = text[i + 1]; 
		c = text[i + 2]; 
		d = text[i + 3]; 
		e = text[i + 4]; 
		f = text[i + 5]; 
		g = text[i + 6]; 
		h = text[i + 7]; 
		result += (a << 3) + ((b & 0x1C)>>2); 
		result += ((b & 0x3)<<6) + (c<<1) + (d>>4); 
		result += ((d & 0xF) << 4) + (e>>1);
		result += ((e & 0x1) << 7) + (f <<2) + (g>>3);
		result += ((g & 0x7) << 5) + h;
	}
	return result;
}

int main(int argc, char* argv[]) {

	string msg = "Base32 encoder/decoder\nHow to use:\n\t-e <text>\t-\tencode text\n\t-d <text>\t-\tdecode text";
	
	#ifdef debug 
	cout << "base32: " << base32decode("GEZDGNBVGV5HU6T2LI======");
	#else
	
	if (argc != 3) {
		cout << msg;
		return -1;
	}
	string flag = argv[1];
	string text = argv[2];

	if (flag == "-e")
		cout << "base32: " << base32encode(text);
	else if (flag == "-d")
		cout << "decoded text:" << base32decode(text);
	else {
		cout << msg;
		return -2;
	}

	#endif
	return 0;
}
