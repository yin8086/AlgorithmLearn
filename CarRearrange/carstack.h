namespace yin {
    
    template<class T>
    class SNode
    {
        friend class Stack<T>;
        T m_data;
        SNode<T> *m_link;
    };


    template<class T>
    class Stack
    {
        SNode<T> *m_top;
    public:
        Stack():m_top(nullptr){}
        Stack<T>& push(const T &rhs);
        Stack<T>& pop(T &rhs);
        bool isEmpty() const{ return m_top == NULL ;}
        T top() const 
        { 
            if(m_top) return m_top->m_data;
            else throw OutOfBounds();
        }
        ~Stack()  
        { 
            SNode<T> *p;
            while(m_top)
            {
                p = m_top;
                m_top= m_top->m_link;
                delete p;
            }
        }
    };

    template<class T>
    Stack<T>& Stack<T>::push(const T &rhs)
    {

        SNode<T> *p = new SNode<T>;
        p->m_link = m_top;
        p->m_data = rhs;
        m_top = p;
        return *this;
    }

    template<class T>
    Stack<T>& Stack<T>::pop(T &rhs)
    {
        if(m_top) 
        {
            SNode<T> *p = m_top;
            m_top = m_top->m_link;
            rhs = p->m_data;
            delete p;
        }
        else throw OutOfBounds();
        return *this;
    }


    template<class T>
    class Queue
    {
        SNode<T> *m_first, *m_rear;

    public:
        Queue():m_first(nullptr),m_rear(nullptr) {}
        bool isEmpty() const { return m_first == nullptr;}
        Queue<T>& enqueue(const T &rhs);
        Queue<T>& dequeue(T &rhs);
        ~Queue()
        {
            SNode<T> *p
            while (m_first)
            {
                p = m_first;
                m_first = m_first->m_link;
                delete p;
            }
        }
    };

    template<class T>
    Queue<T>& Queue<T>::enqueue(const T &rhs)
    {
        if (!m_first)
        {
            m_first = m_rear = new SNode<T>;
            m_first->m_data = rhs;
            m_first->m_link = nullptr;
        }
        else
        {
            m_rear->m_link = new SNode<T>;
            m_rear->m_link->m_data = rhs;
            m_rear->m_link->m_link = nullptr;
            m_rear = m_rear->m_link;
        }
        return *this;
    }

    template<class T>
    Queue<T>& Queue<T>::dequeue(T &rhs)
    {
        if (m_first)
        {
            SNode<T> *p = m_first;
            rhs = p->m_data;
            m_first = m_first->m_link;
            delete p;
        }
        else throw OutOfBounds();
        return *this;
    }

}