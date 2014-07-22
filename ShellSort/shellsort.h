
template<class T>
class SortTest
{
private:
    int m_num;
    T *m_pData;
    std::default_random_engine m_engine;
public:
    SortTest<T>(int n):m_num(n) 
    {  
        m_pData = new T[m_num]; 
        m_engine.seed(static_cast<int>(std::time(0)));
    }
    ~SortTest<T>()
    {
        delete [] m_pData;
    }

    void randomFill()
    {

        // basic check
        if (!m_pData)
            return;
        if (m_num<0)
            return;

        std::uniform_real_distribution<double> gr(0, m_num);


        for(int i=0; i<m_num; ++i)
        {
            m_pData[i] = gr(m_engine);
        }
    }
    void printOut()
    {
        for(int i=0; i<m_num; ++i)
        {
            std::cout<<m_pData[i];

            if( (i+1)%5 != 0)
                std::cout<<" ";
            else
                std::cout<<std::endl;
        }
    }
    void shellSort()
    {
        // 核心问题是末尾位置元素的值必须先提取出来。因为后来会被修改

        // basic check
        if (!m_pData)
            return;
        if (m_num<0)
            return;

        // set start distance
        int maxD = 1;
        while(maxD < m_num/3)
            maxD = maxD*3 + 1;

        for(int dis = maxD; dis > 0; dis = (dis-1)/3 )
        {
            // iterate all the distance, until 1
            for(int subIdx = 0; subIdx < dis; ++subIdx)
            {
                // iterate max [dis] sub array
                for(int i = subIdx + dis; i < m_num; i += dis)
                {
                    // iterate all the element of the sub-array to sort
                    T curVal = m_pData[i]; // !!!!!!!must store the value 
                    int j = i-dis;
                    for(; j >= subIdx && m_pData[j] > curVal; j-=dis)
                    {
                        m_pData[j+dis] = m_pData[j];
                    }
                    m_pData[j+dis] = curVal;
                }


#ifdef DEBUG_MY
    for(int myIdx = 0; myIdx <m_num; myIdx ++)
    {
        std::cout<<m_pData[myIdx]<<" ";
    }
    std::cout<<std::endl;
#endif // DEBUG_MY

            }

        }
    }
};



