#include <iostream>
#include <string>
#include <cstdint>
#include "pattermatch.h"

void testPatternMatch(const std::string srcStr, const std::string pat)
{
    std::cout<<"Text: "<<srcStr<<std::endl;
    std::cout<<"Find: ";
    int pos;
    if ((pos = Yincpp::KMPMatch(srcStr, pat)) == -1 )
    //if ((pos = Yincpp::BMMatch(srcStr, pat)) == -1 )
    //if ((pos = Yincpp::RKMatch(srcStr, pat)) == -1 )
    {
        std::cout<<"Not Found"<<std::endl;
    }
    else
    {
        for (int i=0; i<pos; ++i)
        {
            std::cout<<" ";
        }
        std::cout<<pat<<std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout<<"Need 2 parameters!"<<std::endl;
    }
    else
    {
        testPatternMatch(std::string(argv[1]), std::string(argv[2]));
    }
    std::cin.get();
}