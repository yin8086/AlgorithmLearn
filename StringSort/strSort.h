namespace Yincpp
{
    static void sortPassLSB(std::string tmpA[], std::string strA[], int n, int d)
    {
        const int CHARNUM = 256 + 1; // 255 chars and 1 empty sign
        int count[CHARNUM+1] = {0};

        for (int i=0; i<n; ++i)
        {
            // 0 stand for no current char
            // 1...256 stand for char 0...255
            // 0...256 plus 1 stand for count calc
            if (d < strA[i].size())
            {
                count[static_cast<uint8_t>(strA[i][d])+1+1] += 1;
            }
            else
            {
                count[0+1] ++;
            }
        }
        // frequency 1...CHARNUM to start idx 0...CHARNUM-1
        for (int i=0; i<CHARNUM; ++i)
        {
            count[i+1] += count[i];
        }

        for (int i=0; i<n; ++i)
        {
            if (d < strA[i].size())
            {
                tmpA[count[static_cast<uint8_t>(strA[i][d])+1]++] = strA[i];
            }
            else
            {
                tmpA[count[0]++] = strA[i];
            }
        }
    }


    // sort string from LSB using the method
    // of count sort or bin sort, for sequence
    // array use count, for link representation
    // use bin sort
    //@param strA[0...n-1]
    void sortStringLSB(std::string strA[], int n)
    {
        
        std::string *tmpA = new std::string[n];

        // 1. get max string size for number of pass
        int maxSize = strA[0].size();
        for(int i=1; i<n; ++i)
        {
            if (strA[i].size() > maxSize)
            {
                maxSize = strA[i].size();
            }
        }

        int pi;
        for (pi = maxSize - 1; pi >= 1; pi -= 2)
        {
            sortPassLSB(tmpA, strA, n, pi);
            sortPassLSB(strA, tmpA, n, pi-1);
        }

        if (pi == 0)
        {
            sortPassLSB(tmpA, strA, n, pi);
            for (int i=0; i<n; ++i)
            {
                strA[i] = tmpA[i];
            }
        }

        delete [] tmpA;
    }

    static inline int charAt(std::string s, int d)
    {
        if(d < s.size())
            return s[d];
        else
            return -1;
    }

    void sortStrQuick3(std::string strA[], int left, int right, int d)
    {
        if (left >= right)
        {
            return;
        }

        int pL = left, pR = right;
        char pivot = charAt(strA[left], d);
        int pi = left + 1;
        while (pi <= pR)
        {
            int cmp = charAt(strA[pi], d) - pivot;
            if (cmp < 0)
            {
                std::swap(strA[pi++], strA[pL++]);
            }
            else if(cmp >0)
            {
                 std::swap(strA[pi], strA[pR--]);
            }
            else
            {
                pi++;
            }
        }
        // a[left...pL-1] < pivot = a[pL...pR] < a[pR+1...right]
        sortStrQuick3(strA, left, pL-1, d);
        if ( pivot >= 0)
        {
            sortStrQuick3(strA, pL, pR, d+1);
        }

        sortStrQuick3(strA, pR+1, right, d);
    }

    void sortStringQuick3MSB(std::string strA[], int n)
    {
        sortStrQuick3(strA, 0, n-1, 0);
    }
}