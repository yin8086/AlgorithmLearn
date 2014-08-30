//template<class T>
//class BinaryTree;

template<class T>
struct BinaryTreeNode
{
    //friend class BinaryTree;
    T m_data;
    BinaryTreeNode<T> *m_left,*m_right;
public:
    BinaryTreeNode(T data, BinaryTreeNode<T> *left, BinaryTreeNode<T> *right):
        m_data(data),m_left(left),m_right(right) {}
    BinaryTreeNode(T data):BinaryTreeNode(data, 0, 0) {}
    BinaryTreeNode():BinaryTreeNode(T(), 0, 0) {}
};


template<class T>
class BinaryTree
{
private:
    BinaryTreeNode<T> *m_root;
    void preOrder(std::function<void(BinaryTreeNode<T> *)> visit,
        BinaryTreeNode<T>* t); // recursive

    void clear(BinaryTreeNode<T> *p);

public:
    BinaryTree():m_root(nullptr) {}
    BinaryTree<T>& operator=(BinaryTree<T> && rhs)
    {
        m_root = rhs.m_root;
        rhs.m_root = nullptr;
        return *this;
    }
    BinaryTree(BinaryTree<T> && rhs)
    {
        m_root = rhs.m_root;
        rhs.m_root = nullptr;
    }
    ~BinaryTree();
    bool isEmpty() const { return m_root == nullptr; }
    bool getRoot(T& rhs) const;
    void makeTree(const T& rhs, BinaryTree<T> &lt, BinaryTree<T> &rt);
    void breakTree(T& rhs, BinaryTree<T> &left, BinaryTree<T> &right);
    void preOrder(std::function<void(BinaryTreeNode<T> *)> visit)
    {
        // recursive
        preOrder(visit, m_root);
    } 
    void inOrder(std::function<void(BinaryTreeNode<T> *)> visit); // stack method 1
    void postOrder(std::function<void(BinaryTreeNode<T> *)> visit); // stack method 2
    void levelOrder(std::function<void(BinaryTreeNode<T> *)> visit); // queue method


    static void unitTest();

};

template<class T>
void BinaryTree<T>::clear(BinaryTreeNode<T> *p)
{
    if(m_root)
    {
        clear(p->m_left);
        clear(p->m_right);
        delete p;
    }
}

template<class T>
BinaryTree<T>::~BinaryTree()
{
    postOrder([](BinaryTreeNode<T> *p) {delete p;});
    //clear(m_root);
    m_root = 0;
}

template<class T>
bool BinaryTree<T>::getRoot(T& rhs) const
{
    if (m_root)
    {
        rhs = m_root->m_data;
        return true;
    }
    else
        return false;
}

template<class T>
void BinaryTree<T>::makeTree(const T& rhs, BinaryTree<T> &left, BinaryTree<T> &right)
{
    m_root = new BinaryTreeNode<T>(rhs, left.m_root, right.m_root);
    left.m_root = right.m_root = nullptr;

}

template<class T>
void BinaryTree<T>::breakTree(T& rhs, BinaryTree<T> &left, BinaryTree<T> &right)
{
    if (!m_root)
    {
        throw BadInput();
    }

    rhs = m_root->m_data;
    left.m_root = m_root->m_left;
    right.m_root = m_root->m_right;

    delete m_root;
    m_root = nullptr;
}

template<class T>
void BinaryTree<T>::preOrder(std::function<void(BinaryTreeNode<T> *)> visit, BinaryTreeNode<T> *root)
{
    if (root)
    {
        visit(root);
        preOrder(visit, root->m_left);
        preOrder(visit, root->m_right);
    }
}


template<class T>
void BinaryTree<T>::inOrder(std::function<void(BinaryTreeNode<T> *)> visit)
{
    if(!m_root) return;
    BinaryTreeNode<T> *p;

    std::stack<BinaryTreeNode<T> *> nodeStack;
    // 1. push root element
    nodeStack.push(m_root);
    
    // 2. outer loop deal with a new tree(subtree)
    while(!nodeStack.empty())
    {
        while(p = nodeStack.top())
        {
            // first visit of root element, preorder
            nodeStack.push(p->m_left);
        }
        nodeStack.pop();
        if(!nodeStack.empty())
        {
            p = nodeStack.top(); // second visit of root element, inorder
            nodeStack.pop();
            visit(p);
            nodeStack.push(p->m_right); // push the right subtree and return outer loop
        }
    }

}

template<class T>
void BinaryTree<T>::postOrder(std::function<void(BinaryTreeNode<T> *)> visit)
{
    if(!m_root) return;
    std::stack<std::pair<BinaryTreeNode<T> *, int> > nodeStack;
    std::pair<BinaryTreeNode<T> *, int> nodePair;
    nodeStack.push(std::make_pair(m_root, 0)); // ready for first visit
    while(!nodeStack.empty())
    {
        nodePair = nodeStack.top(); 
        nodeStack.pop();
        switch(nodePair.second)
        {
        case 0:
            // first visit
            nodeStack.push(std::make_pair(nodePair.first, 1)); // ready for second
            if (nodePair.first->m_left) // push only if it's not empty
            {
                nodeStack.push(std::make_pair(nodePair.first->m_left, 0)); // left child first visit
            }
            break;
        case 1:
            // second visit
            nodeStack.push(std::make_pair(nodePair.first, 2)); // ready for third
            if (nodePair.first->m_right)
            {
                nodeStack.push(std::make_pair(nodePair.first->m_right, 0));
            }
            break;
        case 2:
            // third visit
            // no fourth
            visit(nodePair.first);
            break;
        }
    }

}

template<class T>
void BinaryTree<T>::levelOrder(std::function<void(BinaryTreeNode<T> *)> visit)
{
    if(!m_root) return;
    std::queue<BinaryTreeNode<T> *> nodeQueue;

    BinaryTreeNode<T> *p;
    nodeQueue.push(m_root);

    while (!nodeQueue.empty())
    {
        p = nodeQueue.front();
        nodeQueue.pop();
        visit(p);
        if (p->m_left)
        {
            nodeQueue.push(p->m_left);
        }
        if (p->m_right)
        {
            nodeQueue.push(p->m_right);
        }
        
    }

}

template<class T>
void BinaryTree<T>::unitTest()
{
    // (3+2*5)/(6+3)
    BinaryTree<T> emptyT, ta, tb, tc, td;

    std::cout<<"Original expression:\t(3+2*5)/(6-3)"<<std::endl;
    // Y.makeTree(value, X, X) only valid if X is null tree(m_root=0)
    tb.makeTree('2',emptyT,emptyT); // 2
    tc.makeTree('5',emptyT,emptyT); // 3

    ta.makeTree('*', tb, tc); // 2*5
    tc.makeTree('3', emptyT, emptyT); // 3
    tb.makeTree('+', tc, ta); // 3 + 2*5

    tc.makeTree('6', emptyT, emptyT); // 6
    ta.makeTree('3', emptyT, emptyT); // 3
    td.makeTree('-', tc, ta); // 6 + 3

    ta.makeTree('/', tb, td); //(3+2*5)/(6+3)

    
    // preOrder no ()
    std::cout<<"Prefix:\t\t";
    ta.preOrder([](BinaryTreeNode<T> *p){  std::cout<<p->m_data<<" ";});
    std::cout<<std::endl;

    // inOrder with ()
    std::cout<<"Infix:\t\t";
    ta.inOrder([](BinaryTreeNode<T> *p){  std::cout<<p->m_data<<" ";});
    std::cout<<std::endl;

    // postOrder no ()
    std::cout<<"Postfix:\t";
    ta.postOrder([](BinaryTreeNode<T> *p){  std::cout<<p->m_data<<" ";});
    std::cout<<std::endl;

    // levelOrder no ()
    std::cout<<"Levelfix:\t";
    ta.levelOrder([](BinaryTreeNode<T> *p){  std::cout<<p->m_data<<" ";});
    std::cout<<std::endl;
    
}