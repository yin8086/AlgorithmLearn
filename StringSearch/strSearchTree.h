namespace Yincpp
{
    const int R = 256;

    template<class T>
    class TrieST;

    template<class T>
    class StrSearchNode
    {
        friend class TrieST<T>;
    private:
        T m_val;
        bool isVal;
        StrSearchNode<T> *m_next[R];
    public:
        StrSearchNode():m_val(T()), isVal(false)
        {
            for (int i=0; i<R; ++i)
            {
                m_next[i] = nullptr;
            }
        }
    };

    template<class T>
    class TrieST
    {
    private:
        StrSearchNode<T> *m_root;
        StrSearchNode<T>* get(StrSearchNode<T>* x, const std::string &key, int d);
        StrSearchNode<T>* put(StrSearchNode<T>* x, const std::string &key, T val, int d);
        StrSearchNode<T>* remove(StrSearchNode<T>* x, const std::string &key, int d);
        void clear(StrSearchNode<T>* x);
        void collect(StrSearchNode<T> *x, const std::string &pre, std::queue<std::string> &sQ);
    public:
        TrieST():m_root(nullptr) {}
        ~TrieST()
        {
            clear(m_root);
        }
        T get(const std::string &key)
        {
            StrSearchNode<T> *x = get(m_root, key, 0);
            if (!x)
            {
                return NULL;
            }
            else
            {
                if (x->isVal)
                {
                    return x->m_val;
                }
                else
                {
                    throw OutOfBounds();
                }
            }
            
        }

        void put(const std::string &key, T val)
        {
            m_root = put(m_root, key, val, 0);
        }

        void remove(const std::string &key)
        {
            m_root = remove(m_root, key, 0);
        }

        void traverseWithPrefix(const std::string &key);
    };

    template<class T>
    void TrieST<T>::clear(StrSearchNode<T>* x)
    {
        if (x)
        {
            for (int i  = 0; i < R; i ++)
            {
                clear(x->m_next[i]);
            }

            delete x;
        }
        
    }

    template<class T>
    StrSearchNode<T>* TrieST<T>::get(StrSearchNode<T>* x, const std::string &key, int d)
    {
        if (!x)
        {
            return nullptr;
        }

        if (d == key.size())
        {
            return x;
        }

        return get(x->m_next[key[d]], key, d+1);
    }


    template<class T>
    StrSearchNode<T>* TrieST<T>::put(StrSearchNode<T>* x, const std::string &key, T val, int d)
    {
        if (!x)
        {
            x = new StrSearchNode<T>;
        }
        if (d == key.size())
        {
            x->m_val = val;
            x->isVal = true;
            return x;
        }
        x->m_next[key[d]] = put(x->m_next[key[d]], key, val, d+1);
        return x;
    }

    template<class T>
    StrSearchNode<T>* TrieST<T>::remove(StrSearchNode<T>* x, const std::string &key, int d)
    {
        if (!x)
        {
            return x;
        }
        if (d == key.size())
        {
            x->isVal = false;
        }
        else
        {
            x->m_next[key[d]] = remove(x->m_next[key[d]], key, d+1);
        }

        if (x->isVal)
        {
            return x;
        }
        else
        {
            for (int c=0; c<256; ++c)
            {
                // not all empty return x unchanged
                if (x->m_next[c] != nullptr)
                {
                    return x;
                }
            }
            // all empty return nullptr
            return nullptr;
        }
    }

    template<class T>
    void TrieST<T>::collect(StrSearchNode<T> *x, const std::string &pre, std::queue<std::string> &sQ)
    {
        if (!x)
        {
            return;
        }

        if (x->isVal)
        {
            sQ.push(pre);
        }
        for (int c=0; c<R; ++c)
        {
            collect(x->m_next[c], pre+char(c), sQ);
        }
    }

    template<class T>
    void TrieST<T>::traverseWithPrefix(const std::string &pre)
    {
        std::queue<std::string> sQ;
        collect(get(m_root, pre, 0), pre, sQ);
        while (!sQ.empty())
        {
            std::cout<<sQ.front()<<" ";
            sQ.pop();
        }
        std::cout<<std::endl;
    }
}