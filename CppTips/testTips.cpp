#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include "cpptips.h"


void testShuffle()
{
    const int POKER_SIZE = 54;
    int poker[POKER_SIZE], n = 1;
    std::generate(poker, poker + POKER_SIZE, [&n]() { return n++; });
    std::cout << "Original Sequence: " << std::endl;
    for (const auto &c: poker)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    Yincpp::myRandomShuffle(poker, POKER_SIZE);
    std::cout << "MyShuffle Sequence: " << std::endl;
    for (const auto &c : poker)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    n = 1;
    std::generate(poker, poker + POKER_SIZE, [&n]() { return n++; });
    std::random_shuffle(poker, poker + POKER_SIZE);
    std::cout << "STL Shuffle Sequence: " << std::endl;
    for (const auto &c : poker)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

int main()
{
    //Yincpp::testDoubleExclamation();
    testShuffle();
    std::cin.get();
}