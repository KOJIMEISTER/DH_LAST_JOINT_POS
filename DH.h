#ifndef DH_H
#define DH_H
#include <vector>
#include <mutex>

class Matrix4D;

class DH
{
public:
    struct Pos
    {
        float x{ 0.f };
        float y{ 0.f };
        float z{ 0.f };
    };
    struct Joint
    {
        float Theta{ 0.f };
        float A{ 0.f };
        float D{ 0.f };
        float Alpha{ 0.f };
    };
    [[nodiscard]] Pos CalcLastJointPos(const std::vector<float>& Thetas);
    void SetJoints(std::vector<Joint>& Joints);

private:
    void MakeDHMatrix(const Joint&, Matrix4D& ToMat) const;
    Pos GetPosFromMatrix(const Matrix4D&) const;
    std::mutex MutexJoints{};
    std::vector<Joint> Joints{};
};
#endif // DH_H
