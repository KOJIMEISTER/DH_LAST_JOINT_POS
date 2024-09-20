#include "DH.h"

//#define JOINT_POS_ECHO

#ifdef JOINT_POS_ECHO
#include <iostream>
#endif // JOINT_POS_ECHO

static constexpr float PI = 3.14159f;

// Матрица перехода Денавита-Хартенберга: wikipedia.org/wiki/Denavit–Hartenberg_parameters
void DH::MakeDHMatrix(const Joint& CurJoint, Matrix4D& ToMat) const
{
	ToMat.Set(0, 0, cos(CurJoint.Theta));
	ToMat.Set(0, 1, -sin(CurJoint.Theta) * cos(CurJoint.Alpha));
	ToMat.Set(0, 2, sin(CurJoint.Theta) * sin(CurJoint.Alpha));
	ToMat.Set(0, 3, CurJoint.A * cos(CurJoint.Theta));

	ToMat.Set(1, 0, sin(CurJoint.Theta));
	ToMat.Set(1, 1, cos(CurJoint.Theta) * cos(CurJoint.Alpha));
	ToMat.Set(1, 2, -cos(CurJoint.Theta) * sin(CurJoint.Alpha));
	ToMat.Set(1, 3, CurJoint.A * sin(CurJoint.Theta));

	ToMat.Set(2, 0, 0.f);
	ToMat.Set(2, 1, sin(CurJoint.Alpha));
	ToMat.Set(2, 2, cos(CurJoint.Alpha));
	ToMat.Set(2, 3, CurJoint.D);

	ToMat.Set(3, 0, 0);
	ToMat.Set(3, 1, 0);
	ToMat.Set(3, 2, 0);
	ToMat.Set(3, 3, 1);
}

DH::Pos DH::GetPosFromMatrix(const Matrix4D& Mat) const
{
	return Pos{ Mat.At(0, 3), Mat.At(1, 3), Mat.At(2, 3) };
}

DH::DH()
{
	// TODO: Добавить загрузку параметров DH через конфиг JSON
	Joints.reserve(6);
	Joints.emplace_back(0.f, 0.f, 0.213f, PI / 2.f);
	Joints.emplace_back(0.f, -0.8f, 0.193f, 0.f);
	Joints.emplace_back(0.f, -0.59f, -0.16f, 0.f);
	Joints.emplace_back(0.f, 0.f, 0.25f, PI / 2.f);
	Joints.emplace_back(0.f, 0.f, 0.28f, -PI / 2.f);
	Joints.emplace_back(0.f, 0.f, 0.25f, 0.f);
}

DH::Pos DH::CalcLastJointPos(const std::vector<float>& Thetas)
{
	// Чтобы не вызывать new много раз при вызове CalcLastJointPos переиспользуем матрицы,
	// Все равно старые значения перезаписываются
	static Matrix4D BuffMat, BuffMatTmp, BuffMatJoint;
	size_t JointsSize = Joints.size();
	size_t ThetasSize = Thetas.size();
	if (JointsSize == 0)
	{
		return Pos{};
	}
	for (size_t i = 0; i < ThetasSize and i < JointsSize; ++i)
	{
		Joints[i].Theta = Thetas[i];
	}
	// Выносим из цикла, чтобы избежать умножение на первом проходе
	MakeDHMatrix(Joints.front(), BuffMat);
#ifdef JOINT_POS_ECHO
	{
		const Pos JointPos{ GetPosFromMatrix(BuffMat) };
		std::cout << JointPos.x << " " << JointPos.y << " " << JointPos.z << std::endl;
	}
#endif // JOINT_POS_ECHO
	for (size_t i = 1; i < JointsSize; ++i)
	{
		MakeDHMatrix(Joints[i], BuffMatJoint);
		DH::Matrix4D::Multiply(BuffMat, BuffMatJoint, BuffMatTmp);
		// Чтобы присвоить произведение без выделения памяти
		DH::Matrix4D::Swap(BuffMat, BuffMatTmp);
#ifdef JOINT_POS_ECHO
		{
			const Pos JointPos{ GetPosFromMatrix(BuffMat) };
			std::cout << JointPos.x << " " << JointPos.y << " " << JointPos.z << std::endl;
		}
#endif // JOINT_POS_ECHO
	}
	return GetPosFromMatrix(BuffMat);
}

void DH::Matrix4D::Clear()
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

DH::Matrix4D::Matrix4D()
{
	Matrix = new float* [4];
	for (int i = 0; i < 4; ++i)
	{
		Matrix[i] = new float[4];
	}
}

DH::Matrix4D::Matrix4D(Matrix4D&& Mat) noexcept
{
	Matrix = Mat.Matrix;
	Mat.Matrix = nullptr;
}

DH::Matrix4D::~Matrix4D()
{
	Clear();
}

DH::Matrix4D& DH::Matrix4D::operator=(Matrix4D&& Mat) noexcept
{
	if (&Mat == this)
	{
		return *this;
	}
	Clear();
	Matrix = Mat.Matrix;
	Mat.Matrix = nullptr;
	return *this;
}

void DH::Matrix4D::Multiply(const Matrix4D& FirMat, const Matrix4D& SecMat, Matrix4D& ToMat)
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

void DH::Matrix4D::Swap(Matrix4D& FirMat, Matrix4D& SecMat)
{
	auto Ptr = FirMat.Matrix;
	FirMat.Matrix = SecMat.Matrix;
	SecMat.Matrix = Ptr;
}

const float& DH::Matrix4D::At(const int& Row, const int& Column) const
{
	return Matrix[Row][Column];
}

void DH::Matrix4D::Set(const int& Row, const int& Column, const float& Value)
{
	Matrix[Row][Column] = Value;
}
