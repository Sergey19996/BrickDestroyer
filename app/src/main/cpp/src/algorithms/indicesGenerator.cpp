#include "indicesGenerator.h"
#include <cstdlib>  // для rand() и srand*()
#include <ctime>  //для time()
#include <iostream>


#define WATERIDX 7
indicesGenerator::indicesGenerator()
{
    std::srand(std::time(nullptr));
}

uint8_t indicesGenerator::GenerateGroundIndex(uint8_t escapingNumber)
{
    int FirstLine = 32;
    int SecondLine = 40;
    int ThirdLine = 48;
    int FourLine = 56;
    uint8_t FirstRow = FirstLine + rand() % 2;   // generate 4 random numbers equal idx in sprite
    uint8_t SecondRow = SecondLine + rand() % 2;
    uint8_t ThirdRow = ThirdLine + rand() % 2;
    uint8_t FourRow = FourLine + rand() % 2;



    if (escapingNumber != 0)
    {
        int availableNumbers[4] = { FirstLine, SecondLine, ThirdLine, FourLine };


        do {
            FirstRow = availableNumbers[rand() % 4] + rand() % 2;
        } while (FirstRow == escapingNumber); // повторяется пока == escaping

        return FirstRow;

    }
    else
    {

        int randRow = rand() % 4;
        switch (randRow)     // take module from idx
        {   //depends from the module we write number
            case 0:
                return  FirstRow;
                break;
            case 1:
                return  SecondRow;
                break;
            case 2:
                return  ThirdRow;
                break;
            case 3:
                return  FourRow;
                break;
        }

    }

    return 0;
}

std::vector<uint8_t>& indicesGenerator::AlgorithmBasicGround(unsigned int width, unsigned int line)
{


    for (unsigned int idx = 0; idx < width * line; idx++)
    {


        indices[idx] = GenerateGroundIndex();

    }

    for (unsigned int x = 0; x < width - 1; x++)    // checking on repitable tile
    {
        for (unsigned int y = 0; y < line - 1; y++)
        {
            if (indices[width * y + x] == indices[width * (y + 1) + x])
            {
                indices[width * (y + 1) + x] = GenerateGroundIndex(indices[width * (y + 1) + x]);
            }
            if (indices[width * y + x] == indices[width * (y) + x + 1])
            {
                indices[width * (y)+x + 1] = GenerateGroundIndex(indices[width * (y)+x + 1]);
            }
            if (indices[width * y + x] == indices[width * (y + 1)+x + 1])
            {
                indices[width * (y + 1) + x + 1] = GenerateGroundIndex(indices[width * (y + 1) + x + 1]);
            }
        }
    }

    return indices;
}

std::vector<uint8_t>& indicesGenerator::AlgorithmLakeGround(unsigned int width, unsigned int height,unsigned int LakesCount)
{
    indices.clear();
    indices.resize(width * height);  //хранит значения Uint8_t где 255 макс число
    lakesRoots.clear();

//std::vector<unsigned int> lakesRoots(7);
    lakesRoots.resize(LakesCount);
    AlgorithmBasicGround(width, height);   // заполняем уровень землей



    for (int i = 0; i < lakesRoots.size(); i++)
    {
        //int lakeRoot = rand() % (width * height);  //создаём случайный индекс (позиция рута) в 1д
        //int lakeRoot = (5 - i ) * width + 2 + i;


        do {

            lakesRoots[i] = rand() % (width * height);;       // Записываю его в вектор


        } while (indices[lakesRoots[i]] == WATERIDX);  //  по случайному индексу сажу root

        indices[lakesRoots[i]] = WATERIDX;   //  по случайному индексу сажу root


    };



    return indices;
}

void indicesGenerator::reset()
{
    indices.clear();

}
