namespace Yincpp
{
    template<class T>
    void make2DArray(T ** &x, int rows, int cols)
    {
        x = new T* [rows];
        for (int i=0; i<rows; ++i)
        {
            x[i] = new T [cols];
        }
    }

    template<class T>
    void delete2DArray(T ** & x, int rows)
    {
        for (int i=0; i<rows; ++i)
        {
            delete [] x[i];
        }

        delete [] x;
        x = 0;
    }
}