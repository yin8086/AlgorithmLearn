#include <cmath>
namespace Yincpp
{
    std::pair<int, std::string> longestConsecutiveSubString(const std::string &str)
    {
        std::vector<std::string> substrs;
        int maxCount = 1, count = 1;
        std::string substr;
        int strLen = str.length();

        // divide into several substring start from i
        // so all the substrings is
        // i=0...length-1, substrs[i].substr(0,1...substrs[i].size())
        for (int i=0; i<strLen; ++i)
        {
            substrs.push_back(str.substr(i, strLen-i));
        }

        for (int i=0; i<strLen; ++i)
        {
            for (int j=i+1; j<strLen; ++j)
            {
                count = 1;
                if (substrs[i].substr(0, j-i) == substrs[j].substr(0, j-i))
                {
                    count ++;

                    for (int k = j+ (j-i); k < strLen; k++)
                    {
                        if (substrs[i].substr(0, j-i) == substrs[k].substr(0, j-i))
                        {
                            count ++;
                        }
                        else
                            break;
                    }
                    if (count > maxCount)
                    {
                        maxCount = count;
                        substr = substrs[i].substr(0, j-i);
                    }
                }
            }
        }

        return make_pair(maxCount, substr);

    }


    void testLongSame(const std::string &str)
    {
        for (int subLen=str.length()-1; subLen>1; --subLen)
        {
            for (int j=0; j<str.length(); j++)
            {
                if (j + subLen <= str.length())
                {
                    size_t t=0;
                    size_t num=0;
                    std::string tep = str.substr(j, subLen);
                    t = str.rfind(tep);
                    num = str.find(tep);
                    if (t!=num)
                    {
                        std::cout<<tep<<" "<<t+1<<std::endl;
                        return;
                    }
                }
            }
        }
    }

    long long numOfDigit(long long i, int digit)
    {
        long long num = 0;
        while (i > 0)
        {
            if (i % 10 == digit)
            {
                num++;
            }
            i /= 10;
        }
        return num;
    }

    std::pair<long long, long long> numberOfDigit(long long num, int digit)
    {
        long long maxI = 1;
        int maxNum = 0;

        for (long long i = 1; i <= num; ++i)
        {
            int curN = numOfDigit(i, digit);
            if (curN > 0)
            {
                maxI = i;
                maxNum += curN;
            }
        }
        return std::make_pair(maxI, maxNum);
    }



    size_t testLongQuery(const std::string &query, const std::string &text)
    {
        for (int subLen=query.length()-1; subLen>1; --subLen)
        {
            // substring length from query.size()-1 to 1
            for (int j=0; j<query.length(); j++)
            {
                // substring start idx from 0 to length
                if (j + subLen <= query.length())
                {
                    // if the substring is valid
                    std::string tep = query.substr(j, subLen);
                    // first substring is right
                    if (text.find(tep) != std::string::npos)
                    {
                        return tep.size();
                    }
                }
            }
        }
    }

    static long long powerInt(int base, int head)
    {
        long long result = 1;
        for (int i = 0; i < head; ++i)
        {
            result *= base;
        }
        return result;
    }

    static long long  NumbertOfD(const char *strN, int digit)
    {
        if (!strN || *strN < '0' || *strN > '9' || *strN == '\0')
        {
            return 0;
        }

        int length = strlen(strN);
        long long numberMax;
        int first = strN[0] - '0';
        if (first < digit)
        {
            numberMax = 0;
        }
        else if (first == digit)
        {
            numberMax = atol(strN + 1) + 1;
        }
        else
        {
            numberMax = powerInt(10, length - 1);
        }

        // XXX(X+1) -> 1XXXX, 1XXX(X+1) -> 2XXXX 
        long long numberBig = first * (length - 1) * powerInt(10, length - 2);
        long long numberSmall = NumbertOfD(strN + 1, digit);

        return numberMax + numberBig + numberSmall;
    }

    int numberOfDigitRecur(long long n, int digit)
    {
        if (n <= 0 || digit < 0 || digit > 9)
        {
            return 0;
        }

        char strN[50];
        sprintf_s(strN, "%d", n);

        return NumbertOfD(strN, digit);
    }

    //@param string: [pStart, pEnd]
    static void reverseStr(char *pStart, char *pEnd)
    {
        if (!pStart || !pEnd)
        {
            return;
        }

        while (pStart < pEnd)
        {
            char tmpC = *pEnd;
            *pEnd = *pStart;
            *pStart = tmpC;

            pStart++; pEnd--;
        }
    }

    char* strROR(char *pData, int k)
    {
        if (!pData || k < 0)
        {
            return pData;
        }

        
        char *pStart = pData, *pEnd = pData;
        
        while (*pEnd != '\0')
        {
            pEnd++;
        }
        size_t strLen = pEnd - pStart;
        pEnd--;

        k = k % strLen;

        reverseStr(pStart, pStart + strLen - k - 1);
        reverseStr(pStart + strLen - k, pEnd);
        reverseStr(pStart, pEnd);
        return pData;
    }
    
    char* strROL(char *pData, int k)
    {
        if (!pData || k < 0)
        {
            return pData;
        }


        char *pStart = pData, *pEnd = pData;

        while (*pEnd != '\0')
        {
            pEnd++;
        }
        size_t strLen = pEnd - pStart;
        pEnd--;

        k = k % strLen;

        reverseStr(pStart, pStart + k - 1);
        reverseStr(pStart + k, pEnd);
        reverseStr(pStart, pEnd);
        return pData;
    }




}