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
    
    std::string random_Str(std::default_random_engine &eng, std::uniform_int_distribution<> &uid)
    {
        int strSize = uid(eng);
        std::string strR(strSize, 0);
        std::generate_n(strR.begin(), strSize, 
            [&eng]() 
            {
                    const char charset[] =
                        /*"0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"*/
                        "abcdefghijklmnopqrstuvwxyz";
                    // delete \0
                    std::uniform_int_distribution<> uid(0, sizeof(charset)-2);
                    return charset[ uid(eng) ];

             });
        return strR;
    }

}