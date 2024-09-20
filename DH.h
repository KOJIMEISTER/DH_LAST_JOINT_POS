#pragma once
#include <vector>

class DH
{
	class Matrix4D;
	struct Pos;
	struct Joint
	{
		float Theta{ 0.f };
		float A{ 0.f };
		float D{ 0.f };
		float Alpha{ 0.f };
	};

	std::vector<Joint> Joints;
	void MakeDHMatrix(const Joint&, Matrix4D& ToMat) const;
	Pos GetPosFromMatrix(const Matrix4D&) const;

public:

	struct Pos
	{
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
	};

	DH();
	~DH() = default;

	[[nodiscard]] Pos CalcLastJointPos(const std::vector<float>& Thetas);

private:

	class Matrix4D
	{
		float** Matrix{ nullptr };
		void Clear();
	public:
		Matrix4D();
		Matrix4D(const Matrix4D&) = delete;
		Matrix4D(Matrix4D&&) noexcept;
		~Matrix4D();
		Matrix4D& operator=(const Matrix4D&) = delete;
		Matrix4D& operator=(Matrix4D&&) noexcept;
		static void Multiply(const Matrix4D& FirMat, const Matrix4D& SecMat, Matrix4D&ToMat);
		static void Swap(Matrix4D& FirMat, Matrix4D& SecMat);
		[[nodiscard]] const float& At(const int& Row, const int& Column) const;
		void Set(const int& Row, const int& Column, const float& Value);
	};

};

