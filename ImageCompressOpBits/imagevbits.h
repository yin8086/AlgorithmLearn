namespace Yincpp
{
    void Vbits(int l[], int b[], int n, int s[], int kay[])
    {
        int L = 256, header = 11;
        s[0] = 0;
        for(int i=1; i<=n; ++i)
        {
            int lsum = l[i], bmax = b[i]; // only i
            s[i] = s[i-1] + lsum*bmax;
            kay[i] = i;

            // k is merge numbers
            for (int mNum = 2; mNum <= i && lsum+l[i-mNum+1]<=L; mNum++)
            {
                lsum += l[i-mNum+1];
                if (b[i-mNum+1] > bmax)
                {
                    bmax = b[i-mNum+1];
                }
                if (lsum * bmax + s[i-mNum] < s[i])
                {
                    s[i] = lsum * bmax + s[i-mNum];
                    kay[i] = mNum;
                }

            }
            s[i] += header;

        }
    }

    void traceBackICOP(int kay[], int n)
    {
        if (n == 0)
        {
            return;
        }
        else
        {
            traceBackICOP(kay, n-kay[n]);
            std::cout<<n-kay[n]+1<<std::endl;
        }
    }

    void ImageCompressOpBitsTest()
    {
        int l[] = {0, 6, 3, 10, 2, 3};
        int b[] = {0, 1, 2, 3, 2, 1};
        int s[6], kay[6];
        Vbits(l, b, 5, s, kay);

        traceBackICOP(kay, 5);

    }

}