//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_CELLDATA_H
#define MY_APPLICATION_CELLDATA_H

#include <unordered_map>
#include <vector>
struct CellData
{
    char flags;
    char mask;
    uint8_t idx;


    CellData(float flags, float mask, uint8_t idx) :flags(flags), mask(mask), idx(idx) {};

};


class CellChecker {

public:

    CellChecker();

    bool findIndex(char& flags,uint8_t& idx);


private:


    std::vector<CellData> Data;


};

#endif //MY_APPLICATION_CELLDATA_H
