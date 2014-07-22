#include <algorithm>
#include <iostream>
#include "unionfind.h"
#include "yinexception.h"

UnionFind::UnionFind(int n):m_num(n)
{
    m_root = new bool[n];
    m_parent = new int[n];
    // root = true and parent = num of tree
    // root = false and parent is parent
    for (int i=0; i < n; ++i)
    {
        m_root[i] = true;
        m_parent[i] = 1; // root is true and this value is num of nodes
    }
}

int UnionFind::Find(int e)
{
    if (e >= m_num)
    {
        throw OutOfBounds();
    }

    // Add path compaction
    int j = e;
    while(!m_root[j])
        j = m_parent[j];

    // j is root, op is old parent
    int op;
    while(e != j)
    {
        op = m_parent[e];
        m_parent[e] = j;
        e = op;
    }

    return j;
}

// @param i root i
// @param j root j
void UnionFind::Union(int i, int j)
{
    // i have small length
    // Use weight method and parent tree method
    if (m_parent[j] < m_parent[i])
    {
        std::swap(i, j);
    }
    
    // ensure m_parent[i] <= m_parent[j]
    m_parent[j] += m_parent[i]; // weight fix
    m_root[i] = false;
    m_parent[i] = j;
}

void UnionFind::Combine(int ni, int nj)
{
    if (ni >= m_num || nj >= m_num)
    {
        throw OutOfBounds();
    }
    int i = Find(ni);
    int j = Find(nj);

    if (i != j)
    {
        Union(i, j);
    }
}


void UnionFind::unitTest()
{
    UnionFind uf(10);

    /*
    uf.Combine(4, 3);
    uf.Combine(3, 8);
    uf.Combine(6, 5);
    uf.Combine(9, 4);
    uf.Combine(2, 1);
    uf.Combine(5, 0);
    uf.Combine(7, 2);
    uf.Combine(6, 1);
    */

    uf.Combine(0, 1);
    uf.Combine(2, 3);
    uf.Combine(4, 5);
    uf.Combine(6, 7);
    uf.Combine(0, 2);
    uf.Combine(4, 6);
    uf.Combine(0, 4);

    for(int i=0; i<10; ++i)
    {
        std::cout<<"Node "<<i<<", Union "<<uf.Find(i)<<std::endl;
    }
}
