

namespace Yincpp
{
    class Network
    {
    protected:
        int m_numVert;
        int m_numEdge;

    public:
        virtual int begin(int vi) = 0;
        virtual int nextVertex(int vi) = 0;
        virtual void initialPos() = 0;
        virtual void deactivePos() = 0;

        int edges() const { return m_numEdge;}
        int vertices() const { return m_numVert;}

        virtual void BFS(int vi, int reach[], int label);
        virtual void DFS(int vi, int reach[], int label);
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

    template<class T>
    void make2DArray(T ** &x, int rows, int cols)
    {
        x = new T* [rows];
        for (int i=0; i<rows; ++i)
        {
            x[i] = new T [cols];
        }
    }

    template<class T>
    void delete2DArray(T ** & x, int rows)
    {
        for (int i=0; i<rows; ++i)
        {
            delete [] x[i];
        }

        delete [] x;
        x = 0;
    }

    // Matrix hierarchy
    template<class T>
    class MatrixWDigraph : virtual public Network
    {
    protected:
        T m_NoEdge;

        T **m_a;

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

        MatrixWDigraph<T>& addEdge(int vi, int vj, const T& weight);
        MatrixWDigraph<T>& removeEdge(int vi, int vj);
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
    class MatrixWGraph : public MatrixWDigraph<T>
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
    };


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

        
    public:
        LinkedBase(int vert = 10)
        {
            m_numEdge = 0;
            m_numVert = vert;
            m_chains = new Chain<T> [m_numVert+1];
        }
        ~LinkedBase() { delete [] m_chains; }
        int edges() const { return m_numEdge; }
        int vertices() const { return m_numVert; }
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
    class LinkedGraph: public LinkedDigraph<T>
    {
    public:
        LinkedGraph(int vert = 10) : LinkedDigraph(vert) {}
        LinkedGraph<T>& addEdge(int vi, int vj);
        LinkedGraph<T>& removeEdge(int vi, int vj);
        int degree(int vi) const { return inDegree(vi); }
        int outDegree(int vi) const { return inDegree(vi); }
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


}

