namespace Yincpp
{
    class Network
    {
    public:
        virtual int begin(int vi) = 0;
        virtual int nextVertex(int vi) = 0;
        virtual void initialPos() = 0;
        virtual void deactivePos() = 0;

        virtual int edges() const = 0;
        virtual int vertices() const = 0;

        virtual void BFS(int vi, int reach[], int label);
        virtual void DFS(int vi, int reach[], int label);
        bool Topological(int v[]);
    };

    void Network::BFS(int vi, int reach[], int label)
    {
        std::queue<int> qV;
        initialPos();

        int curV, adjV;
        reach[vi] = label;
#ifdef _DEBUG
        std::cout<<vi;
#endif
        qV.push(vi);
        while(!qV.empty())
        {
            curV = qV.front();
            qV.pop();

            adjV = begin(curV);
            while(adjV)
            {
                if (!reach[adjV])
                {
                    reach[adjV] = label;
#ifdef _DEBUG
                    std::cout<<adjV;
#endif
                    qV.push(adjV);
                }
                adjV = nextVertex(curV);
            }
        }
        
    }

    void Network::DFS(int vi, int reach[], int label)
    {
        initialPos();
        std::stack<int> sV;
        int curV, adjV;
        sV.push(vi);
        while (!sV.empty())
        {
            curV = sV.top();
            if (!reach[curV])
            {
                reach[curV] = label;
#ifdef _DEBUG
                std::cout<<curV;
#endif
                adjV = begin(curV);
                if (adjV)
                {
                    sV.push(adjV);
                }
            }
            else
            {
                adjV = nextVertex(curV);
                if (adjV)
                {
                    sV.push(adjV);
                }
                else
                {
                    sV.pop();
                }
            }
        }
        


        deactivePos();
    }

    bool Network::Topological(int v[])
    {
        
        int n_v = vertices();
        int *inNum = new int [n_v+1];
        memset(inNum, 0, sizeof(int)*(n_v+1));
        initialPos();

        // 1. calculate indegree
        for (int i=1; i<=n_v; ++i)
        {
            int j = begin(i);
            while (j)
            {
                inNum[j] ++;
                j = nextVertex(i);
            }
        }
        std::stack<int> vStack;
        for (int i=1; i<=n_v; ++i)
        {
            if (inNum[i] == 0)
            {
                vStack.push(i);
            }
        }

        int vTop = 0;
        int curVert;
        while (!vStack.empty())
        {
            v[vTop++] = curVert = vStack.top();
            vStack.pop();

            int j = begin(curVert);
            while (j)
            {
                if ((--inNum[j]) == 0)
                {
                    vStack.push(j);
                }
                j = nextVertex(curVert);
            }
        }
        
        deactivePos();
        delete [] inNum;
        return vTop == n_v;
    }
    
    struct NodeType
    {
        int left;
        int right;
    };

    class Undirected: virtual public Network
    {
    private:
        int *m_bin;
        NodeType *m_node;

        void createBins(int b, int n);
        void destroyBins() { delete [] m_node; delete [] m_bin;}
        void insertBins(int b, int v);
        void moveBins(int bMax, int toBin, int v);
        

    public:
        virtual int degree(int vi) const = 0;
        bool BipartiteCover(int L[], int C[], int &m);
    };

    void Undirected::createBins(int b, int n)
    {
        m_node = new NodeType[n+1];
        m_bin = new int [b+1];
        for (int i=0; i<=b; ++i)
        {
            m_bin[i] = 0;
        }
    }

    void Undirected::insertBins(int b, int v)
    {
        if (!b)
        {
            return ;
        }
        m_node[v].left = b;
        if (m_bin[b])
        {
            m_node[m_bin[b]].left = v;
        }
        m_node[v].right = m_bin[b];
        m_bin[b] = v;
    }

    void Undirected::moveBins(int bMax, int toBin, int v)
    {
        int l = m_node[v].left;
        int r = m_node[v].right;

        if (r)
        {
            m_node[r].left = m_node[v].left;
        }
        if (l > bMax || m_bin[l] != v)
        {
            m_node[l].right = r;
        }
        else
        {
            m_bin[l] = r;
        }
        insertBins(toBin, v);
    }

    bool Undirected::BipartiteCover(int L[], int C[], int &m)
    {
        int n_v = vertices();

        int sizeOfA = 0;
        for (int i=1; i<=n_v; ++i)
        {
            if (L[i] == 1)
            {
                sizeOfA++;
            }
        }
        int sizeOfB = n_v - sizeOfA;

        createBins(sizeOfB, n_v);

        int *New = new int [n_v+1];
        bool *Change = new bool [n_v+1];
        bool *Cov = new bool [n_v+1];
        initialPos();
        std::stack<int> S;

        for (int i=1; i<=n_v; ++i)
        {
            Cov[i] = Change[i] = false;
            if (L[i] == 1)
            {
                New[i] = degree(i);
                insertBins(New[i], i); // i insert New[i] bin, New[i] is value
            }
        }

        int covered = 0, MaxBin = sizeOfB;
        m = 0;

        while (MaxBin > 0)
        {
            if (m_bin[MaxBin])
            {
                int v = m_bin[MaxBin];
                C[m++] = v;
                int j = begin(v), k;
                while (j)
                {
                    if (!Cov[j])
                    {
                        Cov[j] = true;
                        covered ++;
                        k = begin(j);
                        while (k)
                        {
                            New[k] --;
                            if (!Change[k])
                            {
                                S.push(k);
                                Change[k] = true;
                            }
                            k = nextVertex(j);

                        }

                    }
                    j = nextVertex(v);
                }

                while (!S.empty())
                {
                    k = S.top();
                    S.pop();
                    Change[k] = false;
                    moveBins(sizeOfB, New[k], k);
                }
            }
            else MaxBin --;
        }

        deactivePos();
        destroyBins();
        delete [] New;
        delete [] Change;
        delete [] Cov;

        return covered == sizeOfB;


    }

    template<class T>
    struct EdgeNode
    {
        T m_weight;
        int m_u, m_v;
    public:
        operator T() const { return m_weight;}
    };

    template<class T>
    class WNetwork: virtual public Network
    {
    public:
        virtual void First(int i, int &j, T &c) = 0;
        virtual void Next(int i, int &j, T &c) = 0;
        bool Kruskal(EdgeNode<T> t[]);
    };


    template<class T>
    bool WNetwork<T>::Kruskal(EdgeNode<T> t[])
    {
        int n_v = vertices();
        int n_e = edges();
        initialPos();
        EdgeNode<T> *E = new EdgeNode<T>[n_e + 1];
        int k=0; 
        for (int i=1; i<=n_v; ++i)
        {
            int j;
            T c;
            First(i, j, c);
            while(j)
            {
                if (i < j)
                {
                    E[++k].m_weight = c;
                    E[k].m_u = i;
                    E[k].m_v = j;
                }
                Next(i, j, c);
            }
        }

        MinHeap<EdgeNode<T> > mh(1);
        mh.initialize(E, n_e, n_e);
        UnionFind uf(n_v);

        int numEdge = 0;
        k = 0;
        EdgeNode<T> tmpE;
        while (n_e && k < n_v-1)
        {
            mh.deleteMin(tmpE);
            n_e--;
            int u = tmpE.m_u-1; // in union find, pos start from 0
            int v = tmpE.m_v-1;
            if (!uf.connected(u, v))
            {
                t[k++] = tmpE;
                uf.Combine(u, v);
            }
        }

        mh.deactive();
        deactivePos();
        delete [] E;
        return k == n_v-1;
    }


    // Matrix hierarchy
    template<class T>
    class MatrixWDigraph : virtual public Network
    {
    protected:
        T m_NoEdge;

        T **m_a;
        int m_numVert;
        int m_numEdge;
        int *m_iterPos;
    public:
        MatrixWDigraph(int vert = 10, T noEdge = 0);
        ~MatrixWDigraph() { delete2DArray(m_a, m_numVert);}
        bool exist(int i, int j) const;
        int outDegree(int vi) const;
        int inDegree(int vi) const;

        virtual int begin(int vi);
        virtual int nextVertex(int vi);
        virtual void initialPos();
        virtual void deactivePos();

        virtual int edges() const { return m_numEdge;}
        virtual int vertices() const { return m_numVert;}

        MatrixWDigraph<T>& addEdge(int vi, int vj, const T& weight);
        MatrixWDigraph<T>& removeEdge(int vi, int vj);

        void shortestPath(int s, T d[], int p[]);
        void allPairsShortestPath(T **c, int **kay);
    };

    template<class T>
    MatrixWDigraph<T>::MatrixWDigraph(int vert /* = 10 */, T noEdge /* = 0 */)
        :m_NoEdge(noEdge)
    {
        m_numVert = vert;
        m_numEdge = 0;
        make2DArray(m_a, m_numVert+1, m_numVert+1);
        for (int i=1; i<=m_numVert; ++i)
        {
            for (int j=1; j<=m_numVert; ++j)
            {
                m_a[i][j] = m_NoEdge;
            }
        }
    }

    template<class T>
    bool MatrixWDigraph<T>::exist(int i, int j) const
    {
        if (i < 1 || j < 1 || i > m_numVert || j > m_numVert || m_a[i][j] == m_NoEdge)
        {
            return false;
        }
        else
            return true;

    }

    template<class T>
    MatrixWDigraph<T>& MatrixWDigraph<T>::addEdge(int vi, int vj, const T& weight)
    {
        if (vi < 1 || vj < 1 || vi > m_numVert || vj > m_numVert || vi == vj || m_a[vi][vj] != m_NoEdge)
        {
            throw BadInput();
        }

        m_a[vi][vj] = weight;
        m_numEdge ++ ;
        return *this;
    }

    template<class T>
    MatrixWDigraph<T>& MatrixWDigraph<T>::removeEdge(int vi, int vj)
    {
        if (vi < 1 || vj < 1 || vi > m_numVert || vj > m_numVert ||  m_a[vi][vj] == m_NoEdge)
        {
            throw BadInput();
        }

        m_a[vi][vj] = m_NoEdge;
        m_numEdge --;
        return *this;
    }

    template<class T>
    int MatrixWDigraph<T>::outDegree(int vi) const
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw BadInput();
        }

        int sum = 0;
        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                sum ++ ;
            }
        }

        return sum;
    }

    template<class T>
    int MatrixWDigraph<T>::inDegree(int vi) const
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw BadInput();
        }

        int sum = 0;
        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_a[i][vi] != m_NoEdge)
            {
                sum ++ ;
            }
        }

        return sum;
    }

    template<class T>
    void MatrixWDigraph<T>::initialPos()
    {
        m_iterPos = new int [m_numVert + 1]; // all set to zero
    }

    template<class T>
    void MatrixWDigraph<T>::deactivePos()
    {
        delete [] m_iterPos;
    }

    template<class T>
    int  MatrixWDigraph<T>::begin(int vi)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }

        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                m_iterPos[vi] = i;
                return i;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        return 0;
    }

    template<class T>
    int  MatrixWDigraph<T>::nextVertex(int vi)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }
        for (int i=m_iterPos[vi]+1; i <= m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                m_iterPos[vi] = i;
                return i;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        return 0;
    }

    template<class T>
    void MatrixWDigraph<T>::shortestPath(int s, T d[], int p[])
    {
        if (s < 1 || s > m_numVert)
        {
            throw OutOfBounds();
        }

        Chain<int> L;

        ChainIterator<int> CI;

        for (int i=1; i<= m_numVert; ++i)
        {
            d[i] = m_a[s][i];
            if (d[i] == m_NoEdge)
            {
                p[i] = 0;
            }
            else
            {
                p[i] = s;
                L.insert(0, i); // vertex need to consider
            }
        }

        while (!L.isEmpty())
        {
            int *v = CI.initialize(L);
            int *w = CI.next();

            while (w) // get min element
            {
                if (d[*w] < d[*v])
                {
                    v = w;
                }
                w = CI.next();
            }

            int i = *v;
            L.remove(*v);
            for (int j=1; j<=m_numVert; ++j)
            {
                // has a route from i to j
                // and no other way (!p[j])/(d[j] == m_NoEdge)
                if (m_a[i][j] != m_NoEdge && (!p[j] || d[i] + m_a[i][j] < d[j]) )
                {
                    d[j] = d[i] + m_a[i][j];
                    if (!p[j])
                    {
                        L.insert(0, j);
                    }
                    p[j] = i;
                }
            }

        }



    }


    template<class T>
    void MatrixWDigraph<T>::allPairsShortestPath(T **c, int **kay)
    {
        for (int i=1; i<=m_numVert; ++i)
        {
            for (int j=1; j<=m_numVert; ++j)
            {
                c[i][j] = m_a[i][j];
                kay[i][j] = 0;
            }
        }
        for (int i=1; i<=m_numVert; ++i) c[i][i] = 0;

        for (int k=1; k<=m_numVert; ++k)
        {
            for (int i=1; i<=m_numVert; ++i)
            {
                for (int j=1; j<=m_numVert; ++j)
                {
                    T dist1 = c[i][j];
                    T distHi = c[i][k];
                    T distLo = c[k][j];

                    if (distHi != m_NoEdge && distLo != m_NoEdge &&
                        (dist1 == m_NoEdge || distHi+distLo < dist1) )
                    {
                        c[i][j] = distHi+distLo;
                        kay[i][j] = k;
                    }
                }
            }
        }

    }


    template<class T>
    class MatrixWGraph : public MatrixWDigraph<T>, virtual public WNetwork<T>
    {
    public:
        MatrixWGraph(int vert = 10, T noEdge = 0) : MatrixWDigraph(vert, noEdge)
        {

        }

        MatrixWGraph<T>& addEdge(int vi, int vj, const T& weight)
        {
            MatrixWDigraph::addEdge(vi, vj, weight);
            m_a[vj][vi] = weight;
            return *this;
        }

        MatrixWGraph<T>& removeEdge(int vi, int vj)
        {
            MatrixWDigraph::removeEdge(vi, vj);
            m_a[vj][vi] = m_NoEdge;
            return *this;
        }

        int degree(int vi) const
        {
            return outDegree(vi);
        }

        virtual void First(int i, int &j, T &c);
        virtual void Next(int i, int &j, T &c);

        virtual int begin(int vi);
        virtual int nextVertex(int vi);
        virtual void initialPos();
        virtual void deactivePos();

        virtual int edges() const { return m_numEdge;}
        virtual int vertices() const { return m_numVert;}

    };


    template<class T>
    void MatrixWGraph<T>::First(int vi, int &vj, T &c)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }

        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                vj = m_iterPos[vi] = i;
                c = m_a[vi][i];
                return;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        vj = 0;
        c = m_NoEdge;
        return;
    }

    template<class T>
    void MatrixWGraph<T>::Next(int vi, int &vj, T &c)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }
        for (int i=m_iterPos[vi]+1; i <= m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                vj = m_iterPos[vi] = i;
                c = m_a[vi][i];
                return;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        vj = 0;
        c = m_NoEdge;
        return;
    }

    template<class T>
    void MatrixWGraph<T>::initialPos()
    {
        m_iterPos = new int [m_numVert + 1]; // all set to zero
    }

    template<class T>
    void MatrixWGraph<T>::deactivePos()
    {
        delete [] m_iterPos;
    }

    template<class T>
    int  MatrixWGraph<T>::begin(int vi)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }

        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                m_iterPos[vi] = i;
                return i;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        return 0;
    }

    template<class T>
    int  MatrixWGraph<T>::nextVertex(int vi)
    {
        if (vi < 1 || vi > m_numEdge)
        {
            throw OutOfBounds();
        }
        for (int i=m_iterPos[vi]+1; i <= m_numVert; ++i)
        {
            if (m_a[vi][i] != m_NoEdge)
            {
                m_iterPos[vi] = i;
                return i;
            }
        }

        m_iterPos[vi] = m_numVert + 1;
        return 0;
    }


    template<class T>
    class MatrixDigraph : public MatrixWDigraph<T>
    {
    public:
        MatrixDigraph(int vert = 10) : MatrixWDigraph(vert, 0)
        {

        }

        MatrixDigraph<T>& addEdge(int vi, int vj)
        {
            MatrixWDigraph::addEdge(vi, vj, 1);
            return *this;
        }

        MatrixDigraph<T>& removeEdge(int vi, int vj)
        {
            MatrixWDigraph::removeEdge(vi, vj);
            return *this;
        }

    };


    // Linked Hierarchy
    template<class T>
    class LinkedBase: public Network
    {
    protected:
        Chain<T> *m_chains;
        int m_numVert;
        int m_numEdge;
        
    public:
        LinkedBase(int vert = 10)
        {
            m_numEdge = 0;
            m_numVert = vert;
            m_chains = new Chain<T> [m_numVert+1];
        }
        ~LinkedBase() { delete [] m_chains; }

        virtual int edges() const { return m_numEdge;}
        virtual int vertices() const { return m_numVert;}

        int outDegree(int vi) const
        {
            if (vi < 1 || vi > m_numVert)
            {
                throw OutOfBounds();
            }
            return m_chains[vi].length();
        }

        
    };

    

    template<class T>
    class LinkedDigraph : public LinkedBase<int>
    {

    public:
        LinkedDigraph(int vert = 10) : LinkedBase(vert) {}
        bool exist(int vi, int vj) const
        {
            if (vi < 1 || vi > m_numVert)
            {
                throw OutOfBounds();
            }
            return (m_chains[vi].search(vj)) ? true : false;
        }
        LinkedDigraph<T>& addEdge(int vi, int vj);
        LinkedDigraph<T>& removeEdge(int vi, int vj);

        int inDegree(int vi) const;

        virtual int begin(int vi);
        virtual int nextVertex(int vi);
        virtual void initialPos();
        virtual void deactivePos();
    protected:
        LinkedDigraph<T>& addEdgeNoCheck(int vi, int vj);

        ChainIterator<int> *m_pos;
    };

    template<class T>
    LinkedDigraph<T>& LinkedDigraph<T>::addEdgeNoCheck(int vi, int vj)
    {
        m_chains[vi].insert(0, vj);
        m_numEdge++;
        return *this;
    }

    template<class T>
    LinkedDigraph<T>& LinkedDigraph<T>::addEdge(int vi, int vj)
    {
        if (vi < 1|| vi > m_numVert|| vj <1|| vj > m_numVert|| vi == vj|| exist(vi, vj))
        {
            throw BadInput();
        }
        addEdgeNoCheck(vi, vj);
    }

    template<class T>
    LinkedDigraph<T>& LinkedDigraph<T>::removeEdge(int vi, int vj)
    {
        if (vi < 1|| vi > m_numVert)
        {
            throw OutOfBounds();
        }
        m_chains[vi].remove(vj);
        m_numEdge --;
        return this;
    }

    template<class T> 
    int  LinkedDigraph<T>::inDegree(int vi) const
    {
        if (vi < 1|| vi > m_numVert)
        {
            throw OutOfBounds();
        }

        int sum=0;
        for (int i=1; i<=m_numVert; ++i)
        {
            if (m_chains[i].search(vi))
            {
                sum ++;
            }
        }

        return sum;
    }

    template<class T>
    void LinkedDigraph<T>::initialPos()
    {
        m_pos = new ChainIterator<int> [m_numVert+1];
    }

    template<class T>
    void LinkedDigraph<T>::deactivePos()
    {
        delete [] m_pos;
    }

    template<class T>
    int LinkedDigraph<T>::begin(int vi)
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw OutOfBounds();
        }
        int *x = m_pos[vi].initialize(m_chains[vi]);
        return (x) ? *x : 0;
    }

    template<class T>
    int LinkedDigraph<T>::nextVertex(int vi)
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw OutOfBounds();
        }
        int *x = m_pos[vi].next();
        return (x) ? *x : 0;
    }


    template<class T>
    class LinkedGraph: public LinkedDigraph<T>, virtual public Undirected
    {
    public:
        LinkedGraph(int vert = 10) : LinkedDigraph(vert) {}
        LinkedGraph<T>& addEdge(int vi, int vj);
        LinkedGraph<T>& removeEdge(int vi, int vj);
        int degree(int vi) const { return inDegree(vi); }
        int outDegree(int vi) const { return inDegree(vi); }

        virtual int begin(int vi);
        virtual int nextVertex(int vi);
        virtual void initialPos();
        virtual void deactivePos();
        virtual int edges() const { return m_numEdge;}
        virtual int vertices() const { return m_numVert;}
    protected:
        LinkedGraph<T>& addEdgeNoCheck(int vi, int vj);

    };

    template<class T>
    LinkedGraph<T>& LinkedGraph<T>::addEdge(int vi, int vj)
    {
        if (vi < 1|| vi > m_numVert|| vj <1|| vj > m_numVert || vi == vj || exist(vi, vj))
        {
            throw BadInput();
        }
        addEdgeNoCheck(vi, vj);
        return *this;
    }

    template<class T>
    LinkedGraph<T>& LinkedGraph<T>::addEdgeNoCheck(int vi, int vj)
    {
        m_chains[vi].insert(0, vj);
        try
        {
            m_chains[vj].insert(0, vi);
        }
        catch (...)
        {
        	m_chains[vi].remove(vj);
            throw;
        }
        m_numEdge ++;
        return *this;
    }

    template<class T>
    LinkedGraph<T>& LinkedGraph<T>::removeEdge(int vi, int vj)
    {
        LinkedDigraph::removeEdge(vi, vj);
        m_numEdge++;
        LinkedDigraph::removeEdge(vj, vi);
        return *this;
    }


    template<class T>
    void LinkedGraph<T>::initialPos()
    {
        m_pos = new ChainIterator<int> [m_numVert+1];
    }

    template<class T>
    void LinkedGraph<T>::deactivePos()
    {
        delete [] m_pos;
    }

    template<class T>
    int LinkedGraph<T>::begin(int vi)
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw OutOfBounds();
        }
        int *x = m_pos[vi].initialize(m_chains[vi]);
        return (x) ? *x : 0;
    }

    template<class T>
    int LinkedGraph<T>::nextVertex(int vi)
    {
        if (vi < 1 || vi > m_numVert)
        {
            throw OutOfBounds();
        }
        int *x = m_pos[vi].next();
        return (x) ? *x : 0;
    }

    void graphUnitTest()
    {
        
        MatrixWGraph<int> md(4);
        md.addEdge(1, 2, 1);
        md.addEdge(1, 3, 1);
        md.addEdge(1, 4, 1);
        md.addEdge(2, 3, 1);
        md.addEdge(3, 4, 1);
        
        /*
        LinkedGraph<int> md(4);
        md.addEdge(1, 2);
        md.addEdge(1, 3);
        md.addEdge(1, 4);
        md.addEdge(2, 3);
        md.addEdge(3, 4);
        */
        std::cout<<"Vertices: "<<md.vertices()<<std::endl;
        std::cout<<"Edges: "<<md.edges()<<std::endl;
        for (int i=1; i <= md.vertices(); ++i)
        {
            std::cout<<"Vert "<<i<<" degree"<<md.degree(i)<<std::endl;
        }
        std::cout<<"BFS: ";
        int *reach = new int[md.vertices()+1];
        memset(reach, 0, sizeof(int)*(md.vertices()+1));
        md.DFS(1, reach, 99);

        delete [] reach;

    }

    void TopologicalSortTest()
    {
        int n_v = 6;
        MatrixDigraph<int> md(n_v);
        md.addEdge(1, 2);
        md.addEdge(1, 3);
        md.addEdge(1, 4);
        md.addEdge(3, 2);
        md.addEdge(3, 5);
        md.addEdge(4, 5);
        md.addEdge(6, 4);
        md.addEdge(6, 5);

        int *sortRes = new int [n_v];
        md.Topological(sortRes);
        
        std::cout<<"TopologicalSort:";
        for (int i=0; i<n_v; ++i)
        {
            std::cout<<sortRes[i]<<(i+1 != n_v ? " ":"\n");
        }

        delete [] sortRes;

    }

    void BipartieCoverTest()
    {
        int n_v = 17;
        LinkedGraph<int> lg(n_v);

        lg.addEdge(1, 4).addEdge(1, 6).addEdge(1, 7).addEdge(1, 8).addEdge(1, 9)
            .addEdge(1, 13);

        lg.addEdge(2, 4).addEdge(2, 5).addEdge(2, 6).addEdge(2, 8);

        lg.addEdge(3, 8).addEdge(3, 10).addEdge(3, 12).addEdge(3, 14).addEdge(3, 15);

        lg.addEdge(16, 5).addEdge(16, 6).addEdge(16, 8).addEdge(16, 12).addEdge(16, 14)
            .addEdge(16, 15);

        lg.addEdge(17, 4).addEdge(17, 9).addEdge(17, 10).addEdge(17, 11);

        int L[18] = {0, 1, 1, 1, 0,0,0,0,0,0,0,0,0,0,0,0, 1, 1};
        int C[18] = {0};

        int m;
        lg.BipartiteCover(L, C, m);
        std::cout<<"BipartiteCover: ";
        for (int i=0; i<m; ++i)
        {
            std::cout<<C[i]<<(i == m-1 ? "\n" : " ");
        }
    }


    void shortedPathTest()
    {
        const int VERLEN = 5;

        MatrixWDigraph<int> md(VERLEN, 99);

        md.addEdge(1, 2, 4);
        md.addEdge(1, 3, 2);
        md.addEdge(1, 5, 8);
        md.addEdge(2, 4, 4);
        md.addEdge(2, 5, 5);
        md.addEdge(3, 4, 1);
        md.addEdge(4, 5, 3);

        int pre[VERLEN+1] = {0};
        int dest[VERLEN+1] = {0};

        int startPos = 1;
        md.shortestPath(startPos, dest, pre);

        int path[10] = {0};
        std::cout<<"StartPos: "<<startPos<<std::endl;
        for (int i=1; i<=VERLEN; ++i)
        {
            
            if (i != startPos)
            {
                int curP = pre[i];
                int pCur = 0;
                path[pCur++] = i;
                while (curP != 0)
                {
                    path[pCur++] = curP;
                    curP  = pre[curP];
                }
                std::cout<<"Destination "<<i<<" Length: "<<dest[i]<<", Path: ";
                for (int j=pCur-1; j >= 0; j--)
                {
                    std::cout<<path[j]<<(j != 0 ? "->" : "\n");
                }
            }
        }

    }

    template<class T>
    void OutputPath(T **c, int **kay, T NoEdge, int i, int j)
    {
        if (c[i][j] == NoEdge)
        {
            std::cout<<"No path from "<<i<<" to"<<j<<std::endl;
            return;
        }
        else
        {
            std::cout<<"The path is: "<<i;
            outputPath(kay, i, j);
            std::cout<<std::endl;
        }
    }

    void  outputPath(int **kay, int i, int j)
    {
        if (i == j)
        {
            return;
        }
        else if (kay[i][j] == 0) // @ destination
        {
            std::cout<<"->"<<j;
        }
        else
        {
            outputPath(kay, i, kay[i][j]);
            outputPath(kay, kay[i][j], j);
        }
    }

    void allPairsShortestPairTest()
    {
        const int VERLEN = 10;

        MatrixWDigraph<int> md(VERLEN, 99);

        md.addEdge(1, 2, 2).addEdge(1, 4, 20);
        md.addEdge(2, 5, 1);
        md.addEdge(3, 1, 3);
        md.addEdge(4, 3, 8).addEdge(4, 6, 6).addEdge(4, 7, 4);
        md.addEdge(5, 3, 7).addEdge(5, 8, 3);
        md.addEdge(6, 3, 1);
        md.addEdge(7, 8, 1);
        md.addEdge(8, 6, 2).addEdge(8, 10, 2);
        md.addEdge(9, 7, 2);
        md.addEdge(10, 9, 1);

        int **c;
        int **kay;
        Yincpp::make2DArray(c, VERLEN+1, VERLEN+1);
        Yincpp::make2DArray(kay, VERLEN+1, VERLEN+1);
        md.allPairsShortestPath(c, kay);
        int startI = 1, endI = 10;
        OutputPath(c, kay, 99, startI, endI);
        std::cout<<"Length:"<<c[startI][endI]<<std::endl;
        Yincpp::delete2DArray(c, VERLEN+1);
        Yincpp::delete2DArray(kay, VERLEN+1);
    }


    void minSpanningTreeTest()
    {
        MatrixWGraph<int> mg(7);
        mg.addEdge(1, 6, 10).addEdge(1, 2, 28);
        mg.addEdge(2, 7, 14).addEdge(2, 3, 16);
        mg.addEdge(3, 4, 12);
        mg.addEdge(4, 7, 18);
        mg.addEdge(5, 4, 22).addEdge(5, 6, 24).addEdge(5, 7, 25);

        EdgeNode<int> *ent = new EdgeNode<int>[mg.edges()];
        if(mg.Kruskal(ent))
        {
            std::cout<<"Find the min Spanning tree:"<<std::endl;
            for (int i=0; i<mg.vertices()-1; ++i)
            {
                std::cout<<"("<<ent[i].m_u<<", "<<ent[i].m_v<<")";
                std::cout<<(i == mg.vertices()-2 ? "\n":", ");
            }
        }
        else
        {
            std::cout<<"No Spanning tree!"<<std::endl;
        }
        delete []  ent;

    }
}

