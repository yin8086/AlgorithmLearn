#include <iostream>
#include <random>
#include <ctime>
#include "quicksort.h"

void quickSortTest()
{
    int numE = 123;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, numE * 4);

    int *testArray = new int[numE];
    std::cout<<"====Unsorted====="<<std::endl;

    for (int i=0; i<numE; i++)
    {
        testArray[i] = uidn(newEngine);
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    //Yincpp::QuickSort(testArray, numE);
    //Yincpp::QuickSortStack(testArray, numE);
    Yincpp::Quick3Sort(testArray, numE);
    //Yincpp::QuickSortOneWay(testArray, numE);

    std::cout<<"====sorted====="<<std::endl;
    for (int i=0; i<numE; i++)
    {
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    delete [] testArray;
}

int main()
{
    quickSortTest();
    std::cin.get();
}