namespace Yincpp
{
    //@param r[1:n] Ri stand for ith matrix's width
    void MatrixChain(int r[], int n, int **c, int **kay)
    {
        for (int i=1; i<n; ++i)
        {
            c[i][i] = 0;
            c[i][i+1] = r[i]*r[i+1]*r[i+2];
            kay[i][i+1] = i;
        }
        c[n][n] = 0;

        for (int s=2; 1 + s <= n; s++)
        {
            for (int i=1; i+s <= n; i++)
            {
                c[i][i+s] = c[i][i] + c[i+1][i+s] + r[i]*r[i+1]*r[i+s+1];
                kay[i][i+s] = i;
                for (int j=i+1; j<i+s; j++)
                {
                    int curC = c[i][j] + c[j+1][i+s] + r[i]*r[j+1]*r[i+s+1];
                    if (curC < c[i][i+s])
                    {
                        c[i][i+s] = curC;
                        kay[i][i+s] = j;
                    }
                }
            }
        }
    }

    void traceMatrixChain(int **kay, int i, int j)
    {
        // post order
        if (i == j) return;
        //left
        traceMatrixChain(kay, i, kay[i][j]);
        traceMatrixChain(kay, kay[i][j]+1, j);
        std::cout<<"Multiply M "<<i<<", "<<kay[i][j];
        std::cout<<" and M"<<(kay[i][j]+1)<<", "<<j;
        std::cout<<" = M "<<i<<", "<<j<<std::endl;
    }

    void matrixChainTest()
    {
        const int n=5;
        int r[n+2] = {0, 10, 5, 1, 10, 2, 10};
        int **c;
        int **kay;
        Yincpp::make2DArray(c, n+1, n+1);
        Yincpp::make2DArray(kay, n+1, n+1);
        MatrixChain(r, n, c, kay);
        traceMatrixChain(kay, 1, n);
        std::cout<<"Time: "<<c[1][n]<<std::endl;
        Yincpp::delete2DArray(c, n+1);
        Yincpp::delete2DArray(kay, n+1);

    }



}