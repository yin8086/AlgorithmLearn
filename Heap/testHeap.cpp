#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include <queue>
#include <stack>
#include "yinexception.h"
#include "huffman.h"

void heapUnitTest()
{
    int numE = 100;
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
}

int main()
{
    HuffUnitTest();
    std::cin.get();

}