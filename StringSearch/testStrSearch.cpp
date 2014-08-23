#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>
#include "../Base/yinexception.h"
#include "../Base/baseutil.h"
#include "strSearchTree.h"

void searchTreeTest()
{
    Yincpp::TrieST<int> tst;
    int numE = 5;
    std::default_random_engine eng(std::time(0));
    std::uniform_int_distribution<> uid(1, 8);
    std::vector<std::string> sv;
    for (int i=0; i<numE; ++i)
    {
        std::string tmpStr = Yincpp::random_Str(eng, uid);
        tst.put(tmpStr, tmpStr.size());
        sv.push_back(tmpStr);
    }

    std::cout<<"Search "<<sv[numE/4]<<", value = "<<tst.get(sv[numE/4])
        <<"("<<sv[numE/4].size()<<")"<<std::endl;
    std::cout<<"Search "<<sv[numE/2]<<", value = "<<tst.get(sv[numE/2])
        <<"("<<sv[numE/2].size()<<")"<<std::endl;
    std::cout<<"Search "<<sv[3*numE/4]<<", value = "<<tst.get(sv[3*numE/4])
        <<"("<<sv[3*numE/4].size()<<")"<<std::endl;

    tst.traverseWithPrefix("");
    tst.remove(sv[numE/4]);
    tst.traverseWithPrefix("");
}

int main()
{
    searchTreeTest();
    std::cin.get();
}