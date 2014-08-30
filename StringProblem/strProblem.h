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
}