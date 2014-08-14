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

    template<class Tw, class Tp>
    class Knap;


    class bbnode
    {
        template<class Tw, class Tp>
        friend class Knap;

        bbnode *m_parent;
        bool isLeft;
    };

    template<class Tw, class Tp>
    class HeapNode
    {
        friend class Knap<Tw, Tp>;

        bbnode *m_ptr;  // corresponding bbnode
        Tp m_uprofit;    // upper bound profit
        Tp m_profit;
        Tw m_weight;
        int m_level; // node dequeue randomly, so must save level

    public:
        bool operator < (const HeapNode<Tw, Tp> &rhs)
        {
            return m_uprofit < rhs.m_uprofit;
        }
    };


    template<class Tw, class Tp>
    Tp Knapsack(Tp p[], Tw w[], Tw c, int n, int x[]);

    template<class Tw, class Tp>
    Tp KnapsackBB(Tp p[], Tw w[], Tw c, int n, int x[]);

    template<class Tw, class Tp>
    class Knap
    {
        friend Tp Knapsack<Tw, Tp>(Tp p[], Tw w[], Tw c, int n, int x[]);
        friend Tp KnapsackBB<Tw, Tp>(Tp p[], Tw w[], Tw c, int n, int x[]);
    private:
        void Knapsack(int i);
        Tp Bound(int i);

        Tp MaxProfitKnapsack();
        void addLiveNode(MaxHeap<HeapNode<Tw, Tp> > &H, bbnode *E, Tp up, Tp cp, Tw cw, bool ch, int level);
        Tw c; // capacity
        int n;
        Tw *w;
        Tp *p;
        Tw cw;
        Tp cp;
        int *x, *bestx;
        Tp bestp;
        std::list<bbnode *> m_bag;
    public:
        ~Knap()
        {
            for (std::list<bbnode *>::iterator p = m_bag.begin(); p != m_bag.end(); p++)
            {
                delete *p;
            }
        }
    };

    template<class Tw, class Tp>
    Tp Knap<Tw, Tp>::Bound(int i)
    {
        Tw cleft = c - cw; // all the weight available left
        Tw ap = cp;

        // go to left
        while (i <= n && w[i] <= cleft)
        {
            cleft -= w[i];
            ap += p[i];
            i++;
        }

        if (i <= n)
        {
            ap += cleft/w[i] * p[i];
        }

        return ap;
    }

    template<class Tw, class Tp>
    void Knap<Tw, Tp>::Knapsack(int i)
    {
        // i level or ith

        if (1 <= i && i <= n)
        {
            if (cw + w[i] <= c) // left tree, x[i] = 1
            {
                x[i] = 1;
                cw += w[i];
                cp += p[i];
                Knapsack(i+1);
                cw -= w[i];
                cp -= p[i];
            }

            if (Bound(i+1) > bestp) // if use all the weights left. then right tree
            {
                x[i] = 0;
                Knapsack(i+1);
            }

            bestx[i] = x[i];
        }
        else
        {
            /*
            for (int j=1; j<=n; ++j)
            {
                bestx[j] = x[j];
            }
            */
            bestp = cp;
        }
        // new method for bestx
        //bestx[i-1] = x[i-1];
    }

    class KSObject
    {
        friend int Knapsack<int, int>(int p[], int w[], int c, int n, int x[]);
        friend int KnapsackBB<int, int>(int p[], int w[], int c, int n, int x[]);
    public:
        bool operator <=(const KSObject &rhs) const{ return d >= rhs.d;}
    private:
        int ID;
        double d;
    };

    template<class Tw, class Tp>
    Tp Knapsack(Tp p[], Tw w[], Tw c, int n, int x[])
    {
        Tw W = 0;
        Tp P = 0;

        KSObject *KQ = new KSObject[n];
        for (int i=1; i<=n; ++i)
        {
            KQ[i-1].ID = i;
            KQ[i-1].d = 1.0*p[i]/w[i];
            P += p[i];
            W += w[i];
        }
        if (W <= c)
        {
            return P;
        }

        MergeSort(KQ, n);

        Knap<Tw, Tp> K;
        K.p = new Tp[n+1];
        K.w = new Tw[n+1];
        for (int i=1; i<=n; ++i)
        {
            K.p[i] = p[KQ[i-1].ID];
            K.w[i] = w[KQ[i-1].ID];
        }
        K.cp = K.cw = 0;
        K.c = c;
        K.n = n;
        K.x = new int[n+1];
        K.bestx = new int[n+1];
        K.bestp = 0;

        K.Knapsack(1);
        for (int i=1; i<=n; ++i)
        {
            x[KQ[i-1].ID] = K.bestx[i];
        }
        delete [] KQ;
        delete [] K.p;
        delete [] K.w;
        delete [] K.x;
        delete [] K.bestx;

        return K.bestp;
        

    }
    
    template<class Tw, class Tp>
    void Knap<Tw, Tp>::addLiveNode(MaxHeap<HeapNode<Tw, Tp> > &H, bbnode *E, Tp up, Tp cp, Tw cw, bool ch, int level)
    {
        bbnode *pb = new bbnode;
        pb->isLeft = ch;
        pb->m_parent = E;
        m_bag.push_back(pb);

        HeapNode<Tw, Tp> hn;
        hn.m_level = level;
        hn.m_ptr = pb;
        hn.m_weight = cw;
        hn.m_profit = cp;
        hn.m_uprofit = up;
        H.insert(hn);
    }

    template<class Tw, class Tp>
    Tp Knap<Tw, Tp>::MaxProfitKnapsack()
    {
        MaxHeap<HeapNode<Tw, Tp> > mh(1000);
        int level = 1;
        bbnode *E = nullptr;
        Tp up = Bound(1);

        while (level != n+1)
        {
            if (cw + w[level] <= c)
            {
                if (cp + p[level] > bestp)
                {
                    bestp = cp + p[level];
                }
                addLiveNode(mh, E, up, cp + p[level], cw + w[level], true, level+1);
            }
            up = Bound(level+1);
            if (up >= bestp)   // why >= ?
            {
                addLiveNode(mh, E, up, cp, cw, false, level+1);
            }

            HeapNode<Tw, Tp> hn;
            mh.deleteMax(hn);
            E = hn.m_ptr;
            cw = hn.m_weight;
            cp = hn.m_profit;
            up = hn.m_uprofit;
            level= hn.m_level;
        }

        for (int i=n; i>0; --i)
        {
            bestx[i] = E->isLeft ? 1 : 0;
            E = E->m_parent;
        }

        return cp;
    }

    template<class Tw, class Tp>
    Tp KnapsackBB(Tp p[], Tw w[], Tw c, int n, int x[])
    {
        Tw W = 0;
        Tp P = 0;

        KSObject *KQ = new KSObject[n];
        for (int i=1; i<=n; ++i)
        {
            KQ[i-1].ID = i;
            KQ[i-1].d = 1.0*p[i]/w[i];
            P += p[i];
            W += w[i];
        }
        if (W <= c)
        {
            return P;
        }

        MergeSort(KQ, n);

        Knap<Tw, Tp> K;
        K.p = new Tp[n+1];
        K.w = new Tw[n+1];
        for (int i=1; i<=n; ++i)
        {
            K.p[i] = p[KQ[i-1].ID];
            K.w[i] = w[KQ[i-1].ID];
        }
        K.cp = K.cw = 0;
        K.c = c;
        K.n = n;
        K.bestx = new int [n+1];
        K.bestp = 0;

        K.bestp = K.MaxProfitKnapsack();

        for (int i=1; i<=n; ++i)
        {
            x[KQ[i-1].ID] = K.bestx[i];
        }
        delete [] KQ;
        delete [] K.p;
        delete [] K.w;
        delete [] K.bestx;
        
        return K.bestp;
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

    void KnapsackBTTest()
    {
        // Test case 16.7
        /*int w[] = {0, 3, 5, 2, 1};
        int p[] = {0, 9, 10, 7, 4};
        int n=4;
        int c = 7;
        int bestX[5];*/

        // Test case 2
        /*
        int n = 4;
        int p[] = {0, 6, 10, 12, 13};
        int w[] = {0, 2, 4, 6, 7};
        int c = 11;
        int bestX[5];*/
        


        // Test case 3
        int n = 5;
        int p[] = {0, 6, 3, 5, 4, 6};
        int w[] = {0, 2, 2, 6, 5, 4};
        int c = 10;
        int bestX[6];

        int bestP = Knapsack(p, w, c, n, bestX);
        std::cout<<"Best Solution Profit: "<<bestP<<std::endl;
        for (int i=1; i<=n; ++i)
        {
            std::cout<<bestX[i]<<(i==n?"\n":" ");
        }
    }

    void KnapsackBBTest()
    {
        // Test case 16.7
        /*int w[] = {0, 3, 5, 2, 1};
        int p[] = {0, 9, 10, 7, 4};
        int n=4;
        int c = 7;
        int bestX[5];*/

        // Test case 2
        /*
        int n = 4;
        int p[] = {0, 6, 10, 12, 13};
        int w[] = {0, 2, 4, 6, 7};
        int c = 11;
        int bestX[5];
        */


        // Test case 3
        int n = 5;
        int p[] = {0, 6, 3, 5, 4, 6};
        int w[] = {0, 2, 2, 6, 5, 4};
        int c = 10;
        int bestX[6];
        int bestP = KnapsackBB(p, w, c, n, bestX);
        std::cout<<"Best Solution Profit: "<<bestP<<std::endl;
        for (int i=1; i<=n; ++i)
        {
            std::cout<<bestX[i]<<(i==n?"\n":" ");
        }
    }

}