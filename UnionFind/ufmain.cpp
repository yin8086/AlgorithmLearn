#include <iostream>
#include "unionfind.h"
#include "yinexception.h"


int main()
{
    try
    {
        UnionFind::unitTest();
    }
    catch(std::exception *ecp)
    {
        std::cout<<ecp->what()<<std::endl;
    }
    std::cin.get();
}