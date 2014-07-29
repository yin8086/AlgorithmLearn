enum NodeColor
{
    RED_NODE,
    BLACK_NODE
};


// is 2-3-4 tree indeed
// 4-node(3 elements) is  RED -- BLACK --- RED in RedBlack Tree
// 3-node(2 elements) is  BLACK --- RED in RedBlack Tree
// 2-node(1 elements) is  BLACK  in RedBlack Tree
template<class T>
struct RedBlackNode
{
    T m_data;
    RedBlackNode<T> *m_left,*m_right;
    NodeColor m_type;
public:
    RedBlackNode(T data, RedBlackNode<T> *left, RedBlackNode<T> *right, NodeColor nc):
        m_data(data),m_left(left),m_right(right), m_type(nc) {}
    RedBlackNode(T data):RedBlackNode(data, 0, 0, RED_NODE) {}
    RedBlackNode():RedBlackNode(T(), 0, 0, RED_NODE) {}
};

template<class E, class K>
class RedBlackTree
{
private:
    RedBlackNode<E> *m_root;
    void clear(RedBlackNode<E> *t);

    RedBlackNode<E> * rotateRight(RedBlackNode<E> *pA); 
    RedBlackNode<E> * rotateLeft(RedBlackNode<E> *pA);

    void inOrder(std::function<void(RedBlackNode<E> *)> visit, RedBlackNode<E> *t);
    // 0 null, 1 black, 2 red
    int nodeType(RedBlackNode<E> *p)
    {
        if (p)
        {
            return p->m_type == RED_NODE ? 2 : 1;
        }
        else
            return 0;
        // Null is Black
    }
    void setBlack(RedBlackNode<E> *p)
    {
        if (p)
        {
            p->m_type = BLACK_NODE;
        }
    }
    void setRed(RedBlackNode<E> *p)
    {
        #ifdef _DEBUG
        assert(p);
        #endif // _DEBUG
        p->m_type = RED_NODE;
    }

    std::tuple<int, int, bool> isRedBlack(RedBlackNode<E> *t);

public:
    RedBlackTree():m_root(nullptr) {}
    ~RedBlackTree()
    {
        clear(m_root);
        m_root = 0;
    }
    RedBlackTree<E, K>& insert(const E& eValue);
    RedBlackTree<E, K>& remove(const K& key, E& eValue);
    bool search(const K& key, E& eValue);

    void inOrder(std::function<void(RedBlackNode<E> *)> visit)
    {
        inOrder(visit, m_root);
        std::cout<<std::endl;
    }

    void testBalance()
    {
        std::tuple<int, int, bool> retT = isRedBlack(m_root);
        bool isRB = std::get<2>(retT);
        std::cout<<std::endl<<"Height: "<<std::get<1>(retT);
        std::cout<<"\tIs it a RedBlack?-----"<<(isRB?"true":"false")<<std::endl;
    }

    static void unitTest();

};

template<class E, class K>
void RedBlackTree<E, K>::clear(RedBlackNode<E> *t)
{
    if (t)
    {
        clear(t->m_left);
        clear(t->m_right);
        delete t;
    }
}

template<class E, class K>
void RedBlackTree<E, K>::inOrder(std::function<void(RedBlackNode<E> *)> visit, RedBlackNode<E> *t)
{
    if (t)
    {
        inOrder(visit, t->m_left);
        visit(t);
        inOrder(visit, t->m_right);
    }
}

// Rotation Color change is different in insert and remove, 
// but rotation is the same, so we only rotate
// and let insert and remove change the color

template<class E, class K>
RedBlackNode<E> * RedBlackTree<E, K>::rotateLeft(RedBlackNode<E> *pA)
{
    RedBlackNode<E> *pV = pA->m_right;
    pA->m_right = pV->m_left;
    pV->m_left = pA;
    return pV;
}


template<class E, class K>
RedBlackNode<E> * RedBlackTree<E, K>::rotateRight(RedBlackNode<E> *pA)
{
    RedBlackNode<E> *pV = pA->m_left;
    pA->m_left = pV->m_right;
    pV->m_right = pA;
    return pV;
}

template<class E, class K>
std::tuple<int, int, bool> RedBlackTree<E, K>::isRedBlack(RedBlackNode<E> *t)
{
    if (!t)
    {
        return std::make_tuple(1, 0, true);
    }

    std::tuple<int, int, bool> lhs = isRedBlack(t->m_left);
    std::tuple<int, int, bool> rhs = isRedBlack(t->m_right);

    int height = std::get<1>(lhs) > std::get<1>(rhs) ? std::get<1>(lhs) + 1: std::get<1>(rhs)+1;
#ifdef _DEBUG
    std::cout<<"("<<t->m_data<<", "<<( t->m_type == 0?"RED":"BLACK")
        <<") ";
#endif // _DEBUG
    if (std::get<2>(lhs) && std::get<2>(rhs))
    {
        if (nodeType(t)==2 && ( nodeType(t->m_left)==2 || nodeType(t->m_right)==2 ))
        {
            return std::make_tuple(0, height, false);
        }
        if (std::get<0>(lhs) != std::get<0>(rhs))
        {
            return std::make_tuple(0, height, false);
        }
        else
        {
            return std::make_tuple(std::get<0>(lhs)+(nodeType(t)==2?0:1), height, true);
        }

    }
    else
    {
        return std::make_tuple(0, height, false);
    }

}


template<class E, class K>
bool RedBlackTree<E, K>::search(const K& key, E& eValue)
{
    RedBlackNode<E> *p = m_root;
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
RedBlackTree<E, K>& RedBlackTree<E, K>::insert(const E& eValue)
{
    RedBlackNode<E> *p= m_root, *pp = nullptr, *gp = nullptr; 
    std::stack<RedBlackNode<E> * > pathStack;
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

    RedBlackNode<E> *newNode = new RedBlackNode<E>(eValue, 0 ,0, RED_NODE);

    // 2. insert Node
    if (!pp)
    {
        m_root = newNode;
        setBlack(m_root); // set root Node to Black
        // finish insert into an empty tree
    }
    else
    {
        char rType[2];
        if (key < pp->m_data)
        {
            pp->m_left = newNode;
            rType[1] = 'L';
        }
        else
        {
            pp->m_right = newNode;
            rType[1] = 'R';
        }

        // 3. rotate or change color
        
        do  // may recursively balance the tree
        {
            pp = pathStack.top();
            if (nodeType(pp)==2) // do not allow two consecutive red nodes
            {
                pathStack.pop(); // pop pp
                gp = pathStack.top();
                pathStack.pop(); // pop gp

                rType[0] = pp == gp->m_left ? 'L' : 'R';

                if ( ( rType[0] == 'L' && nodeType(gp->m_right)==2 ) ||
                     ( rType[0] == 'R' && nodeType(gp->m_left)==2 ) )
                {
                    // XXr
                    // insert into 4-node, so it will be split into a 
                    // a 3-node and a 2-node, and insert middle element
                    // to parents
                    
                    // so left right is black(single node) and
                    // middle is red which become parts of parent node
                    // because insertion into parents, parents may check
                    // if the node is full again
                    setBlack(gp->m_left);
                    setBlack(gp->m_right);
                    if (!pathStack.empty())
                    {
                        setRed(gp);

                        if (nodeType(pathStack.top())==2)
                        {
                            // two consecutive red node, continue loop
                            rType[1] =  gp == pathStack.top()->m_left ? 'L' : 'R';
                        }
                    }
                }
                else
                {
                    // XXb
                    // insert into a 3-node, and a middle element is selected to be black
                    // and left right will be red
                    if (rType[0] == 'L' && nodeType(gp->m_right)<=1) // Lxb
                    {
                        if (rType[1] == 'R')
                        {
                            gp->m_left = rotateLeft(gp->m_left);
                        }

                        gp = rotateRight(gp);
                    }
                    else if (rType[0] == 'R' && nodeType(gp->m_left)<=1) //Rxb
                    {
                        if (rType[1] == 'L')
                        {
                            gp->m_right = rotateRight(gp->m_right);
                        }

                        gp = rotateLeft(gp);
                    }

                    setBlack(gp);
                    setRed(gp->m_left);
                    setRed(gp->m_right);

                    if (pathStack.empty())
                    {
                        m_root = gp;
                    }
                    else
                    {
                        if (gp->m_data < pathStack.top()->m_data)
                        {
                            pathStack.top()->m_left = gp;
                        }
                        else
                        {
                            pathStack.top()->m_right = gp;
                        }
                    }

                    break;
                }

            }
            else
            {
                // ---Black Node do nothing and finish insert
                break;
            }
        } while (!pathStack.empty());

    }
    return *this;
}


template<class E, class K>
RedBlackTree<E, K>& RedBlackTree<E, K>::remove(const K& key, E& eValue)
{
    RedBlackNode<E> *p = m_root, *pp = nullptr; 
    std::stack<RedBlackNode<E> * > pathStack;
    bool needBalance = false;
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
            RedBlackNode<E> *s = p->m_left, *ps = p;

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
        RedBlackNode<E> *c = p->m_left ? p->m_left : p->m_right;

        // only two black need rebalace
        needBalance = nodeType(p) == 1 && nodeType(c) <=1;

        // remove an element from a 2-node/3-node
        // the left one element will be black
        setBlack(c);
        

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

    if (!needBalance)
    {
        return *this;
    }

    do 
    {
        pp = pathStack.top();
        pathStack.pop();
        NodeColor origColor = pp->m_type;
        if ( (dType == 'R' && nodeType(pp->m_left)==1 &&  nodeType(pp->m_left->m_left)<=1 && nodeType(pp->m_left->m_right)<=1)
            || (dType == 'L' && nodeType(pp->m_right)==1 &&  nodeType(pp->m_right->m_left)<=1 && nodeType(pp->m_right->m_right)<=1)  
            )
        {
            // Lb0, Rb0
            // rob an element from parent and merge left right parent three nodes
            // since remove will make parent empty, so need re-loop
            setBlack(pp);
            if (dType == 'R')
            {
                setRed(pp->m_left);
            }
            else
            {
                setRed(pp->m_right);
            }

            // parents red, have >=2  element, remove is safe
            // do not need to re-loop
            if (origColor == RED_NODE)
            {
                break;
            }
            // empty(), root jump out loop
            // not empty, set type, re-loop
            if (!pathStack.empty())
            {
                dType = pp == pathStack.top()->m_left ? 'L' : 'R';
            }
        }
        else
        {
            
            if (dType == 'R')
            {
                if (nodeType(pp->m_left) == 1)  // Rb
                {
                    // borrow a node X from left neighbor
                    // X goto parent Y's pos ,Y go to current node
                    // so new root become the color of original color
                    // Y become black

                    if (nodeType(pp->m_left->m_right) == 2) //Rb2 , Rb1(2)
                    {
                        pp->m_left = rotateLeft(pp->m_left);
                    }
                    pp = rotateRight(pp); // Rb2, Rb1(1,2)

                    pp->m_type = origColor;
                    setBlack(pp->m_right);
                    // a 3-node become 2-node, red become black
                    setBlack(pp->m_left); // Rb1(1)
                    //
                }
                else if(nodeType(pp->m_left) == 2)// Rr
                {
                    // Rr1(2), Rr2
                    if (nodeType(pp->m_left->m_right)==1 &&  nodeType(pp->m_left->m_right->m_right) == 2 )
                    {
                        // p's left neighbor has a 4-node/3-node Y, the right-most 
                        // element of Y is  X which
                        // need to be red

                        // borrow X, which makes that X replace pp in parent, 
                        // and pp go to right

                        RedBlackNode<E> *x = pp->m_left->m_right->m_right;
                        pp->m_left->m_right->m_right = x->m_left;
                        x->m_left = pp->m_left;
                        pp->m_left = x->m_right;
                        x->m_right = pp;
                        setBlack(x);
                        pp = x;
                    }
                    
                    // Rr1(1)
                    else if (nodeType(pp->m_left->m_right)==1 && nodeType(pp->m_left->m_right->m_left) == 2)
                    {
                            // p's left neighbor has a 3-node Y, the right-most 
                            // element of Y is  X 
                            // will go to parent, and parent go to p
                            // so Y left only one element, and become black

                            pp->m_left = rotateLeft(pp->m_left);
                            pp = rotateRight(pp);
                            setBlack(pp->m_left->m_right);
                    }
                    else if(nodeType(pp->m_left->m_right) == 1)// Rr0
                    {
                        // p's left neighbor has a 2-node Y, p will merge left 
                        // neighbor, parent into one node, so parent's only one left
                        // elements will be black, and left neighbor become Red

                        pp = rotateRight(pp);
                        setBlack(pp);
                        setRed(pp->m_right->m_left);
                    }

                }
            }
            else
            {
                if (nodeType(pp->m_right) == 1)  // Lb
                {
                    // borrow a node X from right neighbor
                    // X goto parent Y's pos ,Y go to current node
                    // so new root become the color of original color
                    // Y become black

                    if (nodeType(pp->m_right->m_left) == 2) //Lb2, Lb1(2)
                    {
                        pp->m_right = rotateRight(pp->m_right);
                    }
                    pp = rotateLeft(pp); // Lb2, Lb1(1,2)

                    pp->m_type = origColor;
                    setBlack(pp->m_left);
                    // a 3-node become 2-node, red become black
                    setBlack(pp->m_right); // Lb1(1) !!!!!!!!!!!!!

                }
                else if(nodeType(pp->m_right) == 2)// Lr
                {
                    // Lr1(2), Lr2
                    if (nodeType(pp->m_right->m_left) == 1 && nodeType(pp->m_right->m_left->m_left) == 2 )
                    {
                        // p's right neighbor has a 4-node/3-node Y, the left-most 
                        // element of Y is  X which
                        // need to be red

                        // borrow X, which makes that X replace pp in parent, 
                        // and pp go to right, x become black

                        RedBlackNode<E> *x = pp->m_right->m_left->m_left;
                        pp->m_right->m_left->m_left = x->m_right;
                        x->m_right = pp->m_right;
                        pp->m_right = x->m_left;
                        x->m_left = pp;
                        setBlack(x);
                        pp = x;
                    }

                    // Lr1(1)
                    else if (nodeType(pp->m_right->m_left) == 1 && nodeType(pp->m_right->m_left->m_right) == 2)
                    {


                        // p's right neighbor has a 3-node Y, the left-most 
                        // element of Y is  X 
                        // will go to parent, and parent go to p
                        // so Y left only one element, and become black

                        pp->m_right = rotateRight(pp->m_right);
                        pp = rotateLeft(pp);
                        setBlack(pp->m_right->m_left);
                    }
                    else if(nodeType(pp->m_right->m_left) == 1) // Lr0
                    {
                        // p's right neighbor has a 2-node Y, p will merge right 
                        // neighbor, parent into one node, so parent's only one left
                        // elements will be black, and right neighbor become Red

                        pp = rotateLeft(pp);
                        setBlack(pp);
                        setRed(pp->m_left->m_right);
                    }


                }
            }

            if (pathStack.empty())
            {
                m_root = pp;
            }
            else
            {
                if (pp->m_data < pathStack.top()->m_data)
                {
                    pathStack.top()->m_left = pp;
                }
                else
                {
                    pathStack.top()->m_right = pp;
                }
            }

            break;
            // only one rotate
        }
    } while (!pathStack.empty());
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
void RedBlackTree<E, K>::unitTest()
{
    RedBlackTree<E, K> rbTree;
    E tmpE;
    std::function<void(RedBlackNode<E> *p)> vfunc = [](RedBlackNode<E> *p){  
        std::cout<<p->m_data<<" ";} ;
    
    /*
    int testSeq[] = {10, 40, 0, 8, 0};
    
    for (int i=0; i<sizeof(testSeq)/sizeof(*testSeq); ++i)
    {
        tmpE.m_key = testSeq[i];
        tmpE.m_data = testSeq[i];
        rbTree.insert(tmpE);
    }
    */

    
    int numE = 100;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, numE * 4);
    int randNum;
    
    for (int i=0; i<numE/2; ++i)
    {
        randNum = uidn(newEngine);
        tmpE.m_key = randNum;
        tmpE.m_data = randNum;
        rbTree.insert(tmpE);
    }
    std::cout<<"============"<<std::endl;
    rbTree.testBalance();
    rbTree.inOrder(vfunc);


    for (int i=0; i<numE*2; ++i)
    {
        rbTree.remove(uidn(newEngine), tmpE);
        
    }
    std::cout<<"============"<<std::endl;
    rbTree.testBalance();
    rbTree.inOrder(vfunc);

    
    for (int i=numE/2; i<numE; ++i)
    {
        randNum = uidn(newEngine);
        tmpE.m_key = randNum;
        tmpE.m_data = randNum;
        rbTree.insert(tmpE);
    }
    std::cout<<"============"<<std::endl;
    rbTree.testBalance();
    rbTree.inOrder(vfunc);


    for (int i=0; i<numE*2; ++i)
    {
        rbTree.remove(uidn(newEngine), tmpE);

    }
    std::cout<<"============"<<std::endl;
    rbTree.testBalance();
    rbTree.inOrder(vfunc);
    
}