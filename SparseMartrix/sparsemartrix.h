// 此程序的关键就在于转置时的巧妙技巧
// 以及理解了Move Constructor的用处


#include "yinexception.h"

template<class T>
class SparseMartrix;


template<class T>
class Term
{
    friend class SparseMartrix<T>;
    friend std::istream& operator>>(std::istream& ins, SparseMartrix<T>& rhs);
    friend std::ostream& operator<<(std::ostream& ous, const SparseMartrix<T>& rhs);
    friend SparseMartrix<T> operator+(const SparseMartrix<T>& lhs, const SparseMartrix<T>& rhs);
    int row;
    int col;
    T value;
};


template<class T>
class SparseMartrix
{
public:
    // friend operator overload function
    // 这里就是关键，友元函数直接再类内定义，而且必须再类内定义
    friend std::istream& operator>>(std::istream& ins, SparseMartrix<T>& rhs)
    {
        std::cout<<"Enter number of rows, cols, and terms" << std::endl;
        ins >> rhs.rows >> rhs.cols >> rhs.terms;
        if (rhs.terms > rhs.maxTerms)
        {
            throw NoMem();
        }

        for (int i=0; i<rhs.terms; ++i)
        {
            std::cout<<"Enter row, col, and value of term" <<(i+1) << std::endl;
            ins >> rhs.a[i].row >> rhs.a[i].col >> rhs.a[i].value;
        }
        return ins;
    }
    friend std::ostream& operator<<(std::ostream& ous, const SparseMartrix<T>& rhs)
    {
        ous << "Rows:" <<rhs.rows << ", Cols:" << rhs.cols <<std::endl;
        for(int i=0; i<rhs.terms; ++i)
        {
            ous<<"Row:"<<rhs.a[i].row
                <<", Col:"<<rhs.a[i].col
                <<", Value:"<<rhs.a[i].value
                <<std::endl;
        }
        return ous;
    }

    // here will call copy constructor and return by value, however
    // the value of the pointer will be copied but it will be deleted by the 
    // original object
    // so this time we need  MOVE Constructor
    // 也就是掠夺原始obj的资源，但不用复制，同时必须保证，原始obj在销毁时完全无害
    // 即在原始obj中，将其要释放的资源置空，防止被销毁，并复制指针，完成资源的过度
    friend SparseMartrix<T> operator+(const SparseMartrix<T>& lhs, const SparseMartrix<T>& rhs)
    {
        // NRV
        // allocate enough space

        if(lhs.rows != rhs.rows || lhs.cols != rhs.cols)
            throw SizeNotMatch();

        SparseMartrix<T> ans((lhs.terms+rhs.terms)<<1);
        ans.rows = lhs.rows;
        ans.cols = lhs.rows;
        ans.terms = 0;

        int cursorL=0,cursorR=0;
        while(cursorL < lhs.terms && cursorR < rhs.terms)
        {
            // L and R are all not empty
            int idxL = lhs.a[cursorL].row * lhs.cols + lhs.a[cursorL].col;
            int idxR = rhs.a[cursorR].row * rhs.cols + rhs.a[cursorR].col;

            if(idxL < idxR)
                ans.Append(lhs.a[cursorL++]);
            else if(idxL > idxR)
                ans.Append(rhs.a[cursorR++]);
            else // ==
            {
                if (lhs.a[cursorL].value + rhs.a[cursorR].value != 0)
                {
                    Term<T> tmpT;
                    tmpT.row = lhs.a[cursorL].row;
                    tmpT.col = lhs.a[cursorL].col;
                    tmpT.value = lhs.a[cursorL].value + rhs.a[cursorR].value;
                    ans.Append(tmpT);
                }
                cursorL ++;
                cursorR ++;
            }
        }

        // a or b 
        while(cursorL < lhs.terms)
        {
            // a. check L left elements
            ans.Append(lhs.a[cursorL++]);

        }
        while(cursorR <rhs.terms)
        {
            // b. check R left elements
            ans.Append(rhs.a[cursorR++]);
        }

        return ans;

        // here will call copy constructor and 
    }
         
    // public function
    SparseMartrix(int maxTerms = 20);

    // Important Move copy constructor
    SparseMartrix(SparseMartrix &&rhs)
    {
        rows = rhs.rows;
        cols = rhs.cols;
        terms = rhs.terms;
        maxTerms = rhs.maxTerms;
        a = rhs.a;
        rhs.a = nullptr;
    }

    ~SparseMartrix() { delete [] a;}
    void Transpose(SparseMartrix<T> &b) const;

private:
    void Append(const Term<T>& t);

    int rows, cols;
    int terms;
    Term<T> *a;
    int maxTerms;
};



template<class T>
SparseMartrix<T>::SparseMartrix(int maxTerms /* = 20 */):maxTerms(maxTerms)
{
    // if maxTerms < 1
    a = new Term<T> [maxTerms];
    terms = 0;
    rows=cols=0;
}

template<class T>
void SparseMartrix<T>::Transpose(SparseMartrix<T> &b) const
{
    b.rows = cols;
    b.cols = rows;
    b.terms = terms;

    int *colSignificant = new int [cols + 1];
    int *firstSigificantInCol = new int [cols + 1];

    for(int i=1; i<= cols; ++i)
        colSignificant[i] = 0;

    for(int i=0; i<terms; ++i)
        colSignificant[a[i].col]++;

    firstSigificantInCol[1] = 0; // first col first nonzero must at 0 index
    for(int i=2; i<=cols; ++i)
        firstSigificantInCol[i] = firstSigificantInCol[i-1] + colSignificant[i-1];

    for(int i=0; i<terms; ++i)
    {
        int j = firstSigificantInCol[a[i].col]++ ; // current row first index
        b.a[j].row = a[i].col;
        b.a[j].col = a[i].row;
        b.a[j].value = a[i].value;
    }

}

template<class T>
void SparseMartrix<T>::Append(const Term<T>& t)
{
    if (terms+1 > maxTerms)
    {
        throw NoMem();
    }

    a[terms++] = t;
}
