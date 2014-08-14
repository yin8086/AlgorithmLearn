#include <iostream>
#include <ctime>
#include <random>
#include <list>
#include "../Heap/yinexception.h"
#include "../Heap/heap.h"
#include "../MergeSort/mergesort.h"
#include "knapsackDP.h"

int main()
{
    //Yincpp::Knapsack0_1Test();
    Yincpp::KnapsackBTTest();
    Yincpp::KnapsackBBTest();
    std::cin.get();
}