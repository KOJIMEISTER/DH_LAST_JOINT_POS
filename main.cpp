#include "DH.h"

int main()
{
	DH DHA{};
	DH::Pos XYZ = DHA.CalcLastJointPos({ 15.f, -50.f, -60.f, 95.f, 50.f, 0.f });
	return 0;
}