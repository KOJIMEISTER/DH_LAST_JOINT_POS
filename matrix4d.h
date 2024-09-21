#ifndef MATRIX4D_H
#define MATRIX4D_H

class Matrix4D
{
    float** Matrix{ nullptr };

public:
    Matrix4D();
    ~Matrix4D();

    static void Multiply(const Matrix4D& FirMat, const Matrix4D& SecMat, Matrix4D&ToMat);
    static void Swap(Matrix4D& FirMat, Matrix4D& SecMat);

    [[nodiscard]] const float& At(const int& Row, const int& Column) const;
    void Set(const int& Row, const int& Column, const float& Value);

    Matrix4D(const Matrix4D&) = delete;
    Matrix4D& operator=(const Matrix4D&) = delete;
};

#endif // MATRIX4D_H
