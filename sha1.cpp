#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;
#define int32 unsigned int
#define int64 unsigned long long
#define byte unsigned char

//Константы
int32 A = 0x67452301;
int32 B = 0xEFCDAB89;
int32 C = 0x98BADCFE;
int32 D = 0x10325476;
int32 E = 0xC3D2E1F0;

//Функция цикл. сдвига влево. x - аргумент, n - сдвиг
int32 shiftLeft(int32 x, int32 n) {
    return (x << n) | (x >> (32 - n));
}

//Ф-я добавления длины сообщения
void addingLenght(vector<byte>& buf, const int64 len) {
    for (auto i = 7; i >= 0; i--)
        buf.push_back(byte((len >> (i * 8)) & 0xFF));
}

//Ф-я дополнения сообщения нулями
void addingNulls(vector<byte>& buf) {
    do
        buf.push_back(0);
    while (buf.size() % 64 != 56);
}

//Ф-я добавления единичного байта
void adding1byte(vector<byte>& buf) {
    buf.push_back(128);   //128 = 1000 0000
}

//Ф-я конвертирования 512 битного блока сооб-я в 80 слов по 32 бита
int32* conv512to80(vector<byte>& buf, int offset) {
    int32* words = new int32[80];

    for (auto i = 0; i<16; i++) {
        words[i] = 0;
        words[i] += int32(buf[4*i + offset]) << 24;
        words[i] += int32(buf[4*i + 1 + offset]) << 16;
        words[i] += int32(buf[4*i + 2 + offset]) << 8;
        words[i] += int32(buf[4*i + 3 + offset]);
    }
    for (auto i = 16; i < 80; i++) 
        words[i] = shiftLeft(words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16],1);
    
    return words;
}

//Ф-я вычисления раундов
void calcBlocks(int32* buf){  
    int32 i,k,f; 
    int32 a=A,b=B,c=C,d=D,e=E;
    for (i = 0; i < 80; i++) {
        if(i<20) {
            f = (b&c) | ((~b)&d);
            k= 0x5A827999;
        }
        else if(i<40) {
            f = b^c^d;
            k = 0x6ED9EBA1;
        }
        else if(i<60){
            f = (b&c)|(b&d)|(c&d);
            k = 0x8F1BBCDC;
        }
        else {
            f = b^c^d;
            k = 0xCA62C1D6;
        }
        int32 temp = shiftLeft(a, 5) + f + e + k + buf[i];
        e = d;
        d = c;
        c = shiftLeft(b, 30);
        b = a;
        a = temp;
    }
    A += a;
    B += b;
    C += c;
    D += d;
    E += e;
}

//Ф-я вывода хеша
string hex(int32 num) {
    stringstream stream;
    stream << setfill('0') << std::setw(8) << hex << num;
    return stream.str();
}

//Основная ф-я, вызывающая вспомогательные
string sha1(const string msg) {
    int64 len = msg.length();

    vector<byte> buf(len);
    for (auto i = 0; i < len; i++)
        buf[i] = msg[i];

    len *= 8; //Размер сообщения в битах

    //Добавляем единичный байт(бит)
    adding1byte(buf);
    //Выравнивание потока нулями
    addingNulls(buf);
    //Записываем длину сообщения размером 64 бит до выравнивания в блок 512 бит
    addingLenght(buf, len);
    //Преобразуем блок 512 бит в массив из 80 слов по 32 бита и начинаем вычислять раунды
    for(auto offset = 0; offset < buf.size(); offset+=64)
        calcBlocks(conv512to80(buf,offset));

    //Возвращаем конк. строк 16-чных представлений чисел
    return hex(A) + hex(B) + hex(C) + hex(D) + hex(E);
}

int main(int argc, char* argv[])
{
    string msg = "sha1 hash function by mogekoff\nUsing: sha1 <text>";

    if (argc != 2) {
        cout << msg;
        return -1;
    }

    cout << "sha1(" << argv[1] << ")=" << sha1(argv[1]);
}
