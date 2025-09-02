#include "pch.h"
#include "matrix.h"

namespace Matrix
{
    Sparse::Sparse(int sizeCol, int sizeRow, std::string mvalue) : count(0),rowSize(sizeRow),colSize(sizeCol)
    {
        size = sizeCol * sizeRow;
        data = new Element[size];

        this->mvalue = mvalue;
        
        for (int i = 0; i < size; i++)
        {
            data[i].x = data[i].y = DEFAULT_POINT;
            data[i].val = mvalue;
        }       
    }
    Sparse::~Sparse()
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
        }
    }
    const std::string& Sparse::Get(int x, int y) const
    {
        if (!(x >= 0 && x < rowSize && y >= 0 && y < colSize)) return mvalue; // if x,y are outside the matrix

        for (int i = 0; i < size; i++)
            if (data[i].x == x && data[i].y == y)
                return data[i].val;
        return mvalue; // if element isn't found
    }
    POINT Sparse::Get(const std::string& val) const
    {
        POINT ret = { DEFAULT_POINT,DEFAULT_POINT };
        for (int i = 0; i < count; i++)
        {
            if (data[i].val == val)
            {
                ret.x = data[i].x;
                ret.y = data[i].y;
                break;
            }
        }
        return ret;
    }
    bool Sparse::Set(int x, int y, const std::string& val)
    {
        if (!(x >= 0 && x < rowSize && y >= 0 && y < colSize)) return false; // if x,y are outside the matrix

        for (int i = 0; i < count;i++) // first check if elemnt at x,y already exists
        {
            if (data[i].x == x && data[i].y == y)
            {
                data[i].val = val;
                return true;
            }
        }
        if (count < size)
        {
            data[count].x = x;
            data[count].y = y;
            data[count++].val = val;
            return true;
        }
        return false;
    }
    void Sparse::Print(int size1, int size2) const
    {
        for (int i = 0; i < size1; i++)
        {
            for (int j = 0; j < size2; j++)
            {
                std::cout << Get(i, j) << ' '; // print the whole matrix
            }
            std::cout << "\n";
        }
    }
}