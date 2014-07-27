#include <cstdint>
#include <random>
#include <ctime>
#include <functional>
#include <cassert>
#include <iostream>
#include <stack>
#include "avltree.h"


int main()
{
    AVLTree<element, long>::unitTest();
    std::cerr<<"Press any key to continue"<<std::endl;
    std::cin.get();
}