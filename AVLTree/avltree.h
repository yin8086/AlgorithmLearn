

template<class T>
struct AVLNode
{
    T m_data;
    AVLNode<T> *m_left,*m_right;
    int8_t m_bf;
public:
    AVLNode(T data, AVLNode<T> *left, AVLNode<T> *right, int8_t bf):
        m_data(data),m_left(left),m_right(right), m_bf(bf) {}
    AVLNode(T data):AVLNode(data, 0, 0, 0) {}
    AVLNode():AVLNode(T(), 0, 0, 0) {}
};

template<class E, class K>
class AVLTree
{
private:
    AVLNode<E> *m_root;
    bool m_isAvl;
    void clear(AVLNode<E> *t);
    AVLNode<E> * rotateRight(AVLNode<E> *pA); 
    AVLNode<E> * rotateLeft(AVLNode<E> *pA); 
    void inOrder(std::function<void(AVLNode<E> *)> visit, AVLNode<E> *t);
    int getHeightAndBf(AVLNode<E> *t);

public:
    AVLTree():m_root(nullptr),m_isAvl(true) {}
    ~AVLTree()
    {
        clear(m_root);
        m_root = 0;
    }
    AVLTree<E, K>& insert(const E& eValue);
    AVLTree<E, K>& remove(const K& key, E& eValue);
    bool search(const K& key, E& eValue);
    void inOrder(std::function<void(AVLNode<E> *)> visit)
    {
        inOrder(visit, m_root);
        std::cout<<std::endl;
    }

    bool isAVL()
    {
        int height = getHeightAndBf(m_root);
        std::cout<<std::endl<<"Height: "<<height<<std::endl;
        bool ret = m_isAvl;
        return ret;
    }
    static void unitTest();

};

template<class E, class K>
void AVLTree<E, K>::clear(AVLNode<E> *t)
{
    if (t)
    {
        clear(t->m_left);
        clear(t->m_right);
        delete t;
    }
}

template<class E, class K>
void AVLTree<E, K>::inOrder(std::function<void(AVLNode<E> *)> visit, AVLNode<E> *t)
{
    if (t)
    {
        inOrder(visit, t->m_left);
        visit(t);
        inOrder(visit, t->m_right);
    }
}

template<class E, class K>
bool AVLTree<E, K>::search(const K& key, E& eValue)
{
    AVLNode<E> *p = m_root;
    while (p)
    {
        if (key < p->m_data)
        {
            p = p->m_left;
        }
        else if (key > p->m_data)
        {
            p = p->m_right;
        }
        else
        {
            eValue = p->m_data;
            return true;
        }
    }
    return false;
}

template<class E, class K>
int AVLTree<E, K>::getHeightAndBf(AVLNode<E> *t)
{
    if (!t)
    {
        return 0;
    }
    else
    {
        int heightL = getHeightAndBf(t->m_left);
        int heightR = getHeightAndBf(t->m_right);
        std::cout<<"("<<t->m_data<<":"<<heightL-heightR<<") ";
#ifdef _DEBUG
        assert(heightL-heightR == t->m_bf);
#endif
        if (m_isAvl)
        {
            if (heightL-heightR < -1 || heightL-heightR > 1)
            {
                m_isAvl = false;
            }
        }
        return heightL < heightR ? heightR+1 : heightL + 1;
    }
}



template<class E, class K>
AVLNode<E> * AVLTree<E, K>::rotateRight(AVLNode<E> *pA)
{
    AVLNode<E> *pV = pA->m_left;
    pA->m_left = pV->m_right;
    pV->m_right = pA;
    return pV;
}

template<class E, class K>
AVLNode<E> * AVLTree<E, K>::rotateLeft(AVLNode<E> *pA)
{
    AVLNode<E> *pV = pA->m_right;
    pA->m_right = pV->m_left;
    pV->m_left = pA;
    return pV;
}

template<class E, class K>
AVLTree<E, K>& AVLTree<E, K>::insert(const E& eValue)
{
    AVLNode<E> *p_A = nullptr, *p; // A ptr and current ptr
    AVLNode<E> *p_pA = nullptr, *pp = nullptr; // A parent ptr and current parent ptr
    p = m_root;
    std::stack<AVLNode<E> * > pathStack;
    K key = eValue;
    
    // 1. find insert position
    while(p)
    {
        // 1.1 push path into stack
        pathStack.push(p);
        pp = p;
        if (key < p->m_data)
        {
            p = p->m_left;
        }
        else if (key > p->m_data)
        {
            p = p->m_right;
        }
        else
        {
            //throw BadInput(); //distinct key
            return *this;
        }
    }

    AVLNode<E> *newNode = new AVLNode<E>(eValue, 0 ,0, 0);
    
    // 2. insert Node
    if (!pp)
    {
        m_root = newNode;
    }
    else
    {
        if (key < pp->m_data)
        {
            pp->m_left = newNode;
        }
        else
        {
            pp->m_right = newNode;
        }
    }

    // 3. go back from new node to A to change bf
    //    to see if it is needed to rotate
    char rType[2] = {0};
    int rIdx = 0;
    AVLNode<E> *p2;
    //  top change from new node'parent to A's parent
    //  and change bf
    int8_t bfP;
    while(!pathStack.empty())
    {
        p2 = pathStack.top();
        pathStack.pop();

        bfP = p2->m_bf;
        if (key < p2->m_data)
        {
            p2->m_bf ++;
            rType[rIdx] = 'L';
        }
        else
        {
            p2->m_bf --;
            rType[rIdx] = 'R';
        }
        rIdx = 1-rIdx;

        // +-1  -> 0 stop or rotate
        if (bfP == 1 || bfP == -1) // last 1 or -1 node is A node
        {
            p_A = p2;
            p_pA = pathStack.empty() ? nullptr : pathStack.top();
            break;
        }
        // 0 -> +-1 go

    }
    if (p_A)
    {
        // 4. rotate
        if (p_A->m_bf == 2)
        {
            // LL, LR

            if (rType[rIdx] == 'R') // LR
            {
                p_A->m_left = rotateLeft(p_A->m_left);
            }

            // LL, LR
            p_A = rotateRight(p_A);
            if (!p_pA)
            {
                m_root = p_A;
            }
            else
            {
                if (p_A->m_data < p_pA->m_data)
                {
                    p_pA->m_left = p_A;
                }
                else
                {
                    p_pA->m_right = p_A;
                }
            }


            // change balance factor
            if (rType[rIdx] == 'L') // LL
            {
                p_A->m_bf = 0;
                p_A->m_right->m_bf = 0;
            } 
            else // LR
            {
                int oldBf = p_A->m_bf;
                if (oldBf == 0)
                {
                    p_A->m_left->m_bf = p_A->m_right->m_bf = 0;
                }
                else if (oldBf == 1)
                {
                    p_A->m_left->m_bf = 0;
                    p_A->m_right->m_bf = -1;
                }
                else if (oldBf == -1)
                {
                    p_A->m_left->m_bf = 1;
                    p_A->m_right->m_bf = 0;
                }
                p_A->m_bf = 0;
            }

        }
        else if (p_A->m_bf == -2)
        {
            // RR, RL
            if (rType[rIdx] == 'L') // RL
            {
                p_A->m_right = rotateRight(p_A->m_right);
            }

            // RR, RL
            p_A = rotateLeft(p_A);
            if (!p_pA)
            {
                m_root = p_A;
            }
            else
            {
                if (p_A->m_data < p_pA->m_data)
                {
                    p_pA->m_left = p_A;
                }
                else
                {
                    p_pA->m_right = p_A;
                }
            }

            // change balance factor
            if (rType[rIdx] == 'R') // RR
            {
                p_A->m_bf = 0;
                p_A->m_left->m_bf = 0;
            }
            else // RL
            {
                int oldBf = p_A->m_bf;
                if (oldBf == 0)
                {
                    p_A->m_left->m_bf = p_A->m_right->m_bf = 0;
                }
                else if (oldBf == 1)
                {
                    p_A->m_left->m_bf = 0;
                    p_A->m_right->m_bf = -1;
                }
                else if (oldBf == -1)
                {
                    p_A->m_left->m_bf = 1;
                    p_A->m_right->m_bf = 0;
                }
                p_A->m_bf = 0;
            }
        }
        // else do nothing
     }   // modify to root
     m_isAvl = true;
     return *this;
}

template<class E, class K>
AVLTree<E, K>& AVLTree<E, K>::remove(const K& key, E& eValue)
{

    AVLNode<E> *p_A = nullptr, *p; // A ptr and current ptr
    AVLNode<E> *p_pA = nullptr, *pp = nullptr; // A parent ptr and current parent ptr
    p = m_root;
    std::stack<AVLNode<E> * > pathStack;
    char dType = 0;

    // 1. find remove position
    while(p)
    {
        // 1.1 push path into stack
        pathStack.push(p);
        
        if (key == p->m_data)
        {
            break;
        }
        else
        {
            pp = p; // set pp after we move ahead
            if (key < p->m_data)
            {
                p = p->m_left;
            }
            else if (key > p->m_data)
            {
                p = p->m_right;
            }
        }
        
    }

    if (!p)
    {
        // throw BadInput(); // No such key
        return *this;
    }
    else
    {
        // remove the element
        // a little complex
        
        eValue = p->m_data;

        // if have two child, find left child right most
        if (p->m_left && p->m_right)
        {
            AVLNode<E> *s = p->m_left, *ps = p;

            pathStack.push(s);
            while (s->m_right)
            {
                ps = s;
                pathStack.push(s->m_right);
                s = s->m_right;
            }
            p->m_data = s->m_data;
            p = s;
            pp = ps;
        }
        
        pathStack.pop(); // pop p
        AVLNode<E> *c = p->m_left ? p->m_left : p->m_right;

        if (!pp)
        {
            m_root = c;
        }
        else
        {
            if (p == pp->m_left)
            {
                pp->m_left = c;
                dType = 'L';
            }
            else
            {
                pp->m_right = c;
                dType = 'R';
            }
        }
        delete p;
        
    }

    AVLNode<E> *p2;
    int bfP = 0;
    do // every iteration delete a node and only L0, R0 stop rotate
    {
        //  top change from new node'parent to A's parent
        //  and change bf
        p_A  = p_pA = nullptr;
        while(!pathStack.empty())
        {
            p2 = pathStack.top();
            pathStack.pop();

            bfP = p2->m_bf;
            // can not use key
            if (dType == 'L') //R
            {
                p2->m_bf --;
            }
            else //R
            {
                p2->m_bf ++;
            }


            if (bfP == 0) // 0->+-1, stop
            {
                break;
            }
            else  // 1,-1   ->    0/2/-2, rotate or go
            {

                // set 'L' and 'R' for next loop
                if (!pathStack.empty())
                {
                    dType = pathStack.top()->m_left == p2 ? 'L' : 'R';
                }
                else
                    dType = 0;

                if (p2->m_bf == 2 || p2->m_bf == -2)
                {
                    p_A = p2;
                    p_pA = pathStack.empty() ? nullptr : pathStack.top();
                    break;
                }
            }

            // ready to next loop
            
        }

        if (bfP == 0)  // 0->+-1  finishx
        {
            break;
        }

        if (p_A)
        {
            // only L0 R0 don't need to reloop
            int8_t typeN;
            if (p_A->m_bf == -2)
            {
                // L0, L-1, L+1
                typeN = p_A->m_right->m_bf;
                if (typeN == 1)
                {
                    p_A->m_right = rotateRight(p_A->m_right);
                }

                p_A = rotateLeft(p_A);
                if (!p_pA)
                {
                    m_root = p_A;
                }
                else
                {
                    if (p_A->m_data < p_pA->m_data)
                    {
                        p_pA->m_left = p_A;
                    }
                    else
                    {
                        p_pA->m_right = p_A;
                    }
                }

                // change balance factor
                if (typeN == 0) // L0
                {
                    p_A->m_bf = 1;
                    p_A->m_left->m_bf = -1;
                    break;
                }
                else if (typeN == -1) // L-1
                {
                    p_A->m_bf = 0;
                    p_A->m_left->m_bf = 0;
                    // continue loop
                }
                else  // L+1
                {
                    int oldBf = p_A->m_bf;
                    if (oldBf == 0)
                    {
                        p_A->m_left->m_bf = p_A->m_right->m_bf = 0;
                    }
                    else if (oldBf == 1)
                    {
                        p_A->m_left->m_bf = 0;
                        p_A->m_right->m_bf = -1;
                    }
                    else if (oldBf == -1)
                    {
                        p_A->m_left->m_bf = 1;
                        p_A->m_right->m_bf = 0;
                    }
                    p_A->m_bf = 0;

                }
            }
            else if(p_A->m_bf == 2)
            {
                // R0, R+1, R-1
                typeN = p_A->m_left->m_bf;
                if (typeN == -1)
                {
                    p_A->m_left = rotateLeft(p_A->m_left);
                }

                p_A = rotateRight(p_A);
                if (!p_pA)
                {
                    m_root = p_A;
                }
                else
                {
                    if (p_A->m_data < p_pA->m_data) 
                    {
                        p_pA->m_left = p_A;
                    }
                    else
                    {
                        p_pA->m_right = p_A;
                    }
                }

                // change balance factor
                if (typeN == 0) // R0
                {
                    p_A->m_bf = -1;
                    p_A->m_right->m_bf = 1;
                    break;
                }
                else if (typeN == 1) // R1
                {
                    p_A->m_bf = 0;
                    p_A->m_right->m_bf = 0;
                    // continue loop
                }
                else  // R-1
                {
                    int oldBf = p_A->m_bf;
                    if (oldBf == 0)
                    {
                        p_A->m_left->m_bf = p_A->m_right->m_bf = 0;
                    }
                    else if (oldBf == 1)
                    {
                        p_A->m_left->m_bf = 0;
                        p_A->m_right->m_bf = -1;
                    }
                    else if (oldBf == -1)
                    {
                        p_A->m_left->m_bf = 1;
                        p_A->m_right->m_bf = 0;
                    }
                    p_A->m_bf = 0;
                }
            }
        }

    }while(!pathStack.empty());
    m_isAvl = true;
    return *this;
}



///////////////////////////////////////////

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
void AVLTree<E, K>::unitTest()
{
    AVLTree<E, K> avlTree;
    E tmpE;
    std::function<void(AVLNode<E> *p)> vfunc = [](AVLNode<E> *p){  std::cout<<p->m_data<<" ";} ;
    //int testSeq[] = {5, 3, 9, 2, 4, 7, 12, 1, 6, 8, 11, 13, 10};
    /*
    for (int i=0; i<sizeof(testSeq)/sizeof(*testSeq); ++i)
    {
        tmpE.m_key = testSeq[i];
        tmpE.m_data = testSeq[i];
        avlTree.insert(tmpE);
    }
    */
    int numE = 1000;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, numE * 4);
    int randNum;
    /*
    for (int i=0; i<numE; ++i)
    {
        randNum = uidn(newEngine);
        tmpE.m_key = randNum;
        tmpE.m_data = randNum;
        avlTree.insert(tmpE);
    }
    
    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);

    for (int i=0; i<numE; ++i)
    {
        avlTree.remove(uidn(newEngine), tmpE);
    }

    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);
    */

    for (int i=0; i<numE/2; ++i)
    {
        randNum = uidn(newEngine);
        tmpE.m_key = randNum;
        tmpE.m_data = randNum;
        avlTree.insert(tmpE);
    }
    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);
    std::cout<<"==============="<<std::endl;
    for (int i=0; i<numE/2; ++i)
    {
        avlTree.remove(uidn(newEngine), tmpE);
    }
    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);
    std::cout<<"==============="<<std::endl;
    for (int i=numE/2; i<numE; ++i)
    {
        randNum = uidn(newEngine);
        tmpE.m_key = randNum;
        tmpE.m_data = randNum;
        avlTree.insert(tmpE);
    }
    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);
    std::cout<<"==============="<<std::endl;
    for (int i=0; i<numE/2; ++i)
    {
        avlTree.remove(uidn(newEngine), tmpE);
    }
    std::cout<<"Is it an AVL tree ?"<<(avlTree.isAVL()?"True":"False")<<std::endl;
    avlTree.inOrder(vfunc);
    std::cout<<"==============="<<std::endl;

}