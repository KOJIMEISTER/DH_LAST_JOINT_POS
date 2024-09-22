#include <math.h>
#include "DH.h"
#include "matrix4d.h"

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

DH::Pos DH::CalcLastJointPos(const std::vector<float>& Thetas)
{
    // Чтобы не вызывать new много раз при вызове CalcLastJointPos переиспользуем матрицы,
    // Все равно старые значения перезаписываются
    static Matrix4D BuffMat, BuffMatTmp, BuffMatJoint;
    std::scoped_lock JointsLock(MutexJoints);
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
        Matrix4D::Multiply(BuffMat, BuffMatJoint, BuffMatTmp);
        // Чтобы присвоить произведение без выделения памяти
        Matrix4D::Swap(BuffMat, BuffMatTmp);
#ifdef JOINT_POS_ECHO
        {
            const Pos JointPos{ GetPosFromMatrix(BuffMat) };
            std::cout << JointPos.x << " " << JointPos.y << " " << JointPos.z << std::endl;
        }
#endif // JOINT_POS_ECHO
    }
    return GetPosFromMatrix(BuffMat);
}

void DH::SetJoints(std::vector<Joint>& Joints)
{
    std::scoped_lock JointsLock(MutexJoints);
    this->Joints = std::move(Joints);
}
