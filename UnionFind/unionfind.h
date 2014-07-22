
class UnionFind
{
    bool *m_root;
    int *m_parent;
    int m_num;
    void Union(int i, int j);

public:
    UnionFind(int n);
    ~UnionFind() { delete [] m_root; delete [] m_parent;}
    int Find(int e);
    void Combine(int ni, int nj);
    bool connected(int ni, int nj)
    { return Find(ni) == Find(nj); }
    static void unitTest();
};