#include <queue>
#include <stack>
#include <iostream>
#include <functional>
#include <random>
#include <list>
#include <ctime>
#include "../Base/baseutil.h"
#include "../Heap/yinexception.h"
#include "../ChainInsert/chain.h"
#include "../UnionFind/unionfind.h"
#include "../Heap/heap.h"
#include "graph.h"

int main()
{
    //Yincpp::graphUnitTest();
    //Yincpp::TopologicalSortTest();
    //Yincpp::BipartieCoverTest();
    //Yincpp::shortedPathTest();
    //Yincpp::minSpanningTreeTest();
    //Yincpp::allPairsShortestPairTest();
    Yincpp::TravellingSalesPersonTest();
    Yincpp::TravellingSalesPersonBBTest();
    std::cin.get();
}