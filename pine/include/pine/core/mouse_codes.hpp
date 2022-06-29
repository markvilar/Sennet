#pragma once

namespace pine
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

} // namespace pine

#define PINE_MOUSE_BUTTON_0 ::pine::Mouse::Button0
#define PINE_MOUSE_BUTTON_1 ::pine::Mouse::Button1
#define PINE_MOUSE_BUTTON_2 ::pine::Mouse::Button2
#define PINE_MOUSE_BUTTON_3 ::pine::Mouse::Button3
#define PINE_MOUSE_BUTTON_4 ::pine::Mouse::Button4
#define PINE_MOUSE_BUTTON_5 ::pine::Mouse::Button5
#define PINE_MOUSE_BUTTON_6 ::pine::Mouse::Button6
#define PINE_MOUSE_BUTTON_7 ::pine::Mouse::Button7
#define PINE_MOUSE_BUTTON_LAST ::pine::Mouse::ButtonLast
#define PINE_MOUSE_BUTTON_LEFT ::pine::Mouse::ButtonLeft
#define PINE_MOUSE_BUTTON_RIGHT ::pine::Mouse::ButtonRight
#define PINE_MOUSE_BUTTON_MIDDLE ::pine::Mouse::ButtonMiddle
