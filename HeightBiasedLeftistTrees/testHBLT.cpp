#include <iostream>
#include <random>
#include <ctime>
#include "yinexception.h"
#include "HBLTree.h"


int main()
{
    int numE = 20;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, numE * 4);

    int *testArray = new int[numE+1];
    std::cout<<"====Unsorted====="<<std::endl;

    for (int i=1; i<=numE; i++)
    {
        testArray[i] = uidn(newEngine);
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    HeapSort(testArray, numE);

    std::cout<<"====sorted====="<<std::endl;
    for (int i=1; i<=numE; i++)
    {
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    delete [] testArray;
    std::cin.get();

}