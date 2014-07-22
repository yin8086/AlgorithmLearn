#include "heap.h"
#include "../BinaryTree/BinaryTree.h"

template <class T>
struct Huffman
{

    BinaryTree<int> m_tree;
    T m_weight;

public:
    operator T() const { return m_weight; } //must be const
    Huffman(){}
    Huffman<T>& operator=(Huffman<T> &&rhs)
    {
        m_tree = std::move(rhs.m_tree);
        m_weight = rhs.m_weight;
        return *this;
    }
    Huffman(Huffman<T> &&rhs)
    {
        m_tree = std::move(rhs.m_tree);
        m_weight = rhs.m_weight;
    }
};

// @param a[] weight array
// @param n   array size
template<class T>
BinaryTree<int> HuffmanTree(T a[], int n)
{
    Huffman<T> *hmt = new Huffman<T> [n+1];
    BinaryTree<int> z,zero;
    for (int i=1; i<=n; ++i)
    {
        // every iteration m_root in z will be rewritten, and copied to hmt[i]
        // which make the tree only owned to hmt
        z.makeTree(i, zero, zero); //external node value > 0, stands for code
        hmt[i].m_tree = std::move(z);
        hmt[i].m_weight = a[i];
    }

    MinHeap<Huffman<T> > mh(1);
    mh.initialize(hmt, n, n);

    Huffman<T> x,y;
    // every time -2 and +1, it is -1,so is n-1 iteration
    for(int i=1; i <= n-1; i++)
    {
        mh.deleteMin(x);
        mh.deleteMin(y);
        z.makeTree(0, x.m_tree, y.m_tree); // tree z
        x.m_weight = x.m_weight + y.m_weight;
        x.m_tree = std::move(z); // owned to x
        mh.insert(x);
    }

    mh.deleteMin(x);
    mh.deactive();
    delete [] hmt;
    return std::move(x.m_tree);
}


// The most important thing is use move constructor to 
// get the m_root pointer and make the src = 0
// or try to use auto_ptr
void HuffUnitTest()
{
    int numE = 5;
    std::default_random_engine newEngine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uidn(1, numE * 4);

    int *testFreq = new int[numE+1];
    for (int i=1; i<=numE; ++i)
    {
        testFreq[i] = uidn(newEngine);
        std::cout<<"("<<i<<", "<<testFreq[i]<<") ";
    }

    BinaryTree<int> ftree = HuffmanTree(testFreq, numE);
    std::cout<<std::endl;
    ftree.levelOrder([](BinaryTreeNode<int> *p){  
        if (p->m_data > 0)
        {
            std::cout<<p->m_data<<" ";
        }
        });


}