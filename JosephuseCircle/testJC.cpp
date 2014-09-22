#include <iostream>
#include <list>
#include "JCircle.h"

int main()
{
    std::cout << "input n m:";
    int m, n;
    std::cin >> n >> m;
    int remainIntA = Yincpp::lastRemaining(n, m);
    int remainIntB = Yincpp::lastRemainingDynamic(n, m);
    if (remainIntA == -1 || remainIntB == -1)
    {
        std::cout << "input error!" << std::endl;
    }
    else
    {
        std::cout << "Last Remain: " << remainIntA <<" == "<<remainIntB<< std::endl;
    }
    system("pause");
}