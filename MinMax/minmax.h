namespace Yincpp
{


    // @param src, n, minT, maxT
    // src 0...n-1
    // X is array element, 1-n is comparison order
    //                   7
    //               /       \
    //              5        6
    //           /     \    / \
    //          3       4  X   X
    //       /    \    / \
    //      1      2  X   X
    //     / \    / \
    //    X   X  X   X
    template<class T>
    bool MinMax(const T src[], int n, T &minT, T&maxT)
    {
        if (n < 1)
        {
            return false;
        }
        if (n == 1)
        {
            minT = maxT = src[0];
            return true;
        }

        int minIdx, maxIdx, min2, max2;
        int s = 0; // start from position 0
        if (n % 2 == 1)
        {
            minIdx = maxIdx = 0;
            s = 1; // from 1
        }
        else
        {
            minIdx = src[0] < src[1] ? 0 : 1;
            maxIdx = 1 - minIdx;
            s = 2;
        }

        for (; s < n; s += 2)
        {
            if (src[s] < src[s+1])
            {
                min2 = s;
                max2 = s+1;
            }
            else
            {
                min2 = s+1;
                max2 = s;
            }

            if (src[min2] < src[minIdx])
            {
                minIdx = min2;
            }
            if (src[max2] > src[maxIdx])
            {
                maxIdx = max2;
            }
        }

        minT = src[minIdx];
        maxT = src[maxIdx];
        return true;
    }


    void minMaxTest()
    {
        int numE = 1001;
        std::default_random_engine newEngine(static_cast<int>(std::time(0)));
        std::uniform_int_distribution<int> uidn(0, numE * 4);

        int *testArray = new int[numE];
        std::cout<<"====Random Sequence====="<<std::endl;

        for (int i=0; i<numE; i++)
        {
            testArray[i] = uidn(newEngine);
            std::cout<<testArray[i]<<(i == numE-1 ? "\n" : " ");
        }

        int minV, maxV;
        MinMax(testArray, numE, minV, maxV);
        std::cout<<"Min: "<<minV<<", MaxV: "<<maxV<<std::endl;
        delete [] testArray;
    }
}