
namespace Yincpp
{

    template<class T>
    void Merge(T dst[], const T src[], int left, int mid, int right)
    {
        
        int pL = left;
        int pR = mid + 1;
        int pD = left;

        while (pL <= mid && pR <= right)
        {
            if (src[pL] < src[pR])
            {
                dst[pD++] = src[pL++];
            }
            else
            {
                dst[pD++] = src[pR++];
            }
        }

        while (pL <= mid)
        {
            dst[pD++] = src[pL++];
        }
        while (pR <= right)
        {
            dst[pD++] = src[pR++];
        }
    }

    template<class T>
    void MergePass(T dst[], const T src[], int n, int size)
    {
        int i=0;
        while ( i + 2*size <= n) // if left elements >= 2*size
        {
            // merge src[i:i+s-1] and src[i+s:i+2*s-1] to dst[i:i+2*s-1]
            Merge(dst, src, i, i+size-1, i+2*size-1);
            i += 2*size;
        }

        if (i + size < n) //  s | (<s)
        {
            Merge(dst, src, i, i+size-1, n-1);
        }
        else
        {
            for (int j = i; j <= n-1; ++j)
            {
                dst[j] = src[j];
            }
        }
    }
    
    template<class T>
    void MergeSort(T a[], int n)
    {
        // need O(n) space for merge
        T *tmpA = new T[n];
        int numToMerge = 1;
        while (numToMerge < n)
        {
            MergePass(tmpA, a, n, numToMerge);
            numToMerge += numToMerge;
            MergePass(a, tmpA, n, numToMerge);
            numToMerge += numToMerge;
        }
        
        delete [] tmpA;
    }


    void mergeSortTest()
    {
        int numE = 876;
        std::default_random_engine newEngine(static_cast<int>(std::time(0)));
        std::uniform_int_distribution<int> uidn(0, numE * 4);

        int *testArray = new int[numE];
        std::cout<<"====Unsorted====="<<std::endl;

        for (int i=0; i<numE; i++)
        {
            testArray[i] = uidn(newEngine);
            std::cout<<testArray[i]<<" ";
        }
        std::cout<<std::endl;

        MergeSort(testArray, numE);

        std::cout<<"====sorted====="<<std::endl;
        for (int i=0; i<numE; i++)
        {
            std::cout<<testArray[i]<<" ";
        }
        std::cout<<std::endl;

        delete [] testArray;

    }


}