#include <iostream>
#include <ctime>
#include <random>
#include <functional>
#include "chain.h"

int main()
{
    Chain<int> ci;
    int numTest = 32;

    std::default_random_engine engine(static_cast<int>(time(0)));
    std::uniform_int_distribution<int> uid(0, numTest*2);

    for(int i=0; i < numTest; ++i)
    {
        ci.insert(ci.length(), uid(engine));
    }

    std::cout<<"====Unsort===="<<std::endl;
    ci.output(std::cout);
    //ci.insertSort();
    //ci.radixSort(10);
    ci.quickSort();
    std::cout<<std::endl;
    std::cout<<"====Sorted===="<<std::endl;
    ci.output(std::cout);
    std::cout<<std::endl;
    std::cin.get();
    return 0;
}