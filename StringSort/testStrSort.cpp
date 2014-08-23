#include <iostream>
#include <string>
#include <ctime>
#include <cstdint>
#include <random>
#include <algorithm>
#include "../Base/baseutil.h"
#include "strSort.h"



void testSortString()
{
    int numE = 1234;
    std::string *strArray = new std::string[numE];
    std::default_random_engine eng(std::time(0));
    std::uniform_int_distribution<> uid(1, 8);
    for (int i=0; i<numE; ++i)
    {
        strArray[i] = Yincpp::random_Str(eng, uid);
        std::cout<<strArray[i]<<(i==numE-1?"\n":" ");
    }

    //Yincpp::sortStringLSB(strArray, numE);
    Yincpp::sortStringQuick3MSB(strArray, numE);
    std::cout<<"=========Sorted============"<<std::endl;
    bool isSorted = true;
    for (int i=0; i<numE; ++i)
    {
        if (isSorted && i != numE - 1 && strArray[i] > strArray[i+1])
        {
            isSorted = false;
        }
        std::cout<<strArray[i]<<(i==numE-1?"\n":" ");
    }
    std::cout<<"Order Check..."<<(isSorted?"Succeed":"Failed")<<std::endl;
    delete [] strArray;
}

int main()
{
    testSortString();
    std::cin.get();
}