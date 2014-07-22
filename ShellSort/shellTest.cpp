#include<iostream>
#include <ctime>
#include <random>
//#define DEBUG_MY
#include "shellsort.h"

int main()
{
    int num = 100;
    SortTest<double> st(num);
    st.randomFill();

    std::cout<<"==Unsort Ver=="<<std::endl;
    st.printOut();
    st.shellSort();
    std::cout<<"==Sorted Ver=="<<std::endl;
    st.printOut();
    std::cin.get();
    
    return 0;
}
