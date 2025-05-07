//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_INDICESGENERATOR_H
#define MY_APPLICATION_INDICESGENERATOR_H
#include <vector>
#include <glm/glm.hpp>
#include "CellData.h"

// Represents the four possible (collision) directions


class indicesGenerator {
public:

    indicesGenerator();

    uint8_t GenerateGroundIndex(uint8_t escapingNumber = 0);
    std::vector <uint8_t>& AlgorithmBasicGround(unsigned int row, unsigned int line);
    std::vector <uint8_t>& AlgorithmLakeGround(unsigned int row, unsigned int line,unsigned int LakesCount);
    std::vector<unsigned int>& getLakesRoots() { return lakesRoots; };
    void reset();

private:
    std::vector <uint8_t> indices;
    std::vector<unsigned int> lakesRoots;
    CellChecker checker;

};


#endif //MY_APPLICATION_INDICESGENERATOR_H
