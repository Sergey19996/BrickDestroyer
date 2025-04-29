//
// Created by Lenovo on 19.04.2025.
//

#include "CellData.h"

CellChecker::CellChecker()
{
    Data = {

            //
            CellData{0b01000010,
                     0b01011010,29},
            CellData{0b01001000,
                     0b01011110,38},
            CellData{0b01010000,
                     0b01011011,46},
            CellData{0b00001010,
                     0b11011010,62},
            CellData{0b01010010,
                     0b01011010,53},
            CellData{0b00010010,
                     0b01111010,54},
            CellData{0b00011000,
                     0b01011010,12},
            CellData{0b00010000,
                     0b01111011,11},
            CellData{0b01000000,
                     0b01011111,25},
            CellData{0b00001000,
                     0b11011110,8},
            CellData{0b00000010,
                     0b11111010,1},
            CellData{0b10100010,
                     0b11111010,44},
            CellData{0b10001100,
                     0b11011110,52},
            CellData{0b00110001,
                     0b01111011,60},
            CellData{0b00100010,
                     0b11111010,9},
            CellData{0b01000100,
                     0b01011111,10},
            CellData{0b00110000,
                     0b01111011,18},
            CellData{0b01000001,
                     0b01011111,20},
            CellData{0b10000010,
                     0b11111010,22},
            CellData{0b00010001,
                     0b01111011,14},
            CellData{0b10001000,
                     0b11011110,16},
            CellData{0b00110010,
                     0b01111010,28},
            CellData{0b01010001,
                     0b01011011,4},
            CellData{0b01001100,
                     0b01011110,6},
            CellData{0b10001010,
                     0b11011010,30},
            CellData{0b00011010,
                     0b01011010,37},
            CellData{0b01011000,
                     0b01011010,45},
            CellData{0b01101011,
                     0b01111011,61},
            CellData{0b00001100,
                     0b11011110,17},
            CellData{0b01001010,
                     0b01011010,31},
            CellData{0b01000101,
                     0b01011111,36},
            CellData{0b01011010,
                     0b01011010,21},
            CellData{0b10000001,
                     0b11111111,15},
            CellData{0b00100100,
                     0b11111111,23},
            CellData{0b10100000,
                     0b11111111,43},
            CellData{0b00000101,
                     0b11111111,35},
            CellData{0b10000100,
                     0b11111111,51},
            CellData{0b00100001,
                     0b11111111,59},
            CellData{0b00000001,
                     0b11111111,0},
            CellData{0b10000000,
                     0b11111111,27},
            CellData{0b00100000,
                     0b11111111,24},
            CellData{0b00000100,
                     0b11111111,3},
            CellData{0b00100101,
                     0b11111111,34},
            CellData{0b10000101,
                     0b11111111,42},
            CellData{0b10100100,
                     0b11111111,50},
            CellData{0b10100001,
                     0b11111111,58},
            CellData{0b10100101,
                     0b11111111,13},
    };

}

bool CellChecker::findIndex(char& flags,uint8_t& idx)
{

    bool isExist = false;
    // Проходим по всем элементам вектора Data
    for (size_t i = 0; i < Data.size(); ++i) {
        // Применяем маску к флагам и входным данным
        if ((flags & Data[i].mask) == (Data[i].flags & Data[i].mask)) { // мы фильтруем по маске оставляе только нужные нам биты, всё остальное станет 0
            // Возвращаем индекс, если биты совпали по маске
            idx = Data[i].idx;
            isExist = true;
            return true;
        }
    }

    // Если ничего не нашли, возвращаем 0 или другой индикатор
    return isExist;
}
