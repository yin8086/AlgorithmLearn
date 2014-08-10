#include <algorithm>
#include <set>

namespace Yincpp
{

    
    // 0/1 knapsack problem
    // when c is small and weight is integer
    //@param p profit
    //@param w weight of type integer
    //@param c capacity of Knapsack
    //@param n item number
    //@param f[1...n][1...c]  (i,j) start from ith item with capacity j
    template<class T>
    void Knapsack0_1(T p[], int w[], int c, int n, T **f)
    {
        // initial f[n][i]
        for (int i=0; i<=c; ++i)
        {
            f[n][i] = i < w[n] ? 0 : p[n];
        }

        // from f[i+1][*] construct f[i][*]
        // except i=1, we only need f[1][c]
        for (int i = n-1; i>1; i--)
        {
            for (int j=1; j<w[i]; ++j)
            {
                f[i][j] = f[i+1][j];
            }
            for (int j=w[i]; j<=c; ++j)
            {
                f[i][j] = std::max(f[i+1][j], f[i+1][j-w[i]] + p[i]);
            }
        }

        f[1][c] = f[2][c];
        if (c >= w[1])
        {
            f[1][c] = std::max(f[2][c], f[2][c-w[1]]+p[1]);
        }
    }




    template<class T>
    void traceBackKS(T **f, int w[], int c, int n, int x[])
    {
        for (int i=1; i<n; ++i)
        {
            if (f[i][c] == f[i+1][c])
            {
                x[i] = 0;
            }
            else
            {
                x[i] = 1;
                c -= w[i];
            }
        }
        x[n] = f[n][c] ? 1 : 0;
    }

    template<class T, class W>
    struct ProfitPair
    {
        T m_y;
        W m_bestP;

    public:
        ProfitPair(T y, W p):m_y(y), m_bestP(p){}
        ProfitPair():ProfitPair(0, 0){}

        friend bool operator < (const ProfitPair<T, W> &lhs, const ProfitPair<T, W> &rhs)
        {
            return lhs.m_y < rhs.m_y;
        }
    };

    template<class T, class W>
    void Knapsack0_1_A(T p[], W w[], W c, int n, std::set<ProfitPair<T,W> > *setA)
    {
        setA[n].insert(ProfitPair<T,W>(0, 0));
        setA[n].insert(ProfitPair<T,W>(w[n], p[n]));

        for (int i=n-1; i>=1; i--)
        {
            for (std::set<ProfitPair<T,W> >::iterator pi = setA[i+1].begin(); 
                    pi != setA[i+1].end(); pi++)
            {
                setA[i].insert(*pi);
                if (pi->m_y + w[i] >= w[i] && pi->m_y + w[i] <= c)
                {
                    setA[i].insert(ProfitPair<T,W>(pi->m_y + w[i], pi->m_bestP+p[i]));
                }
            }

            for (std::set<ProfitPair<T,W> >::iterator pi = setA[i].begin(); 
                pi != setA[i].end(); )
            {
                std::set<ProfitPair<T,W> >::iterator pCur = pi++;
                while (pi != setA[i].end())
                {
                    if (pi->m_y >= pCur->m_y && pi->m_bestP < pCur->m_bestP)
                    {
                        pi = setA[i].erase(pi);
                    }
                    else
                    {
                        break;
                    }
                }
                // pi has move ahead
            }

        }
    }


    template<class T, class W>
    typename std::set<ProfitPair<T,W> >::iterator  max_less_than(const std::set<ProfitPair<T,W>> &setP, W y)
    {
        std::set<ProfitPair<T,W> >::iterator p = setP.begin();
        std::set<ProfitPair<T,W> >::iterator pp;
        while (p != setP.end() && p->m_y <= y)
        {
            pp = p++;
        }

        return pp;
    }


    template<class T, class W>
    void traceBackKSA(std::set<ProfitPair<T,W> > *setA, W w[], W c, int n, int x[])
    {
        std::set<ProfitPair<T,W>>::iterator pi, pi2;
        for (int i=1; i<n; ++i)
        {
            ProfitPair<T, W> tmpP = ProfitPair<T,W>(c, 0);
            pi = max_less_than(setA[i], c);
            pi2 = max_less_than(setA[i+1], c);

            if (!(pi->m_bestP == pi2->m_bestP))
            {
                x[i] = 1;
                c -= w[i];
            }
            else
            {
                x[i] = 0;
            }
            
        }

        pi = max_less_than(setA[n], c);
        x[n] = pi->m_bestP ? 1 : 0;


    }

    void Knapsack0_1Test()
    {
        /*  Test Case 1
        int n = 5;
        int p[] = {0, 6, 3, 5, 4, 6};
        int w[] = {0, 2, 2, 6, 5, 4};
        int c = 10;
        std::set<ProfitPair<int, int>> *pps = new std::set<ProfitPair<int, int>>[n+1];
        int x[6] = {0};
        Knapsack0_1_A(p, w, c, n, pps);
        traceBackKSA(pps, w, c, n, x);
        std::cout<<"Solution is: ";
        for (int i=1; i<=n; ++i)
        {
            std::cout<<x[i]<<(i==n ? "\n":" ");
        }
        std::set<ProfitPair<int,int>>::iterator pi = max_less_than(pps[1], c);
        std::cout<<"Total Profit: "<<pi->m_bestP<<std::endl;
        delete [] pps;
        */

        /* Test case 2
        int n = 5;
        int p[] = {0, 6, 3, 5, 4, 6};
        int w[] = {0, 2, 2, 6, 5, 4};
        int c = 10;
        int **fArray;
        fArray = new int *[6];
        for (int i=0; i < 6; ++i)
        {
            fArray[i] = new int[c+1];
        }
        */

        /* Test case 3
        int n = 4;
        int p[] = {0, 6, 10, 12, 13};
        int w[] = {0, 2, 4, 6, 7};
        int c = 11;
        int **fArray;
        fArray = new int *[n+1];
        for (int i=0; i < n+1; ++i)
        {
            fArray[i] = new int[c+1];
        }

        int x[5] = {0};
        Knapsack0_1(p, w, c, 4, fArray);
        traceBackKS(fArray, w, c, 4, x);

        std::cout<<"Solution is: ";
        for (int i=1; i<=n; ++i)
        {
            std::cout<<x[i]<<(i==n ? "\n":" ");
        }
        std::cout<<"Total Profit: "<<fArray[1][c]<<std::endl;
        
        for (int i=0; i < n+1; ++i)
        {
            delete [] fArray[i];
        }
        
        delete [] fArray;
        */

        //  Test case 4
        int n = 4;
        int p[] = {0, 6, 10, 12, 13};
        int w[] = {0, 2, 4, 6, 7};
        int c = 11;
        std::set<ProfitPair<int, int>> *pps = new std::set<ProfitPair<int, int>>[n+1];
        int x[5] = {0};
        Knapsack0_1_A(p, w, c, n, pps);
        traceBackKSA(pps, w, c, n, x);
        std::cout<<"Solution is: ";
        for (int i=1; i<=n; ++i)
        {
            std::cout<<x[i]<<(i==n ? "\n":" ");
        }
        std::set<ProfitPair<int,int>>::iterator pi = max_less_than(pps[1], c);
        std::cout<<"Total Profit: "<<pi->m_bestP<<std::endl;
        delete [] pps;
        
    }


}