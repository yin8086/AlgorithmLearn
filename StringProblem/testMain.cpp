#define _CRT_SECURE_NO_WARNINGS
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
    long long endNum = 657890;
    int digit = 1;
    
    std::pair<long long, long long> rs = Yincpp::numberOfDigit(endNum, digit);
    std::cout<<"1~"<<endNum<<" have ["<<rs.second<<"] "<<digit<<"s. Max I: "<<rs.first<<std::endl;
    
    long long nod = Yincpp::numberOfDigitRecur(endNum, digit);
    std::cout << "1~" << endNum << " have [" << nod << "] " << digit << "s." << std::endl;
}

void testRotate()
{
    
    char testStr[100];
    while (true)
    {
        scanf("%s", testStr);
        if (std::strcmp(testStr, "-1") == 0)
        {
            break;
        }

        int k = 2;
        Yincpp::strROR(testStr, k);
        std::cout << "ROR " << k << " Bits: " << testStr << std::endl;
        
        Yincpp::strROL(testStr, k);
        std::cout << "ROR " << k << " Bits: " << testStr << std::endl;
    }
    
}
int main()
{
    //testStr();
    //testNumOfDigit();
    testRotate();
    system("pause");
}


