#include "sha1.h"
#include "hmac.h"
#include "base32.h"
#include <ctime>

//Выбирает код из 6-ти цифр на основе результата ф-ии hmac
int selectCode(string hmac_result) {
    int offset = hmac_result[19] & 0xf;
    int bin_code = (hmac_result[offset] & 0x7f) << 24
        | (hmac_result[offset + 1] & 0xff) << 16
        | (hmac_result[offset + 2] & 0xff) << 8
        | (hmac_result[offset + 3] & 0xff);
    return bin_code;
}

int main() {
    int64 startTime = 0;               //Время, с которого ведется отсчёт
    int64 currentTime = time(0);                    //Дискретное значение времени
    short interval = 30;               //Интервал в секундах (по умолчанию - 30 сек.)
    string secret_key = "2CYNDBJA2GBNDCZA2C65BNOQWPIL5UFU2GH5BOJAHIUQ====";        //Разделяемый секрет
    short code_size = 6;                //Длина токена (по умолчанию - 6)

    cout << "Input your secret key (in base32): ";
    cin >> secret_key;
    cout << "Input time interval in seconds (def. 30): ";
    cin >> interval;
    cout << "Input code size (num of digits, def. 6): ";
    cin >> code_size;

    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "+ TOTP generator by mogekoff\n+ Interval:\t\t"<< interval <<" seconds\n+ Current time:\t\t"<<time(0)<<" seconds\n+ Secret key:\t\t"<<secret_key<<"\n+ Code length:\t\t"<<code_size<<endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    int64 newTime;
    short newLeftSeconds = interval;
    short leftSeconds = interval;
    
    //Бесконечный цикл вычисления кодов
    while(true) {
        //Вычислим текущее время от эры unix
        newTime = (time(0)-startTime)/ interval;
        //Вычтем из 30 сек. текущее время, получим остаток времени
        newLeftSeconds = interval - time(0) % interval;

        //Если время закончилось -> прошло 30 секунд
        if (newTime != currentTime) {
            //Обновляем тек. время
            currentTime = newTime;

            //Создаем строку байт времени (массив байт, но строкой)
            string sT = "00000000";

            //Заполняем ее тек. временем
            for (int i = 0; i < 8; i++)
                sT[7 - i] = (currentTime >> (i * 8));

            //Получаем результат ф-ии HMAC и парсим его
            string hmac_result = HMAC(sT, base32decode(secret_key), sha1, 64);
            for (auto i = 0; i < hmac_result.length(); i++) {
                hmac_result[i] = std::stoi(hmac_result.substr(i, 2), 0, 16);
                hmac_result.erase(i + 1, 1);
            }
            
            //Получаем код отдельной ф-ей и выводим
            auto bin_code = selectCode(hmac_result);
            cout << "\n\nCode: " << setfill('0') << setw(6)<< bin_code%1000000 << endl;
        }
        //Если прошла секунда -> новая секунда не равна прошлой -> обновить таймер
        if (newLeftSeconds != leftSeconds) {
            leftSeconds=newLeftSeconds;
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\bTime left: "<<setfill('0')<<setw(2)<<leftSeconds;
        }
    }
}
