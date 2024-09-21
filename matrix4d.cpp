#include "matrix4d.h"

Matrix4D::Matrix4D()
{
    Matrix = new float* [4];
    for (int i = 0; i < 4; ++i)
    {
        Matrix[i] = new float[4];
    }
}

Matrix4D::~Matrix4D()
{
    if (Matrix)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (Matrix[i])
            {
                delete[] Matrix[i];
            }
        }
        delete[] Matrix;
    }
}

void Matrix4D::Multiply(const Matrix4D& FirMat, const Matrix4D& SecMat, Matrix4D& ToMat)
{
    float Sum = 0.f;
    for (int Row = 0; Row < 4; ++Row)
    {
        for (int Column = 0; Column < 4; ++Column)
        {
            Sum = 0.f;
            for (int Index = 0; Index < 4; ++Index)
            {
                Sum += FirMat.At(Row, Index) * SecMat.At(Index, Column);
            }
            ToMat.Set(Row, Column, Sum);
        }
    }
}

void Matrix4D::Swap(Matrix4D& FirMat, Matrix4D& SecMat)
{
    auto Ptr = FirMat.Matrix;
    FirMat.Matrix = SecMat.Matrix;
    SecMat.Matrix = Ptr;
}

const float& Matrix4D::At(const int& Row, const int& Column) const
{
    return Matrix[Row][Column];
}

void Matrix4D::Set(const int& Row, const int& Column, const float& Value)
{
    Matrix[Row][Column] = Value;
}
