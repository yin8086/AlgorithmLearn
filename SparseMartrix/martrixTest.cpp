#include <iostream>
#include <cstdlib>
#include "sparsemartrix.h"




int main()
{
    try
    {
        SparseMartrix<int> sm1;
        //SparseMartrix<int> sm2;
        std::cin>>sm1;
        std::cout<<sm1;
        SparseMartrix<int> sm2;
        sm1.Transpose(sm2);
        std::cout<<sm2;
        //SparseMartrix<int> sm3 = sm1 + sm2;
        std::cout<<sm1 + sm2;
    }
    catch (std::exception& e)
    {
    	std::cout<<e.what()<<std::endl;
    }
    std::system("pause");
    return 0;
}