

template<class E, class K>
class SortedChain;

template<class E, class K>
class SortedChainNode
{
    friend std::ostream& operator<<(std::ostream& os, const SortedChain<E,K>& rhs);
    friend class SortedChain<E, K>;
private:
    E m_data;
    SortedChainNode<E, K> *m_next;

};

template<class E, class K>
class SortedChain
{
private:
    SortedChainNode<E, K> *m_head;
    int m_length;
    
public:
    SortedChain():m_length(0)
    {
        m_head = new SortedChainNode<E, K>;
        m_head->m_next = NULL;
    }
    ~SortedChain();
    bool isEmpty() const { return m_head->m_next == NULL;}
    int length() const { return m_length; }
    SortedChain<E, K>& insert(const E& eValue);
    bool search(const K& key, E& eValue) const;
    SortedChain<E, K>& remove(const K& key, E& eValue);
    SortedChain<E, K>& distinctInsert(const E& eValue);
    void clear();

    friend std::ostream& operator<<(std::ostream& os, const SortedChain<E,K>& rhs)
    {
        SortedChainNode<E, K> *p = rhs.m_head;
        os << "{ ";
        while(p->m_next != NULL)
        {
            os << p->m_next->m_data;
            if (p->m_next->m_next)
            {
                os << ", ";
            }
            p = p->m_next;
        }
        os << " }" << std::endl;
        return os;
    }
    void unitTest();
};

template<class E, class K>
SortedChain<E, K>::~SortedChain()
{
    SortedChainNode<E, K> *p;
    while(m_head->m_next != NULL)
    {
        p = m_head->m_next;
        m_head->m_next = p->m_next;
        delete p;
    }
    delete m_head;
}

template<class E, class K>
void SortedChain<E, K>::clear()
{
    // clear all the elements
    SortedChainNode<E, K> *p;
    while(m_head->m_next != NULL)
    {
        p = m_head->m_next;
        m_head->m_next = p->m_next;
        delete p;
    }
}

template<class E, class K>
SortedChain<E, K>& SortedChain<E, K>::insert(const E& eValue)
{
    K key = eValue;
    SortedChainNode<E, K> *cur = m_head;
    while(cur->m_next && cur->m_next->m_data < key)
        cur = cur->m_next;

    SortedChainNode<E, K> *pNew = new SortedChainNode<E, K>;
    pNew->m_data = eValue;
    pNew->m_next = cur->m_next;

    cur->m_next = pNew;
    return *this;
}

template<class E, class K>
SortedChain<E, K>& SortedChain<E, K>::distinctInsert(const E& eValue)
{
    K key = eValue;
    SortedChainNode<E, K> *cur = m_head;
    while(cur->m_next && cur->m_next->m_data < key)
        cur = cur->m_next;

    if(cur->m_next && cur->m_next->m_data == key)
        throw BadInput();

    SortedChainNode<E, K> *pNew = new SortedChainNode<E, K>;
    pNew->m_data = eValue;
    pNew->m_next = cur->m_next;

    cur->m_next = pNew;
    return *this;
}

template<class E, class K>
bool SortedChain<E,K>::search(const K& key, E& eValue) const
{
    SortedChainNode<E, K> *cur = m_head;
    while(cur->m_next && cur->m_next->m_data < key)
        cur = cur->m_next;

    if(cur->m_next && cur->m_next->m_data == key)
    {
        eValue = cur->m_next->m_data;
        return true;
    }
    return false;
}

template<class E, class K>
SortedChain<E,K>& SortedChain<E,K>::remove(const K& key, E& eValue)
{
    SortedChainNode<E, K> *cur = m_head;
    while(cur->m_next && cur->m_next->m_data < key)
        cur = cur->m_next;

    if(cur->m_next && cur->m_next->m_data == key)
    {
        eValue = cur->m_next->m_data;
        SortedChainNode<E, K> *p = cur->m_next;
        cur->m_next = p->m_next;
        delete p;
        return *this;
    }
    throw BadInput();
}

template<class E, class K>
void SortedChain<E,K>::unitTest()
{
    std::default_random_engine engine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uid(0, 500);

    for (int i=0; i<20; ++i)
    {
        distinctInsert(E(i, uid(engine)));
    }
    std::cout<<*this;
    E tmpE;
    remove(3, tmpE);
    remove(7, tmpE);
    remove(11, tmpE);
    std::cout<<*this;
    if(search(15, tmpE))
    {
        std::cout<<"search key "<<15<<", Value="<<tmpE<<std::endl;
    }
}