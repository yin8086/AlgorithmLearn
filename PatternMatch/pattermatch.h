namespace Yincpp
{
    // i never back
    int KMPMatch(const std::string srcStr, const std::string pat)
    {
        if (pat.size() == 0 || pat.size() > srcStr.size())
        {
            return -1;
        }
        // 1. construct next[j]
        // next[j]=k is the pos where pat[j-k...j-1] = src[i-k...i-1]
        //                        = pat[0...k-1]
        // so move pattern j to next[j], and i unchanged.
        // so the new next[j]=k must be the first probable not match position
        int *next = new int[pat.size()];
        if (pat.size() >= 1)
        {
            next[0] = -1; // i must forward one
        }
        if (pat.size() >= 2)
        {
            next[1] = 0;
        }
        if (pat.size() >= 3)
        {
            for (int i=2; i<pat.size(); ++i)
            {
                if (pat[i-1] == pat[next[i-1]])
                {
                    next[i] = next[i-1] + 1;
                }
                else
                {
                    int j = i-1;
                    while (j != 0 &&pat[i-1] != pat[next[j]])
                    {
                        j = next[j];
                    }
                    next[i] = next[j] + 1;
                }
            }
        }

        // 2. search
        int i = 0, j = 0;
        while (i < srcStr.size() && j < pat.size())
        {
            if (srcStr[i] == pat[j])
            {
                i++;j++;
            }
            else
            {
                j = next[j];
                if (j == -1)
                {
                    i++;
                    j = 0;
                }
            }
        }


        delete [] next;
        if (j == pat.size())
        {
            return i-pat.size();
        }
        else
        {
            return -1;
        }
        
    }



    // right to left, i back
    int BMMatch(const std::string srcStr, const std::string pat)
    {
        if (pat.size() == 0 || pat.size() > srcStr.size())
        {
            return -1;
        }

        // 1. construct right[]
        // right[c] stand for the right-most pos where
        // c exist in pat
        int right[256] = {-1};
        for (int i = 0; i < pat.size(); i++)
        {
            right[static_cast<int>(pat[i])] = i;
        }

        int i,j;
        i=0;
        while (i <= srcStr.size() - pat.size())
        {
            j = pat.size()-1;
            while (j>=0 && pat[j] == srcStr[i+j])
            {
                j--;
            }
            if (j == -1)
            {
                return i;
            }
            else
            {
                int skip = j - right[static_cast<int>(srcStr[i+j])];
                if (skip < 1)
                {
                    skip = 1;
                }
                i += skip;
            }
        }

        return -1;
    }

    static int64_t yinHash(const std::string pat, int M, int R, int64_t Q)
    {
        int64_t hashP = 0;
        // 1. calc hash of pat
        for (int i=0; i<M; ++i)
        {
            hashP = (hashP*R + static_cast<int64_t>(pat[i]) ) % Q;
        }
        return hashP;
    }
    // based on Hash, i not back
    int RKMatch(const std::string srcStr, const std::string pat)
    {
        if (pat.size() == 0 || pat.size() > srcStr.size())
        {
            return -1;
        }

        const int R = 256;
        int M = pat.size();
        int N = srcStr.size();

        //1. construct first sub string's hash
        int64_t Q = 15487469;// int64_t Random Prime

        // 1. calculate pattern Hash
        int64_t hashP = yinHash(pat, M, R, Q);
        
        // 2. calculate t* R^(m-1)
        // tR^(m-1)  => t*(R^(m-1) % Q)
        int64_t RM = 1;
        for (int i=1; i<M; ++i)
        {
            RM = (RM * R) % Q;
        }

        int64_t hashS = yinHash(srcStr, M, R, Q);
        if (hashS == hashP)
        {
            return 0;
        }

        for (int i=M; i<N; ++i)
        {
            //         mod                 mod
            hashS = ( hashS + Q - ( (RM*static_cast<int64_t>(srcStr[i-M])) % Q ) )% Q;
            hashS =  ( hashS * R +  static_cast<int64_t>(srcStr[i])) % Q;

            if (hashS == hashP)
            {
                return i-M+1;
            }
        }
        return -1;

    }

}