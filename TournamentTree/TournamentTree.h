

template<class T>
class WinnerTree
{
private:
    int *m_tree; // tree internal node
    T *m_player; // player
    int m_Maxsize;
    int m_size;
    int m_lowExt; // lowest level(level k+1) external node number
    int m_offset; // node numbers in level 1~k = 2^k - 1
    void play(int idxTree, int idxP1, int idxP2, std::function<int(T *, int, int)> winner);

public:
    WinnerTree(int maxSize):m_Maxsize(maxSize),m_size(0)
    {
        // internal node are 2-degree node, external are leafs, no 1-degree nodes
        // n(2)+1 = n(0), n(i) is the number of i-degree node
        // m_player 1...MaxSize, Maxsize
        // m_tree   1...Maxsize-1, Maxsize-1
        m_tree = new int[m_Maxsize];
    }
    ~WinnerTree() {delete [] m_tree;}
    void initialize(T a[], int size, std::function<int(T *, int, int)> winner);
    void replay(int i, std::function<int(T *, int, int)> winner);
    int winner() const { return (m_size) ? m_tree[1] : 0; }
    int winner(int i) const { return (i< m_size) ? m_tree[i] : 0;}
};


template<class T>
void WinnerTree<T>::initialize(T a[], int size, std::function<int(T *, int, int)> winner)
{
    if (size > m_Maxsize || size < 2)
    {
        throw BadInput();
    }

    m_player = a;
    m_size = size;
    
    // n = m_size : the leaf/external nodes of the tree
    // calculate s: the first internal node index in last level
    // floor(log2(n-1)) = m, n-1 is the internal size
    // log2(n-1) >= m,  2^m <= n-1
    // s= 2^m <= n-1
    int s;
    for (s=1; 2*s <= m_size-1; s <<= 1);

    // m_size - s : the number of internal node in last level
    // 2*s is the child of s and -1 is the size of all the nodes 
    // from 1 to level k-1
    m_lowExt = 2*(m_size - s);
    m_offset = 2*s - 1; 

    int nodeIdx;
    for(nodeIdx = 2; nodeIdx <= m_lowExt; nodeIdx += 2)
        play((nodeIdx+m_offset)/2, nodeIdx-1, nodeIdx, winner);

    // n is odd and there will be an external node 
    // whose left brother is internal node
    // and its position must be at n pos in tree 
    // and n-1 is the left child of its parent
    // final move to the next right child
    if (m_size % 2)
    {
        play(m_size/2, m_tree[m_size-1], m_lowExt+1, winner);
        nodeIdx = m_lowExt + 3; // + 2 is next and left, +3 is right
    }
    else
        nodeIdx = m_lowExt + 2; // +1 is next and left, +2 is right


    // nodeIdx - m_lowExt is the idx in the upper idx counter from the first 
    // element in the level
    for(; nodeIdx <= m_size; nodeIdx += 2)
        play( (nodeIdx-m_lowExt+ m_size-1)/2, nodeIdx-1, nodeIdx, winner);

}

template<class T>
void WinnerTree<T>::play(int idxTree, int idxP1, int idxP2, std::function<int(T *, int, int)> winner)
{
    // set play result
    m_tree[idxTree] = winner(m_player, idxP1, idxP2);

    while (idxTree > 1 && idxTree % 2)
    {
        m_tree[idxTree/2] = winner(m_player, m_tree[idxTree-1], m_tree[idxTree]);
        idxTree /= 2;
    }

}

template<class T>
void WinnerTree<T>::replay(int i, std::function<int(T *, int, int)> winner)
{
    if (i < 1 || i > m_size)
    {
        throw OutOfBounds();
    }

    // parent is index of m_tree
    // lchild is index of m_player
    int parent, lchild, rchild;
    bool isLeft = true;
    if (i <= m_lowExt)
    {
        parent = (i + m_offset)/2;
        lchild = parent * 2 - m_offset;
        rchild = lchild + 1;
        isLeft = lchild == i;
    }
    else
    {
        parent = ((i - m_lowExt) + m_size - 1) / 2;
        if (parent * 2 == (m_size - 1)) // n is odd
        {
            lchild =  m_tree[parent * 2];
            rchild = i;
            isLeft = false;
        }
        else
        {
            lchild =(parent * 2 - (m_size - 1) + m_lowExt);
            rchild = lchild + 1;
            isLeft = lchild == i;
        }
        
    }

    // when the winner unchanged to old player, stop playing
    // child idx is in player, not in tree
    int newResult;
    do
    {
        newResult = winner(m_player, lchild, rchild);
        if (isLeft && newResult == m_tree[parent] && newResult == rchild)
        {
            // lchild changed and winner unchanged to rchild
            break;
        }
        else if(!isLeft && newResult == m_tree[parent] && newResult == lchild)
        {
            // rchild changed and winner unchanged to lchild
            break;
        }

        // continue to replay
        m_tree[parent] = newResult;
        if (parent > 1)
        {
            parent = parent / 2;
            lchild = m_tree[parent * 2];
            // must consider when lchild is n-1 node
            // and this time rchild is not in the tree but a external node
            // play with internal and external
            if (parent * 2 == m_size - 1)
            {
                rchild = m_lowExt + 1;
            }
            else
            {
                rchild = m_tree[parent * 2 + 1];
            }
        }
        else
            break;
    } while (1);


}




//@param a[1...n]
template<class T>
void TournamentSort(T a[], int n, const int &MAX_VALUE)
{
    WinnerTree<T> wt(n);

    std::function<int(T *, int, int)> winFunc = [](T *a, int lc, int rc) -> int { return a[lc] <= a[rc] ? lc : rc; };
    wt.initialize(a, n, winFunc);
    T *tmpArray = new T [n+1];
    int winnerPlayer;
    for (int i=1; i<=n; ++i)
    {
        winnerPlayer = wt.winner();
        tmpArray[i] = a[winnerPlayer];
        a[winnerPlayer] = MAX_VALUE;
        wt.replay(winnerPlayer, winFunc);
    }
    for (int i=1; i<=n; ++i)
    {
        a[i] = tmpArray[i];
    }
    delete [] tmpArray;

}

template<class T>
void TournamentSort2(T a[], int n, const int &MAX_VALUE);


void TournamentTreeTest()
{
    int numE = 113;
    const int MAX_VAL = numE * 4;

    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(0, MAX_VAL);

    int *testArray = new int[numE+1];
    std::cout<<"====Unsorted====="<<std::endl;

    for (int i=1; i<=numE; i++)
    {
        testArray[i] = uidn(newEngine);
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    //TournamentSort(testArray, numE, MAX_VAL);
    TournamentSort2(testArray, numE, MAX_VAL);

    std::cout<<"====sorted====="<<std::endl;
    for (int i=1; i<=numE; i++)
    {
        std::cout<<testArray[i]<<" ";
    }
    std::cout<<std::endl;

    delete [] testArray;
    
}

void FirstFitPack(int s[], int n, int c)
{
    // Use Max winner tree
    WinnerTree<int> wt(n);
    int *avail = new int [n+1];

    std::function<int(int *, int, int)> winFunc = [](int *a, int lc, int rc) -> int { return a[lc] >= a[rc] ? lc : rc; };

    for (int i=1; i<=n; ++i)
    {
        avail[i] = c;
    }
    wt.initialize(avail, n, winFunc);

    for (int i=1; i<=n; ++i)
    {
        // go from root to leaf
        int p = 2; // start from 2, root's left child
        while (p < n) // <= n-1 ensure it's internal node
        {
            // every iterate p is left child of a efficient tree at first
            int winp = wt.winner(p);
            if (avail[winp] < s[i]) // left unavail, go to right sibling
            {
                p++;
            }
            p *= 2;
        }

        int b;
        p /= 2; // current player's parent in m_tree
        if (p < n)
        {
            // p is tree parent, child is player
            b = wt.winner(p); // left or right
            //    p       p     |     p
            //  /  \    /  \    |   /  \
            //     b-1  b       | b-1   b
            if (b > 1 && avail[b-1] >= s[i])
            {
                // only right child will go into here
                // and become left
                // |     p
                // |   /  \
                // | b-1   b
                b--;
            }
            // else {
            // left child left unchanged
            //    p       p  
            //  /  \    /  \ 
            //     b-1  b    
            // }

        }
        else
            b = wt.winner(p/2); // when n is odd and left internal right external

        std::cout<<"Pack Obj "<<i<<" In Bin "<<b<<std::endl;
        avail[b] -= s[i];
        wt.replay(b, winFunc);
    }
}



void BinPackFirstFitTest()
{
    int nObj, cap;
    std::cout<< "Enter Object Num and bin capacity: " <<std::endl;

    std::cin>>nObj>>cap;
    if (nObj < 2)
    {
        std::cout<<"Two few" << std::endl;
        return;
    }
    int *sObj = new int [nObj+1];
    for (int i=1; i<=nObj; ++i)
    {
        std::cout<< "Enter space requirement of Obj : " <<i<<std::endl;
        std::cin>>sObj[i];
        if (sObj[i]>cap)
        {
            std::cout<<"Two much capacity" << std::endl;
            return;
        }

    }

    FirstFitPack(sObj, nObj, cap);
}





template<class T>
class LoserTree
{
private:
    int *m_tree; // tree internal node, all losers
    int m_winner; // winner
    T *m_player; // player
    int m_Maxsize;
    int m_size;
    int m_lowExt; // lowest level(level k+1) external node number
    int m_offset; // node numbers in level 1~k = 2^k - 1
    void play(int idxTree, int idxP1, int idxP2, std::function<int(T *, int, int)> winner);

public:
    LoserTree(int maxSize):m_Maxsize(maxSize),m_size(0)
    {
        // internal node are 2-degree node, external are leafs, no 1-degree nodes
        // n(2)+1 = n(0), n(i) is the number of i-degree node
        // m_player 1...MaxSize, Maxsize
        // m_tree   1...Maxsize-1, Maxsize-1
        m_tree = new int[m_Maxsize];
        m_winner = 0;
    }
    ~LoserTree() {delete [] m_tree;}
    void initialize(T a[], int size, std::function<int(T *, int, int)> winner);
    void replay(int i, std::function<int(T *, int, int)> winner);
    int winner() const { return (m_size) ? m_winner : 0; }
    int loser(int i) const { return (i< m_size) ? m_tree[i] : 0;}
};

template<class T>
void LoserTree<T>::initialize(T a[], int size, std::function<int(T *, int, int)> winner)
{
    if (size > m_Maxsize || size < 2)
    {
        throw BadInput();
    }

    m_player = a;
    m_size = size;

    // 1. make a winner tree

    // n = m_size : the leaf/external nodes of the tree
    // calculate s: the first internal node index in last level
    // floor(log2(n-1)) = m, n-1 is the internal size
    // log2(n-1) >= m,  2^m <= n-1
    // s= 2^m <= n-1
    int s;
    for (s=1; 2*s <= m_size-1; s <<= 1);

    // m_size - s : the number of internal node in last level
    // 2*s is the child of s and -1 is the size of all the nodes 
    // from 1 to level k-1
    m_lowExt = 2*(m_size - s);
    m_offset = 2*s - 1; 

    int nodeIdx;
    for(nodeIdx = 2; nodeIdx <= m_lowExt; nodeIdx += 2)
        play((nodeIdx+m_offset)/2, nodeIdx-1, nodeIdx, winner);

    // n is odd and there will be an external node 
    // whose left brother is internal node
    // and its position must be at n pos in tree 
    // and n-1 is the left child of its parent
    // final move to the next right child
    if (m_size % 2)
    {
        play(m_size/2, m_tree[m_size-1], m_lowExt+1, winner);
        nodeIdx = m_lowExt + 3; // + 2 is next and left, +3 is right
    }
    else
        nodeIdx = m_lowExt + 2; // +1 is next and left, +2 is right


    // nodeIdx - m_lowExt is the idx in the upper idx counter from the first 
    // element in the level
    for(; nodeIdx <= m_size; nodeIdx += 2)
        play( (nodeIdx-m_lowExt+ m_size-1)/2, nodeIdx-1, nodeIdx, winner);

    // 2. Use winner tree to construct loser tree
    m_winner = m_tree[1];
    for (int i=1; i<m_size-1; ++i)
    {
        // level order set loser
        if (i*2 < m_size - 1)
        {
            // two child is in Node
            m_tree[i] = m_tree[i] == m_tree[i*2] ? m_tree[i*2+1] : m_tree[i*2];
        }
        else if (i*2 == m_size - 1)
        {
            // one inNode other external node
            m_tree[i] = m_tree[i] == m_tree[i*2] ? m_lowExt+1 : m_tree[i*2];
        }
        else
        {
            int idxP1;
            if (i >= s) 
            {
                // last level inNode
                idxP1 = i*2 - m_offset;
            }
            else
            {
                idxP1 = i*2 - (m_size-1) + m_lowExt;
            }
            m_tree[i] = m_tree[i] == idxP1 ? idxP1+1 : idxP1;
        }

    }
}

template<class T>
void LoserTree<T>::replay(int i, std::function<int(T *, int, int)> winner)
{
    // i must be the old winner
    if (i != m_winner)
    {
        throw BadInput();
    }

    // parent is index of m_tree
    // lchild is index of m_player
    int parent, lhs, rhs;
    int winP;
    bool isLeft = true;
    
    if (i <= m_lowExt)
    {
        parent = (i + m_offset)/2;
    }
    else
    {
        parent = ((i - m_lowExt) + m_size - 1) / 2;
    }

    winP = i;
    while ( parent >= 1)
    {
        // keep stable
        if (m_tree[parent] < winP)
        {
            lhs = m_tree[parent];
            rhs = winP;
        }
        else
        {
            lhs = winP;
            rhs = m_tree[parent];
        }

        winP = winner(m_player, lhs, rhs);
        m_tree[parent] = winP == lhs ? rhs : lhs;

        parent /= 2;
    }
    
    m_winner = winP;

}

template<class T>
void LoserTree<T>::play(int idxTree, int idxP1, int idxP2, std::function<int(T *, int, int)> winner)
{
    // set play result
    m_tree[idxTree] = winner(m_player, idxP1, idxP2);

    while (idxTree > 1 && idxTree % 2)
    {
        m_tree[idxTree/2] = winner(m_player, m_tree[idxTree-1], m_tree[idxTree]);
        idxTree /= 2;
    }

}


//@param a[1...n]
template<class T>
void TournamentSort2(T a[], int n, const int &MAX_VALUE)
{
    LoserTree<T> lt(n);

    std::function<int(T *, int, int)> winFunc2 = [](T *a, int lc, int rc) -> int { return a[lc] <= a[rc] ? lc : rc; };
    lt.initialize(a, n, winFunc2);
    T *tmpArray = new T [n+1];
    int winnerPlayer;
    for (int i=1; i<=n; ++i)
    {
        winnerPlayer = lt.winner();
        tmpArray[i] = a[winnerPlayer];
        a[winnerPlayer] = MAX_VALUE;
        lt.replay(winnerPlayer, winFunc2);
    }
    for (int i=1; i<=n; ++i)
    {
        a[i] = tmpArray[i];
    }
    delete [] tmpArray;

}