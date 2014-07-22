
struct element
{
    int m_data;
    long m_key;
public:
    operator long() const { return m_key;}
    element& operator=(long rhs) {m_key=rhs; return *this;}
    friend std::ostream& operator<<(std::ostream& os, const element& rhs)
    {
        os<<rhs.m_data;
        return os;
    }
};

template<class E, class K>
class SkipList;


// class E is a class that compromise of Key and Data
template<class E, class K>
class SkipNode
{
    friend class SkipList<E, K>;
private:
    SkipNode<E,K> **m_link; // N link field
    E m_data;

    SkipNode(int size)
    {
        m_link = new SkipNode<E,K> *[size];
    }
    ~SkipNode()
    {
        delete [] m_link;
    }
};

template<class E, class K>
class SkipList {
public:
    SkipList(K Large, int MaxE = 10000, float p = 0.5);
    ~SkipList();
    bool Search(const K& key, E& eValue) const;
    SkipList<E, K>& Insert(const E& eValue);
    SkipList<E, K>& Delete(const K& key, E& eValue);

private:
    int Level();
    SkipNode<E, K> *SaveSearch(const K& key); //save last position
    
    int m_MaxLevel;
    int m_Levels;
    int m_cutOffs;
    K m_TailKey;
    SkipNode<E, K> *m_head;
    SkipNode<E, K> *m_tail;
    SkipNode<E, K> **m_last;
    std::default_random_engine m_engine;
};

template<class E, class K>
SkipList<E, K>::SkipList(K Large, int MaxE /* = 10000 */, float p /* = 0.5 */)
{
    m_cutOffs = static_cast<int>(p * std::numeric_limits<int>::max()); //random Max
    m_MaxLevel = static_cast<int>(ceil(log(MaxE)/log(1/p)) - 1);

    m_TailKey = Large;
    m_engine.seed(static_cast<int>(std::time(0)));

    m_Levels = 0;

    m_head = new SkipNode<E, K> (m_MaxLevel + 1);
    m_tail = new SkipNode<E, K> (0);
    m_last = new SkipNode<E, K> * [m_MaxLevel + 1];
    m_tail->m_data = Large;

    for(int i=0; i<= m_MaxLevel; ++i)
        m_head->m_link[i] = m_tail;

}

template<class E, class K>
SkipList<E, K>::~SkipList()
{
    SkipNode<E, K> *curP;

    while(m_head != m_tail)
    {
        curP = m_head;
        m_head = m_head->m_link[0];
        delete curP;
    }

    delete m_tail;
    delete [] m_last;
}

template<class E, class K>
bool SkipList<E, K>::Search(const K& key, E& eValue) const
{
    if(key >= m_TailKey) return false;

    SkipNode<E, K> *p = m_head;
    for(int i=m_Levels; i >= 0; i--)
    {
        // iterate all the level
        while(p->m_link[i]->m_data < key)
        {
            p = p->m_link[i];
        }
        // next of p points to the first element >= key
    }

    eValue = p->m_link[0]->m_data;
    return (eValue == key);
}

template<class E, class K>
SkipNode<E, K>* SkipList<E, K>::SaveSearch(const K& key)
{
    SkipNode<E, K> *p = m_head;
    for(int i=m_Levels; i >= 0; i--)
    {
        // iterate all the level
        while(p->m_link[i]->m_data < key)
        {
            p = p->m_link[i];
        }
        m_last[i] = p; // last[i] points to the i level last Node
        // next of p points to the first element >= key
    }
    return (p->m_link[0]);
}

template<class E, class K>
int SkipList<E, K>::Level()
{
    int level = 0;
    std::uniform_int_distribution<int> uidg;
    while(uidg(m_engine) <= m_cutOffs )
        level++;
    return level >= m_MaxLevel ? m_MaxLevel : level;
}


template<class E, class K>
SkipList<E, K>& SkipList<E, K>::Insert(const E& eValue)
{
    K k = eValue;
    if(k >= m_TailKey) throw BadInput();

    SkipNode<E, K> *p = SaveSearch(k);
    if(p->m_data == eValue)  throw BadInput();

    int lev = Level();
    if(lev > m_Levels)
    {
        lev = ++m_Levels;
        m_last[lev] = m_head;
    }

    SkipNode<E, K> *y = new SkipNode<E, K>(lev + 1);
    y->m_data = eValue;
    for(int i=0; i<=lev; ++i) // set all levels pointer
    {
        y->m_link[i] = m_last[i]->m_link[i];
        m_last[i]->m_link[i] = y;
    }


    return *this;
}

template<class E, class K>
SkipList<E, K>& SkipList<E, K>::Delete(const K& key, E& eValue)
{
    if(key >= m_TailKey) throw BadInput();

    SkipNode<E, K> *p = SaveSearch(key);
    if(p->m_data == key)  throw BadInput();

    for(int i=0; i<=m_Levels && m_last[i]->m_link[i] == p; ++i)
    {
        m_last[i]->m_link[i] = p->m_link[i];
    }

    while(m_Levels > 0 && m_head->m_link[m_Levels] == m_tail)
        m_Levels--;

    eValue = p->m_data;
    delete p;
    return *this;
}