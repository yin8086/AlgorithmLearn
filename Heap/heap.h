// MaxHeap
template<class T>
class MaxHeap
{
private:
    int m_size, m_maxSize;
    T *m_heap;

public:
    MaxHeap(int maxSize):m_maxSize(maxSize),m_size(0)
    {
        m_heap = new T[m_maxSize+1]; // start from 1
    }
    ~MaxHeap()
    {
        delete [] m_heap;
    }

    int size() const {return m_size;}
    T max() 
    {
        if (m_size == 0)
        {
            throw OutOfBounds();
        }
        return m_heap[1];
    }
    MaxHeap<T>& insert(const T& rhs);
    MaxHeap<T>& deleteMax(T& rhs);
    void initialize(T a[], int size, int arraySize);
    void deactive() {m_heap = 0;}

};

template<class T>
MaxHeap<T>& MaxHeap<T>::insert(const T& rhs)
{
    if (m_size == m_maxSize)
    {
        throw NoMem();
    }

    int tarPos = ++m_size; // first idx is 1 and size = 0
    while(tarPos != 1 && m_heap[tarPos/2] < rhs) // ¡ýuntil idx =1 with no parents
    {
        m_heap[tarPos] = m_heap[tarPos/2];
        tarPos /= 2;
    }
    m_heap[tarPos] = rhs;
    return *this;
}


template<class T>
MaxHeap<T>& MaxHeap<T>::deleteMax(T& rhs)
{
    if (m_size == 0)
    {
        throw OutOfBounds();
    }

    rhs = m_heap[1];
    T lastNode = m_heap[m_size--];

    int tarPos = 1;
    int childPos = tarPos<<1;
    while (childPos <= m_size) // m_heap[m_size] is the last node
    {
        if (childPos+1 <= m_size && m_heap[childPos] < m_heap[childPos + 1 ])
        {
            // select max child
            childPos++;
        }
        if (lastNode < m_heap[childPos])
        {
            m_heap[tarPos] = m_heap[childPos]; // ¡ü
        }
        else
            break;
        tarPos = childPos;
        childPos = tarPos<<1;
    }
    m_heap[tarPos] = lastNode;

    return *this;
}

template<class T>
void MaxHeap<T>::initialize(T a[], int size, int arraySize)
{
    delete [] m_heap;
    m_heap = a;
    m_size = size;
    m_maxSize = arraySize;

    for(int i=m_size/2; i>=1; i--)
    {
        
        T tmpV = m_heap[i];

        int tarPos = i;
        int childPos = tarPos * 2;

        while (childPos <= m_size )
        {
            if (childPos+1 <= m_size && m_heap[childPos] < m_heap[childPos+1])
            {
                // select max child
                childPos++;
            }

            if (tmpV < m_heap[childPos])
            {
                m_heap[tarPos] = m_heap[childPos];
            }
            else break;

            tarPos = childPos;
            childPos = tarPos*2;
        }

        m_heap[tarPos] = tmpV;
    }

}


// MinHeap
template<class T>
class MinHeap
{
private:
    int m_size, m_maxSize;
    T *m_heap;

public:
    MinHeap(int maxSize):m_maxSize(maxSize),m_size(0)
    {
        m_heap = new T[m_maxSize+1]; // start from 1
    }
    ~MinHeap()
    {
        delete [] m_heap;
    }

    int size() const {return m_size;}
    T min() 
    {
        if (m_size == 0)
        {
            throw OutOfBounds();
        }
        return m_heap[1];
    }
    MinHeap<T>& insert(const T& rhs);
    MinHeap<T>& deleteMin(T& rhs);
    void initialize(T a[], int size, int arraySize);
    void deactive() {m_heap = 0;}

};

template<class T>
MinHeap<T>& MinHeap<T>::insert(const T& rhs)
{
    if (m_size == m_maxSize)
    {
        throw NoMem();
    }

    int tarPos = ++m_size; // first idx is 1 and size = 0
    while(tarPos != 1 && m_heap[tarPos/2] > rhs) // ¡ýuntil idx =1 with no parents
    {
        m_heap[tarPos] = std::move(m_heap[tarPos/2]);
        tarPos /= 2;
    }
    m_heap[tarPos] = std::move(rhs);
    return *this;
}


template<class T>
MinHeap<T>& MinHeap<T>::deleteMin(T& rhs)
{
    if (m_size == 0)
    {
        throw OutOfBounds();
    }

    rhs = std::move(m_heap[1]);
    T lastNode = std::move(m_heap[m_size--]);

    int tarPos = 1;
    int childPos = tarPos<<1;
    while (childPos <= m_size) // m_heap[m_size] is the last node
    {
        if (childPos+1 <= m_size && m_heap[childPos] > m_heap[childPos + 1 ])
        {
            // select min child
            childPos++;
        }
        if (lastNode > m_heap[childPos])
        {
            m_heap[tarPos] = std::move(m_heap[childPos]); // ¡ü
        }
        else
            break;
        tarPos = childPos;
        childPos = tarPos<<1;
    }
    m_heap[tarPos] = std::move(lastNode);

    return *this;
}

template<class T>
void MinHeap<T>::initialize(T a[], int size, int arraySize)
{
    delete [] m_heap;
    m_heap = a;
    m_size = size;
    m_maxSize = arraySize;

    for(int i=m_size/2; i>=1; i--)
    {

        T tmpV = std::move(m_heap[i]); 

        int tarPos = i;
        int childPos = tarPos * 2;

        while (childPos <= m_size )
        {
            if (childPos+1 <= m_size && m_heap[childPos] > m_heap[childPos+1])
            {
                childPos++;
            }

            if (tmpV > m_heap[childPos])
            {
                m_heap[tarPos] = std::move(m_heap[childPos]);
            }
            else break;

            tarPos = childPos;
            childPos = tarPos*2;
        }

        m_heap[tarPos] = std::move(tmpV);
    }

}


//@param a[1...n]
template<class T>
void HeapSort(T a[], int n)
{
    MaxHeap<T> mh(1);
    mh.initialize(a, n, n); // deep copy

    T x;
    for (int i=n; i > 1; i--) // n...2
    {
        mh.deleteMax(x);
        a[i] = x;
    }

    mh.deactive();
}

void heapUnitTest()
{
    int numE = 100;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, numE * 4);

    int *testArray = new int[numE+1];
    std::cout<<"====Unsorted====="<<std::endl;

    for (int i=1; i<=numE; i++)
    {
        testArray[i] = uidn(newEngine);
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    HeapSort(testArray, numE);

    std::cout<<"====sorted====="<<std::endl;
    for (int i=1; i<=numE; i++)
    {
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    delete [] testArray;
}