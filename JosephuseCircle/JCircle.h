#include <list>
namespace Yincpp
{
    // use circle list to simulate the delete m'th element process
    int lastRemaining(unsigned int n, unsigned int m)
    {
        if (n < 1 || m < 1)
        {
            return -1;
        }

        unsigned int i = 0;
        std::list<int> numbers;
        for (i = 0; i < n; ++i)
        {
            numbers.push_back(i);
        }

        typedef std::list<int>::iterator ListIter;

        ListIter pi = numbers.begin();
        while (numbers.size() > 1)
        {
            for (int j = 1; j < m; ++j)
            {
                pi++;
                if (pi == numbers.end())
                {
                    pi = numbers.begin();
                }
            }

            pi = numbers.erase(pi);
            if (pi == numbers.end())
            {
                pi = numbers.begin();
            }
        }

        return *pi;
    }

    /* ***
    f(n, m) is n numbers, every time remove m'th element, then the left number is f(n, m)
    so,
    f(1, m) = 0 ( value stands for the index of the left number in 0~n-1

    0...n-1, delete m'th element, the element's index k is indeed (m-1)%n
    after remove [k] element, the list is
    0...k-1 k+1...n-1        reorder to
    k+1...   n-1       0...  k-1        map to
    |        |        |      |          [use 
    V        V        V      V          [ x - (k+1) ] % n  ->  [ x + n - (k+1) ] % n
    0     n-k-1     n-k-1    n-2

    reverse map is [x + (k+1)] % n, and k = (m-1)%n
    so f'(n-1, m) = [f(n-1, m) + m] % n
    and 
    0...n-1 always remove m, the left one is the same as
    0...k-1 k+1...n-1 always remove m, the left one
    that is to say f(n, m) = f'(n-1, m) = [f(n-1, m) + m] % n

    Conclude
       f(1, m) = 0
       f(n, m) = [f(n-1, m) + m] % n
    */
    // dynamic programming
    int lastRemainingDynamic(unsigned int n, unsigned int m)
    {
        if (n < 1 || m < 1)
        {
            return -1;
        }

        int last = 0;
        for (int i = 2; i <= n; ++i)
        {
            last = (last + m) % i;
        }

        return last;
    }
}