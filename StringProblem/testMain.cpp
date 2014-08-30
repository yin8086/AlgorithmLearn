#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "strProblem.h"


void testStr()
{
    
    /*std::string str;
    std::pair<int, std::string> rs;
    std::cin>>str;
    
    rs = Yincpp::longestConsecutiveSubString(str);
    std::cout<<"Longest Consecutive Substring:"<<rs.second<<", count: "<<rs.first<<std::endl;
    */

    std::string query, text;
    std::cout<<"input query, text:"<<std::endl;
    std::cin>>query>>text;
    std::cout<<"Length: "<<Yincpp::testLongQuery(query, text)<<std::endl;
    //Yincpp::testLongSame(str);
}

void testNumOfDigit()
{
    long long endNum = 100;
    int digit = 9;
    std::pair<long long, long long> rs = Yincpp::numberOfDigit(endNum, digit);
    std::cout<<"1~"<<endNum<<" have ["<<rs.second<<"] "<<digit<<"s. Max I: "<<rs.first<<std::endl;
}
int main()
{
    testStr();
    system("pause");
}


