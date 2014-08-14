namespace Yincpp
{
    template<class T>
    T MaxLoading(T w[], T c, int n, int bestx[]);

    template<class T>
    class Loading
    {
        // must add <T> to show it is a template, not a function
        friend T MaxLoading<T>(T w[], T c, int n, int bestx[]);

    private:
        void maxLoading(int i); //@param i level 1...n
        int n; // number of containers
        T *w, // ith container weight
           c, // capacity
           cw, // current weight
           bestw, // best weight so far
           r,  // left container weight
           *x, // current best x[i]
           *bestx;
    };

    template<class T>
    void Loading<T>::maxLoading(int i)
    {
        if(1 <= i && i <= n)
        {
            r -= w[i];

            // first continuely left until can
            // not left
            if (cw + w[i] <= c) // left tree
            {
                cw += w[i];
                x[i] = 1;
                maxLoading(i+1);
                cw -= w[i];
            }

            // try right, if may lead to better
            // go right subtree, in it try left
            if (cw + r > bestw) // right tree
            {
                x[i] = 0;
                maxLoading(i+1);
            }

            r += w[i];

            
        }

        else
        {
            // must lead to better solution
            bestw = cw;
            /*
            for (int j=1; j<=n; ++j)
            {
                bestx[j] = x[j];
            }
            */


        }

        // new method for bestx
        bestx[i-1] = x[i-1];
    }



    template<class T>
    T MaxLoading(T w[], T c, int n, int bestx[])
    {
        Loading<T> X;
        X.x = new T[n+1];
        X.w = w;
        X.c = c;
        X.n = n;
        X.bestx = bestx;
        X.bestw = 0;
        X.cw = 0;
        X.r = 0;
        for (int i=1; i<=n; ++i)
        {
            X.r += w[i];
        }
        X.maxLoading(1);
        delete [] X.x;
        return X.bestw;
    }

    template<class T>
    class LoadingFIFO;

    template<class T>
    class QNode
    {
        friend class LoadingFIFO<T>;
    private:
        QNode *m_parent;
        bool m_isLeft;
        T m_weight;
    };

    template<class T>
    class LoadingFIFO
    {
    private:
        std::list<QNode<T> *> m_bag;
    public:
        ~LoadingFIFO()
        {
            for (std::list<QNode<T> *>::iterator p = m_bag.begin(); p != m_bag.end(); p++)
            {
                delete *p;
            }
        }
        void addLiveNode(std::queue<QNode<T> *> &Q, T wt, int i,
            int n, T bestw, QNode<T> *E, QNode<T> *&bestE, int bestx[], bool ch);

        T MaxLoading(T w[], T c, int n, int bestx[]);
    };

    template<class T>
    T LoadingFIFO<T>::MaxLoading(T w[], T c, int n, int bestx[])
    {
        std::queue<QNode<T> *> Q;
        Q.push(nullptr);
        int i = 1; // level
        T Ew = 0,
          bestw = 0,
          r = 0;
        for (int j=2; j<=n; ++j)
        {
            r += w[j];
        }

        QNode<T> *E = nullptr,
                 *bestE = nullptr;

        while (true)
        {
            T wt = Ew + w[i];
            if (wt <= c)
            {
                if (wt > bestw) bestw = wt; // update bestw for early terminate

                addLiveNode(Q, wt, i, n, bestw, E, bestE, bestx, true);
            }

            if (Ew + r > bestw)
            {
                addLiveNode(Q, Ew, i, n, bestw, E, bestE, bestx, false);
            }

            E = Q.front(); Q.pop();
            if (!E) // end of a level
            {
                if (Q.empty())
                {
                    break;
                }

                Q.push(nullptr);
                E = Q.front(); Q.pop(); // get a next level node
                i++;
                r -= w[i];   // go to i+1 level and substract w[i+1]
            }
            Ew = E->m_weight; 
        }

        for (int j=n-1; j >0; --j)
        {
            bestx[j] = bestE->m_isLeft ? 1 : 0;
            bestE = bestE->m_parent;
        }

        return bestw;
    }

    template<class T>
    void LoadingFIFO<T>::addLiveNode(std::queue<QNode<T> *> &Q, T wt, int i,
        int n, T bestw, QNode<T> *E, QNode<T> *&bestE, int bestx[], bool ch)
    {
        if (i == n)
        {
            // we have change bestw before
            if (wt == bestw)
            {
                bestE = E;
                bestx[n] = ch;
            }
            return;
        }

        QNode<T> *b = new QNode<T>;
        m_bag.push_back(b);
        b->m_weight = wt;
        b->m_isLeft = ch;
        b->m_parent = E;
        Q.push(b);
    }


    template<class T>
    class LoadingMax;


    class bbnode
    {
        template<class T>
        friend class LoadingMax;

        bbnode *m_parent;
        bool isLeft;
    };

    template<class T>
    class HeapNode
    {
        friend class LoadingMax<T>;

        bbnode *m_ptr;  // corresponding bbnode
        T m_uweight;    // upper bound weight
        int m_level; // node dequeue randomly, so must save level
        // T m_weight; // Node weight

    public:
        bool operator < (const HeapNode &rhs)
        {
            return m_uweight < rhs.m_uweight;
        }
    };

    template<class T>
    class LoadingMax
    {
    private:
        std::list<bbnode *> m_bag;
    public:
        ~LoadingMax()
        {
            for (std::list<bbnode *>::iterator p = m_bag.begin(); p != m_bag.end(); p++)
            {
                delete *p;
            }
        }
        T MaxLoading(T w[], T c, int n, int bestx[]);
        void addLiveNode(MaxHeap<HeapNode<T> > &H, bbnode *E, int level,
            T uweight, bool ch);
    };

    template<class T>
    T LoadingMax<T>::MaxLoading(T w[], T c, int n, int bestx[])
    {
        T *r = new T[n+1];
        MaxHeap<HeapNode<T> > mh(1000);
        // node are randomly deleted from heap,
        // so we must store every r[i] according to
        // its level
        r[n] = 0;
        for (int i=n-1; i>=1; --i)
        {
            r[i] = w[i+1] + r[i+1];
        }

        int level = 1;
        T Ew = 0, bestw = 0;

        bbnode *E = nullptr;
        while (level != n+1)
        {
            // Node level is level+1
            if (Ew + w[level] <= c)
            {
                // use bestw
                if (Ew + w[level] > bestw)
                {
                    bestw = Ew + w[level];
                }
                // use bestw
                addLiveNode(mh, E, level+1, Ew + w[level] + r[level], true);
            }
            // Although we can update bestw, and use up bound to check
            // before insertion, however as bestw increases available
            // node will be unavailable after some time, we may need 
            // double ended priority queue
            if (Ew + r[level] > bestw)
                addLiveNode(mh, E, level+1, Ew + r[level], false);

            // every time we delete the HeapNode with max upper bound
            // so if the HeapNode with max upper bound is level n+1
            // the upper bound is its final weight, and it's the 
            // best solution
            HeapNode<T> curNode;
            mh.deleteMax(curNode);
            E = curNode.m_ptr;
            level = curNode.m_level;
            Ew = curNode.m_uweight - r[level-1]; // so here is r[level-1]
        }

        for (int i=n; i>0; --i)
        {
            bestx[i] = E->isLeft;
            E = E->m_parent;
        }
               
        delete [] r;
        return Ew;
    }

    template<class T>
    void LoadingMax<T>::addLiveNode(MaxHeap<HeapNode<T> > &H, bbnode *E, int level,
        T uweight, bool ch)
    {

        bbnode *pb = new bbnode;
        pb->isLeft = ch;
        pb->m_parent = E;
        m_bag.push_back(pb);

        HeapNode<T> hn;
        hn.m_level = level;
        hn.m_uweight = uweight;
        hn.m_ptr = pb;
        H.insert(hn);
    }

    void ContainerLoadingTest()
    {
        // Test case 16.5
        int w[] = {0, 8, 6, 2, 3};
        int n=4;
        int c = 12;
        int bestX[6];
        int bestW = MaxLoading(w, c, n, bestX);
        std::cout<<"Best Solution Capacity: "<<bestW<<std::endl;
        for (int i=1; i<=n; ++i)
        {
            std::cout<<bestX[i]<<(i==n?"\n":" ");
        }

    }

    void ContainerLoadingTestBB()
    {
        int w[] = {0, 8, 6, 2, 3};
        int n=4;
        int c = 12;
        int bestX[6];
        LoadingFIFO<int> lff;
        int bestW = lff.MaxLoading(w, c, n, bestX);
        std::cout<<"Best Solution Capacity: "<<bestW<<std::endl;
        for (int i=1; i<=n; ++i)
        {
            std::cout<<bestX[i]<<(i==n?"\n":" ");
        }
    }

    void ContainerLoadingTestMax()
    {
        int w[] = {0, 8, 6, 2, 3};
        int n=4;
        int c = 12;
        int bestX[6];
        LoadingMax<int> lm;
        int bestW = lm.MaxLoading(w, c, n, bestX);

        std::cout<<"Best Solution Capacity: "<<bestW<<std::endl;
        for (int i=1; i<=n; ++i)
        {
            std::cout<<bestX[i]<<(i==n?"\n":" ");
        }
    }

}