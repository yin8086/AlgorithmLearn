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

    template<class T>
    void quick3Sort(T a[], int left, int right)
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

        // pL point to next left-most position
        // pR point to next right-most position
        // p  point to the current move position
        int pL = left, p = left+1, pR = right;
        T pivot = a[left];

        while (p <= pR)
        {
            int cmp = a[p] - pivot;
            if (cmp < 0)
            {
                // swap smaller to left-most
                // swap pivot to middle
                // p must go ahead because old p point to pivot
                std::swap(a[pL++], a[p++]);  
            }
            else if (cmp > 0)
            {
                /// old method
                // swap bigger to right-most
                // wait to process the element in original right-most pos
                /*std::swap(a[p], a[pR--]);  */
                /// old method

                /// new method
                while (a[pR] - pivot > 0)
                    pR--;
                if (p < pR)
                {
                    std::swap(a[p], a[pR--]);
                }
                /// new method
            }
            else
            {
                // pass a pivot
                p++;
            }
        }
        // a[left...pL-1] < pivot = a[pL...pR] < a[pR+1...right]
        quick3Sort(a, left, pL-1);
        quick3Sort(a, pR+1, right);
    }


    // type T must support for operator -
    template<class T>
    void Quick3Sort(T a[], int n)
    {
        quick3Sort(a, 0, n-1);
    }

    template<class T>
    void quickSortOneWay(T a[], int left, int right)
    {
        if (left >= right)
        {
            return;
        }

        int pI = left, pJ = left + 1;
        T pivot = a[left];

        while (pJ <= right)
        {
            while (pJ <= right && a[pJ] > pivot)
                pJ ++;

            if (pJ <= right)
            {
                std::swap(a[++pI], a[pJ++]);
            }

        }

        a[left] = a[pI];
        a[pI] = pivot;
        quickSortOneWay(a, left, pI-1);
        quickSortOneWay(a, pI+1, right);
    }

    template<class T>
    void QuickSortOneWay(T a[], int n)
    {
        quickSortOneWay(a, 0, n-1);
    }



}