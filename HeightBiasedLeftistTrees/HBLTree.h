#include <algorithm>
#include <queue>

template<class T>
class MaxHBLT;

template<class T>
class HBLTNode
{
    friend class MaxHBLT<T>;

private:
    T m_data;
    HBLTNode<T> *m_left, *m_right;
    int m_S;

public:
    HBLTNode(const T& e,  int s):m_data(e),m_S(s),m_left(0),m_right(0){}
};


template<class T>
class MaxHBLT
{
private:
    HBLTNode<T> *m_root;
    void clear(HBLTNode<T> *t);
    void meld(HBLTNode<T>* &lhs, HBLTNode<T>* &rhs); // need to change the pointer

public:
    MaxHBLT():m_root(0) {}
    ~MaxHBLT() { clear(m_root); m_root = 0;}
    T max()
    {
        if(!m_root)
        {
            throw OutOfBounds();
        }
        return m_root->m_data;
    }

    MaxHBLT<T>& insert(const T& rhs);
    MaxHBLT<T>& deleteMax(T& rhs);
    MaxHBLT<T>& meld(MaxHBLT<T>& rhs) // make two HBLT into one
    {
        meld(m_root, rhs.m_root);
        rhs.m_root = 0; // move resource
        return *this;
    }
    void initialize(T a[], int n);


};

template<class T>
void MaxHBLT<T>::clear(HBLTNode<T> *t)
{
    if (t)
    {
        clear(t->m_left);
        clear(t->m_right);
        delete t;
    }
}

template<class T>
void MaxHBLT<T>::meld(HBLTNode<T>* &lhs, HBLTNode<T>* &rhs)
{
    // merge to lhs
    if(!rhs) return;
    if(!lhs)
    {
        lhs = rhs;
        return;
    }

    if (rhs->m_data > lhs->m_data)
    {
        std::swap(lhs, rhs);
    }

    meld(lhs->m_right, rhs);
    // here lhs->m_right is not empty
    if (lhs->m_left) // not empty
    {
        if (lhs->m_left->m_S < lhs->m_right->m_S)
        {
            std::swap(lhs->m_left, lhs->m_right);
        }
        lhs->m_S = lhs->m_right->m_S + 1;
    }
    else
    {
        lhs->m_left = lhs->m_right;
        lhs->m_right = 0;
        lhs->m_S = 1;
    }

}


template<class T>
MaxHBLT<T>& MaxHBLT<T>::insert(const T& rhs)
{
    HBLTNode<T> *hnode = new HBLTNode<T>(rhs, 1);
    meld(m_root, hnode);
    return *this;
}


template<class T>
MaxHBLT<T>& MaxHBLT<T>::deleteMax(T& rhs)
{
    if (!m_root)
    {
        throw OutOfBounds();
    }

    rhs = m_root->m_data;
    HBLTNode<T> *lp = m_root->m_left;
    HBLTNode<T> *rp = m_root->m_right;

    delete m_root;
    m_root = lp;
    meld(m_root, rp);
    return *this;
}


// @param a[1...n]
template<class T>
void MaxHBLT<T>::initialize(T a[], int n)
{
    clear(m_root);

    std::queue<HBLTNode<T> *> nodeQueue;
    for (int i=1; i<=n; ++i)
    {
        HBLTNode<T> *pn = new HBLTNode<T>(a[i], 1);
        nodeQueue.push(pn);
    }

    HBLTNode<T> *p1=0, *p2=0;
    while (!nodeQueue.empty())
    {
        p1 = nodeQueue.front();
        nodeQueue.pop();
        if (nodeQueue.empty())
        {
            break;
        }
        p2 = nodeQueue.front();
        nodeQueue.pop();
        meld(p1, p2);
        nodeQueue.push(p1);
    }

    m_root = p1;
}


//@param a[1...n]
template<class T>
void HeapSort(T a[], int n)
{
    MaxHBLT<T> mt;
    mt.initialize(a, n); // deep copy

    T x;
    for (int i=n; i >= 1; i--) // n...2
    {
        mt.deleteMax(x);
        a[i] = x;
    }

    
}