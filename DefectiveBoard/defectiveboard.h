namespace Yincpp
{
    // @param Board[][] 0...size^2, value is the tile idx[0...(size^2-1)/3]
    // @param size must 2^k
    // @param idxTile is 0 first
    void TileBoard(int curRow, int curCol, int defectRow, int defectCol,
                    int size, int **Board, int &idxTile)
    {
        if (size == 1)
        {
            return;
        }
        int t = idxTile++;
        int splitSize = size/2;


        // into 3 else block and 1 if block,
        // that is to say, set three t(three block using tile t)

        // defective in top-left split block
        if (defectRow < curRow + splitSize && defectCol < curCol + splitSize)
        {
            TileBoard(curRow, curCol, defectRow, defectCol, splitSize, Board, idxTile);
        }
        else
        {
            Board[curRow+splitSize-1][curCol+splitSize-1] = t;
            TileBoard(curRow, curCol, curRow+splitSize-1, curCol+splitSize-1, splitSize, Board, idxTile);
        }

        // defective in top-right split block
        if (defectRow < curRow + splitSize && defectCol >= curCol + splitSize)
        {
            TileBoard(curRow, curCol+splitSize, defectRow, defectCol, splitSize, Board, idxTile);
        }
        else
        {
            Board[curRow+splitSize-1][curCol+splitSize] = t;
            TileBoard(curRow, curCol+splitSize, curRow+splitSize-1, curCol+splitSize, splitSize, Board, idxTile);
        }

        // defective in top-left split block
        if (defectRow >= curRow + splitSize && defectCol < curCol + splitSize)
        {
            TileBoard(curRow+splitSize, curCol, defectRow, defectCol, splitSize, Board, idxTile);
        }
        else
        {
            Board[curRow+splitSize][curCol+splitSize-1] = t;
            TileBoard(curRow+splitSize, curCol, curRow+splitSize, curCol+splitSize-1, splitSize, Board, idxTile);
        }

        // defective in top-left split block
        if (defectRow >= curRow + splitSize && defectCol >= curCol + splitSize)
        {
            TileBoard(curRow+splitSize, curCol+splitSize, defectRow, defectCol, splitSize, Board, idxTile);
        }
        else
        {
            Board[curRow+splitSize][curCol+splitSize] = t;
            TileBoard(curRow+splitSize, curCol+splitSize, curRow+splitSize, curCol+splitSize, splitSize, Board, idxTile);
        }
    }


    void DefectiveBoardTest()
    {
        int kOfSize = 2;
        
        int size = 1;
        for (int i=0; i<kOfSize; ++i)
        {
            size *= 2;
        }

        int idxTile = 0;
        int **Board = new int* [size];
        for (int i=0; i<size; ++i)
        {
            Board[i] = new int[size];
            for (int j=0; j<size; ++j)
            {
                Board[i][j] = -1;
            }
        }

        int drow = 3;
        int dcol = 3;

        

        TileBoard(0, 0, drow, dcol, size, Board, idxTile);
        int bitLen = 1, upperBound = 10;
        while (idxTile >= upperBound)
        {
            bitLen++;
            upperBound *= 10;
        }
        std::cout<<"2^"<<kOfSize<<" * 2^"<<kOfSize<<"Board, "
            <<"Defective at ("<<drow<<", "<<dcol<<")"<<std::endl;
        std::cout<<"......"<<idxTile<<" Tile-----Output:"<<std::endl;
        for (int i=0; i<size; ++i)
        {
            for (int j=0; j<size; ++j)
            {
                std::cout.width(bitLen+1);
                std::cout<<Board[i][j];
                std::cout<<(j == size-1 ? "\n":" ");
            }
        }

        for (int i=0; i<size; ++i)
        {
            delete [] Board[i];
        }
        delete [] Board;
    }
}