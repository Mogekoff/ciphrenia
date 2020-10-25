#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;
#define int32 unsigned int
#define int64 unsigned long long
#define byte unsigned char

//Буферы
int32 A; // 67452301h
int32 B; // EFCDAB89h
int32 C; // 98BADCFEh
int32 D; // 10325476h

int32 T[64];

//Логические функции
int32 F(int32 X, int32 Y, int32 Z) { return (X & Y) | (~X & Z); }
int32 G(int32 X, int32 Y, int32 Z) { return (X & Z) | (~Z & Y); }
int32 H(int32 X, int32 Y, int32 Z) { return X^Y^Z; }
int32 I(int32 X, int32 Y, int32 Z) { return Y^(~Z|X); }

//Функция для сдвига влево
int32 shiftLeft(int32 x, int32 n) {
    return (x<<n) | (x>>(32-n));
}

void addingLenght(vector<byte>& buf, const int64 len) {
    
    for (auto i = 7; i >= 0; i--)
        buf.push_back(byte((len >> (i * 8)) & 0xFF));
}

void addingNulls(vector<byte>& buf) {
    do
        buf.push_back(0);
    while (buf.size() % 64 != 56);
}

void initMDbuf(){
    A = 0x67452301; 
    B = 0xEFCDAB89; 
    C = 0x98BADCFE;
    D = 0x10325476;
    for (auto i = 0; i < 64; i++)
        T[i] = int32(pow(2, 32) * abs(sin(double(i + 1))));
    
}

void adding1byte(vector<byte>& buf) {
    buf.push_back(128);   //128 = 1000 0000
}

int conv512to32(vector<byte>& buf, int32*& words){
    auto wSize = buf.size();
    words = new int32[wSize/4];

    for (auto i = 0, j=0; i < wSize; i+=4, j=i/4) {
        words[j] = 0;
        words[j] += int32(buf[i]);
        words[j] += int32(buf[i+1])<<8;
        words[j] += int32(buf[i+2])<<16;
        words[j] += int32(buf[i+3])<<24;
    }
    
    return wSize/4;
}

void calcBlocks(int32 *buf, const int len) {
    for(auto i = 0; i < len; i+=16) {
        auto AA=A, BB=B, CC=C, DD=D;

        //ROUND I

        A = B + shiftLeft((A + F(B, C, D) + buf[i + 0] + T[0]), 7);
        D = A + shiftLeft((D + F(A, B, C) + buf[i + 1] + T[1]), 12);
        C = D + shiftLeft((C + F(D, A, B) + buf[i + 2] + T[2]), 17);
        B = C + shiftLeft((B + F(C, D, A) + buf[i + 3] + T[3]), 22);

        A = B + shiftLeft((A + F(B, C, D) + buf[i + 4] + T[4]), 7);
        D = A + shiftLeft((D + F(A, B, C) + buf[i + 5] + T[5]), 12);
        C = D + shiftLeft((C + F(D, A, B) + buf[i + 6] + T[6]), 17);
        B = C + shiftLeft((B + F(C, D, A) + buf[i + 7] + T[7]), 22);
        
        A = B + shiftLeft((A + F(B, C, D) + buf[i + 8] + T[8]), 7);
        D = A + shiftLeft((D + F(A, B, C) + buf[i + 9] + T[9]), 12);
        C = D + shiftLeft((C + F(D, A, B) + buf[i + 10] + T[10]), 17);
        B = C + shiftLeft((B + F(C, D, A) + buf[i + 11] + T[11]), 22);

        A = B + shiftLeft((A + F(B, C, D) + buf[i + 12] + T[12]), 7);
        D = A + shiftLeft((D + F(A, B, C) + buf[i + 13] + T[13]), 12);
        C = D + shiftLeft((C + F(D, A, B) + buf[i + 14] + T[14]), 17);
        B = C + shiftLeft((B + F(C, D, A) + buf[i + 15] + T[15]), 22);

        //ROUND II

        A = B + shiftLeft((A+G(B, C, D)+buf[i+1]+T[16]), 5);
		D = A + shiftLeft((D+G(A, B, C)+buf[i+6]+T[17]), 9);
		C = D + shiftLeft((C+G(D, A, B)+buf[i+11]+T[18]), 14);
		B = C + shiftLeft((B+G(C, D, A)+buf[i+0]+T[19]), 20);

		A = B + shiftLeft((A+G(B, C, D)+buf[i+5]+T[20]), 5);
		D = A + shiftLeft((D+G(A, B, C)+buf[i+10]+T[21]), 9);
		C = D + shiftLeft((C+G(D, A, B)+buf[i+15]+T[22]), 14);
		B = C + shiftLeft((B+G(C, D, A)+buf[i+4]+T[23]), 20);

		A = B + shiftLeft((A+G(B, C, D)+buf[i+9]+T[24]), 5);
		D = A + shiftLeft((D+G(A, B, C)+buf[i+14]+T[25]), 9);
		C = D + shiftLeft((C+G(D, A, B)+buf[i+3]+T[26]), 14);
		B = C + shiftLeft((B+G(C, D, A)+buf[i+8]+T[27]), 20);

		A = B + shiftLeft((A+G(B, C, D)+buf[i+13]+T[28]), 5);
		D = A + shiftLeft((D+G(A, B, C)+buf[i+2]+T[29]), 9);
		C = D + shiftLeft((C+G(D, A, B)+buf[i+7]+T[30]), 14);
		B = C + shiftLeft((B+G(C, D, A)+buf[i+12]+T[31]), 20);

        //ROUND III

        A = B + shiftLeft((A + H(B, C, D) + buf[i + 5] + T[32]), 4);
        D = A + shiftLeft((D + H(A, B, C) + buf[i + 8] + T[33]), 11);
        C = D + shiftLeft((C + H(D, A, B) + buf[i + 11] + T[34]), 16);
        B = C + shiftLeft((B + H(C, D, A) + buf[i + 14] + T[35]), 23);

        A = B + shiftLeft((A + H(B, C, D) + buf[i + 1] + T[36]), 4);
        D = A + shiftLeft((D + H(A, B, C) + buf[i + 4] + T[37]), 11);
        C = D + shiftLeft((C + H(D, A, B) + buf[i + 7] + T[38]), 16);
        B = C + shiftLeft((B + H(C, D, A) + buf[i + 10] + T[39]), 23);

        A = B + shiftLeft((A + H(B, C, D) + buf[i + 13] + T[40]), 4);
        D = A + shiftLeft((D + H(A, B, C) + buf[i + 0] + T[41]), 11);
        C = D + shiftLeft((C + H(D, A, B) + buf[i + 3] + T[42]), 16);
        B = C + shiftLeft((B + H(C, D, A) + buf[i + 6] + T[43]), 23);
        
        A = B + shiftLeft((A + H(B, C, D) + buf[i + 9] + T[44]), 4);
        D = A + shiftLeft((D + H(A, B, C) + buf[i + 12] + T[45]), 11);
        C = D + shiftLeft((C + H(D, A, B) + buf[i + 15] + T[46]), 16);
        B = C + shiftLeft((B + H(C, D, A) + buf[i + 2] + T[47]), 23);

        //ROUND IV

        A = B + shiftLeft((A + I(B, C, D) + buf[i + 0] + T[48]), 6);
        D = A + shiftLeft((D + I(A, B, C) + buf[i + 7] + T[49]), 10);
        C = D + shiftLeft((C + I(D, A, B) + buf[i + 14] + T[50]), 15);
        B = C + shiftLeft((B + I(C, D, A) + buf[i + 5] + T[51]), 21);

        A = B + shiftLeft((A + I(B, C, D) + buf[i + 12] + T[52]), 6);
        D = A + shiftLeft((D + I(A, B, C) + buf[i + 3] + T[53]), 10);
        C = D + shiftLeft((C + I(D, A, B) + buf[i + 10] + T[54]), 15);
        B = C + shiftLeft((B + I(C, D, A) + buf[i + 1] + T[55]), 21);

        A = B + shiftLeft((A + I(B, C, D) + buf[i + 8] + T[56]), 6);
        D = A + shiftLeft((D + I(A, B, C) + buf[i + 15] + T[57]), 10);
        C = D + shiftLeft((C + I(D, A, B) + buf[i + 6] + T[58]), 15);
        B = C + shiftLeft((B + I(C, D, A) + buf[i + 13] + T[59]), 21);
        
        A = B + shiftLeft((A + I(B, C, D) + buf[i + 4] + T[60]), 6);
        D = A + shiftLeft((D + I(A, B, C) + buf[i + 11] + T[61]), 10);
        C = D + shiftLeft((C + I(D, A, B) + buf[i + 2] + T[62]), 15);
        B = C + shiftLeft((B + I(C, D, A) + buf[i + 9] + T[63]), 21);

        A+=AA; B+=BB; C+=CC; D+=DD;
    }
}

string hex(int32 num) {
    stringstream stream;
 
    for (auto i = 0; i < 4; i++) {
        stream << setfill('0') << std::setw(2) <<hex <<num%256;
        num/=256;
    }
    return stream.str();
}

string md5(const string msg){
    int32* blocks;
    int64 len = msg.length();

    vector<byte> buf(len);
    for (auto i = 0; i<len;i++)
        buf[i]=msg[i];
    
    len = _byteswap_uint64(len*8); //в формат little endian

    //Добавляем единичный байт(бит)
    adding1byte(buf);      
    //Выравнивание потока нулями
    addingNulls(buf);   
    //Записываем длину сообщения в формате 64 бит до выравнивания в формате little-endian
    addingLenght(buf,len);  
    //Инициализируем массив констант Т   
    initMDbuf();       
    //Преобразуем блок 512 бит в массив из 16 по 32 бита и начинаем высчитывать раунды
    calcBlocks(blocks,conv512to32(buf,blocks));   
   
    //Возвращаем конк. строк 16-чных представлений чисел в little-endian формате
    return hex(A)+hex(B)+hex(C)+hex(D);
}



int main(int argc, char* argv[])
{
    string msg = "md5 hash function by mogekoff\nUsing: md5 <text>";

    if (argc != 2) {
        cout << msg;
        return -1;
    }
 
    cout << "md5("<<argv[1]<<")="<< md5(argv[1]);
 
    return 0;
}
