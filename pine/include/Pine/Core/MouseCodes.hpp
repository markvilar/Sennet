#pragma once

namespace Pine
{

typedef enum class MouseCode : uint16_t
{
    // From glfw3.h
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,

    Buttonlast = Button7,
    Buttonleft = Button0,
    Buttonright = Button1,
    Buttonmiddle = Button2
} Mouse;

inline std::ostream& operator<<(std::ostream& os, const MouseCode mouseCode)
{
    return os << static_cast<int32_t>(mouseCode);
}

} // namespace Pine

#define PINE_MOUSE_BUTTON_0 ::Pine::Mouse::Button0
#define PINE_MOUSE_BUTTON_1 ::Pine::Mouse::Button1
#define PINE_MOUSE_BUTTON_2 ::Pine::Mouse::Button2
#define PINE_MOUSE_BUTTON_3 ::Pine::Mouse::Button3
#define PINE_MOUSE_BUTTON_4 ::Pine::Mouse::Button4
#define PINE_MOUSE_BUTTON_5 ::Pine::Mouse::Button5
#define PINE_MOUSE_BUTTON_6 ::Pine::Mouse::Button6
#define PINE_MOUSE_BUTTON_7 ::Pine::Mouse::Button7
#define PINE_MOUSE_BUTTON_LAST ::Pine::Mouse::ButtonLast
#define PINE_MOUSE_BUTTON_LEFT ::Pine::Mouse::ButtonLeft
#define PINE_MOUSE_BUTTON_RIGHT ::Pine::Mouse::ButtonRight
#define PINE_MOUSE_BUTTON_MIDDLE ::Pine::Mouse::ButtonMiddle
