#include "sha1.h"

string HMAC(string msg, string K,string (*H)(string text), const int b){
    string buf;
    auto L = H("0").length()/2; 
    
    if(K.length() > b)              //Если ключ больше размера блока, укорачиваем по хешу
        K = H(K);
    while(K.length() < b)           //Если меньше, дополняем нулями
        K.push_back('\0');

    auto k = K;                     //Копия ключа для будущих вычислений
    for(auto i = 0; i < b; i++)
        k[i] ^= 0x36;
    
    buf=H(k+msg);

    for (auto i = 0; i < b; i++)
        K[i] ^= 0x5c;
    
    //Конв. строку hex символов в строку байт
    for (auto i = 0; i < buf.length(); i++) {
        buf[i] = std::stoi(buf.substr(i, 2), 0, 16);
        buf.erase(i + 1,1);
    }

    return H(K + buf);
}

int main() {
    cout << HMAC("Hello World","pqrstuvwxyz{|}~\x7f\x80\x81\x82\x83",sha1,64);
}
//Пример из Википедии: https://ru.wikipedia.org/wiki/HMAC#Примеры_работы
//Текст: "Hello World"
//Ключ: "pqrstuvwxyz{|}~\x7f\x80\x81\x82\x83"
//Хеш ф-я: sha1
//Размер блока: 64
