#include "DH.h"
#include <iostream>

int main()
{
	DH DHA{};
	DH::Pos XYZ = DHA.CalcLastJointPos({ 15.f, -50.f, -60.f, 95.f, 50.f, 0.f });
    std::cout << XYZ.x << " " << XYZ.y << " " << XYZ.z;
	return 0;
}
