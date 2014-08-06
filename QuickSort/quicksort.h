#include <stack>
namespace Yincpp
{
    
    

    const unsigned int QUICK_N_BREAK = 15;
    // must be recursive or use stack to
    // simulate recursive
    // @param a[] 0...n-1
    template<class T>
    void QuickSort(T a[], int n)
    {
        // 1. set a[n-1] the max element
        int maxI = 0;
        for (int i=maxI+1; i < n; ++i)
        {
            if (a[i] > a[maxI])
            {
                maxI = i;
            }
        }
        T tmp = a[n-1];
        a[n-1] = a[maxI];
        a[maxI] = tmp;

        // 2. quick sort
        quicksort(a, 0, n-2);
    }

    // @param a[] left...right
    template<class T>
    void insertSort(T a[], int left, int right)
    {
        for (int i=left+1; i<=right; ++i)
        {
            T t = a[i];
            int j;
            for (j=i-1; j>=left && t < a[j]; j--)
            {
                a[j+1] = a[j];
            }
            a[j+1] = t;
        }
    }


    // @param a[] left...right
    template<class T>
    void quicksort(T a[], int left, int right)
    {
        // a. small sequence use quick sort
        /*
        if (left >= right)
        {
            return;
        }
        */

        // b. use insertion sort instead of quick sort
        //    when size <= nbreak
        if ( right + 1 - left <= QUICK_N_BREAK)
        {
            insertSort(a, left, right);
            return;
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

        // when loop out, pR must point to an element < pivot
        // so exchange it with pivot which is at its left
        a[left] = a[pR];
        a[pR] = pivot;
        
        quicksort(a, left, pR-1);
        quicksort(a, pR+1, right); // this call can change to a while loop, how to ?

    }


    struct Bound
    {
        int left;
        int right;
    public:
        Bound(int l, int r):left(l),right(r){}
    };

    template<class T>
    void QuickSortStack(T a[], int n)
    {

        // 1. set a[n-1] the max element
        int maxI = 0;
        for (int i=maxI+1; i < n; ++i)
        {
            if (a[i] > a[maxI])
            {
                maxI = i;
            }
        }
        T tmp = a[n-1];
        a[n-1] = a[maxI];
        a[maxI] = tmp;


        std::stack<Bound> sBound;
        sBound.push(Bound(0, n-2));
        Bound curB(0, 0);
        int left, right;

        while(!sBound.empty())
        {
            curB = sBound.top();
            sBound.pop();
            left = curB.left;
            right = curB.right;

            // a. small sequence use quick sort
            /*
            if (left >= right)
            {
                continue;
            }
            */

            // b. use insertion sort instead of quick sort
            //    when size <= nbreak
            if ( right + 1 - left <= QUICK_N_BREAK)
            {
                insertSort(a, left, right);
                continue;
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

            // when loop out, pR must point to an element < pivot
            // so exchange it with pivot which is at its left
            a[left] = a[pR];
            a[pR] = pivot;


            if (pR - left > right - pR)
            {
                sBound.push(Bound(left, pR-1));
                sBound.push(Bound(pR+1, right));
            }
            else
            {
                sBound.push(Bound(pR+1, right));
                sBound.push(Bound(left, pR-1));
            }
        }

    }

    void quickSortTest()
    {
        int numE = 234;
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

        //QuickSort(testArray, numE);
        QuickSortStack(testArray, numE);

        std::cout<<"====sorted====="<<std::endl;
        for (int i=0; i<numE; i++)
        {
            std::cout<<testArray[i]<<" ";
        }
        std::cout<<std::endl;

        delete [] testArray;
    }


}