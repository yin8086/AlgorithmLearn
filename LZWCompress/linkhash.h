#include "sortedchain.h"

template<class E, class K>
class LinkHash
{
    int m_D; // Divisor
    SortedChain<E, K> *ht;
public:

    LinkHash(int d = 11):m_D(d) 
    { ht = new SortedChain<E, K>[d]; }
    ~LinkHash()
    {
        delete [] ht;
    }
    bool Search(const K& key, E& eValue) const 
    {
        return ht[key % m_D].search(key, eValue);
    }
    LinkHash<E,K>& Insert(const E& eValue)
    {
        K key = eValue;
        ht[key % m_D].distinctInsert(eValue);
        return *this;
    }
    LinkHash<E,K>& Delete(const K& key, E& eValue)
    {
        ht[key % m_D].remove(key, eValue);
        return *this;
    }
    void clear()
    {
        // clear all the elements for new operation
        for (int i=0; i<m_D; ++i)
        {
            ht[i].clear();
        }
    }
    void unitTest();
};

template<class E, class K>
void LinkHash<E,K>::unitTest()
{
    std::default_random_engine engine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uid(0, 500);

    for (int i=0; i<20; ++i)
    {
        Insert(E(i, uid(engine)));
    }
    std::uniform_int_distribution<int> uid2(0, 30);

    E tmpE;
    for(int i=0; i<5; ++i)
    {
        K key = uid2(engine);
        if(Search(key, tmpE))
        {
            std::cout<<"Search key "<<key<<", Value="<<tmpE<<std::endl;
        }
        else
        {
            std::cout<<"Search key "<<key<<", not found."<<std::endl;
        }
    }
    
}