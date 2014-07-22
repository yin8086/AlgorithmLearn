template<class T>
class Chain;

template<class T>
class ChainNode
{
    friend Chain<T>;
private:
    T m_data;
    ChainNode<T> *m_next;

};

template<class T>
class Chain
{
private:
    ChainNode<T> *m_head;
    int m_length;
    bool binSort(int range, std::function< int(int) > func);
    
public:
    Chain():m_length(0)
    {
        m_head = new ChainNode<T>;
        m_head->m_next = NULL;
    }
    ~Chain();
    bool isEmpty() const { return m_head->m_next == NULL;}
    int length() const { return m_length; }
    Chain<T>& insert(int k, const T& rhs);
    Chain<T>& remove(int k, T& rhs);
    Chain<T>& insertSort();
    Chain<T>& radixSort(int radix = 10);
    void output(std::ostream& out) const;
};

template<class T>
Chain<T>& Chain<T>::insert(int k, const T& rhs)
{
    ChainNode<T> *cur = m_head;
    while(cur->m_next && k-1 >= 0)
    {
        cur = cur->m_next;
        --k;
    }
    
    if(k == 0)
    {
        ChainNode<T> *newCN = new ChainNode<T>;
        newCN->m_data = rhs;
        newCN->m_next = cur->m_next;
        cur->m_next = newCN;
        ++m_length;
    }
    return *this;
}

template<class T>
Chain<T>& Chain<T>::remove(int k, T& rhs)
{
    if(isEmpty())
    {
        return *this;
    }

    ChainNode<T> *cur = m_head;
    while(cur->m_next && k-1 >= 0)
    {
        cur = cur->m_next;
        --k;
    }

    if(k == 0)
    {
        ChainNode<T> *tarCN = cur->m_next;
        if(!tarCN)
        {
            // no kth element
            return *this;
        }
        else
        {
            rhs = tarCN->m_data;
            cur->m_next = tarCN->m_next;
            delete tarCN;
            --m_length;
        }
    }
    return *this;
}


template<class T>
Chain<T>::~Chain()
{
    while(m_head->m_next != NULL)
    {
        T tmpData;
        remove(0, tmpData);
    }
    delete m_head;
}

template<class T>
void Chain<T>::output(std::ostream& out) const
{
    ChainNode<T> *cur = m_head;

    while(cur->m_next != NULL)
    {
        cur = cur->m_next;
        out<<cur->m_data<<" ";
    }
}

template<class T>
Chain<T>& Chain<T>::insertSort()
{

    // 核心问题的关键是，每轮循环定位到正确的Node。每次插入后都需要
    // 重新寻找末尾位置的Node
    if(m_length > 1)
    {
        
        for(int i=1; i < m_length; ++i)
        {

            // move to the node before
            ChainNode<T> *cur = m_head;
            for(int k=0; k<i; ++k)
                cur = cur->m_next;
        
            // insert
            T curVal = cur->m_next->m_data;
            ChainNode<T> *pInsert = m_head;
            while(pInsert != cur && pInsert->m_next->m_data <= curVal )
                pInsert = pInsert->m_next;
            if (pInsert != cur)
            {
                ChainNode<T> *tmp = cur->m_next;
                cur->m_next = tmp->m_next;
                tmp->m_next = pInsert->m_next;
                pInsert->m_next = tmp;
            }
        }
            
        
    }

    return *this;
}


/// use lambda to decompose data
template<class T>
bool Chain<T>::binSort(int range, std::function< int(int) > func)
{
    if(m_length == 0)
        return true;

    bool allZero = true;

    ChainNode<T> **bottom = new ChainNode<T>*[range + 1];
    ChainNode<T> **top = new ChainNode<T>*[range + 1];
    for(int i=0; i<=range; ++i)
        bottom[i] = 0;

    // distribute the elements
    for(ChainNode<T> *cur = m_head->m_next; cur ; cur = cur->m_next)
    {
        int tarIdx = func(cur->m_data);

        if(allZero && tarIdx != 0)
            allZero = false;

        if(!bottom[tarIdx])
        {
            bottom[tarIdx] = top[tarIdx] = cur;
        }
        else
        {
            top[tarIdx]->m_next = cur;
            top[tarIdx] = cur;
        }
    }

    // re-gather the elements in the bin
    ChainNode<T> *cur = m_head;
    for(int i=0; i<=range; ++i)
    {
        if(bottom[i])
        {
            cur->m_next = bottom[i];
            cur = top[i];
        }
    }    
    if(cur) cur->m_next = NULL; // important, set the last node link NULL

    delete [] bottom;
    delete [] top;

    return allZero;
}

template<class T>
Chain<T>& Chain<T>::radixSort(int radix)
{

    // [x/(radix^i)] % radix
    int den = 1;
    bool allZero;
    do 
    {
        allZero = binSort(radix, 
            [den, radix] (int rhs) -> int { return (rhs/den)%radix;});
        den *= radix;
    } while (!allZero);
    return *this;
}