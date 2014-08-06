namespace Yincpp
{
    const unsigned int QUICK_N_BREAK = 15;

    // @param a[] 0...n-1
    // @param rank 1...n
    template<class T>
    T Select(T a[], int n, int rank)
    {
        if (rank < 1 || rank > n)
        {
            throw OutOfBounds();
        }

        // 1. set a[n-1] the max element
        int maxI = 0;
        for (int i=maxI+1; i < n; ++i)
        {
            if (a[i] > a[maxI])
            {
                maxI = i;
            }
        }

        if (rank-1 == n-1) // if want the last
        {
            return a[maxI];
        }

        else
        {
            T tmp = a[n-1];
            a[n-1] = a[maxI];
            a[maxI] = tmp;

            // 2. quick sort
            return select(a, 0, n-2, rank);
        }
        
    }


    // @param a[] left...right
    template<class T>
    T select(T a[], int left, int right, int rank)
    {
        
        if (left >= right)
        {
            return a[left];
        }
        
        T pivot = a[left];
        int pL = left, pR = right + 1;
        T tmp;
        while(true)
        {

            // if we want to remove the boundary check of
            // the right side, we need to make the original
            // array a[n-1] be the max element and keep unchanged
            // which will lead to every quicksort call can stop
            do 
            {
                pL++;
            } while (/* pL < right  && */ a[pL] < pivot);

            // make >= to > can remove the boundary check
            // of left boundary
            do 
            {
                pR--;
            } while (a[pR] > pivot);

            if (pL >= pR)
            {
                break;
            }

            tmp = a[pR];
            a[pR] = a[pL];
            a[pL] = tmp;

        }

        // pR-left+1 is the rank of the pR element in left...right
        if (rank == pR - left + 1)
        {
            return pivot;
        }

        // when loop out, pR must point to an element < pivot
        // so exchange it with pivot which is at its left
        a[left] = a[pR];
        a[pR] = pivot;
        
        
        if (rank < pR - left + 1)
        {
            return select(a, left, pR-1, rank);
        }
        else
        {
            // For rank element, rank = Idx - left + 1
            // Idx = rank + left - 1
            // so rank in the right side should be
            // Idx - (pR+1) + 1= Idx - pR = rank + left - 1 - pR
            return select(a, pR+1, right, rank + left - 1 - pR );
        }

    }

    void quickSelectionTest()
    {
        int numE = 10;
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

        //int tarIdx = uidn(newEngine) % numE + 1;
        int tarIdx = 7;
        std::cout<<"===================="<<std::endl;
        std::cout<<"Select "<<tarIdx<<"th element: "<<Select(testArray, numE, tarIdx)
            <<std::endl;

        delete [] testArray;
    }

}