#include<iostream>
#include <random>
#include <numeric>
#include <ctime>
#include "yinexception.h"
#include "skiplist.h"


int main()
{
    SkipList<element, long> skl(999);
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    
    int numE = 100;
    std::uniform_int_distribution<int> uidn(0,numE*3);
    for(int i=0; i<numE; ++i)
    {
        element tmpE;
        tmpE.m_data = uidn(newEngine);
        tmpE.m_key = i*2;

        skl.Insert(tmpE);
    }
    element tmpE;
    tmpE.m_data = 312;
    tmpE.m_key = 808;
    skl.Insert(tmpE);
    /*
    std::uniform_int_distribution<int> uidn2(0,numE);
    for(int i=0; i<5; ++i)
    {
        long k = uidn2(newEngine)*2;
        element tmpE;
        if(skl.Search(k, tmpE))
        {
            std::cout<<"Key "<<k
                <<", value = " << tmpE << std::endl;
        }
        
    }
    */

    element tmpE2;
    if(skl.Search(808, tmpE2))
    {
        std::cout<<"Key "<<808
            <<", value = " << tmpE2 << std::endl;
    }
    std::cin.get();
}