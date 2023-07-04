#include <pine/pine.hpp>

int test_vec2()
{
    const auto v1 = pine::Vec2(0.1f, 0.2f);
    const auto v2 = pine::Vec2(0.1f, 0.2f);
    const auto v3 = v1 + v2;
    const auto v4 = v1 * v2;
    return 0;
}

int test_vec3()
{
    const auto v1 = pine::Vec3(0.1f, 0.2f, 0.3f);
    const auto v2 = pine::Vec3(0.1f, 0.2f, 0.3f);
    const auto v3 = v1 + v2;
    const auto v4 = v1 * v2;
    return 0;
}

int test_vec4()
{
    const auto v1 = pine::Vec4(0.1f, 0.2f, 0.3f, 0.4f);
    const auto v2 = pine::Vec4(0.1f, 0.2f, 0.3f, 0.4f);
    const auto v3 = v1 + v2;
    const auto v4 = v1 * v2;
    return 0;
}

int test_quat()
{
    const auto q1 = pine::Quat(0.1f, 0.2f, 0.3f, 0.4f);
    const auto q2 = pine::Quat(0.1f, 0.2f, 0.3f, 0.4f);
    const auto q3 = q1 + q2;
    const auto q4 = q1 * q2;
    return 0;
}

int test_misc()
{
    const auto rad = pine::radians(90.0f);
    return 0;
}

int main(int argc, char** argv)
{
    int error = 0;
    error += test_vec2();
    error += test_vec3();
    error += test_vec4();
    error += test_quat();
    error += test_misc();

    return error;
}
