#pragma once

#include <iostream>

namespace pine
{

enum class MouseCode : uint16_t
{
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
};

inline std::ostream& operator<<(std::ostream& os, const MouseCode mouseCode)
{
    return os << static_cast<int32_t>(mouseCode);
}

} // namespace pine

#define PINE_MOUSE_BUTTON_0 ::pine::MouseCode::Button0
#define PINE_MOUSE_BUTTON_1 ::pine::MouseCode::Button1
#define PINE_MOUSE_BUTTON_2 ::pine::MouseCode::Button2
#define PINE_MOUSE_BUTTON_3 ::pine::MouseCode::Button3
#define PINE_MOUSE_BUTTON_4 ::pine::MouseCode::Button4
#define PINE_MOUSE_BUTTON_5 ::pine::MouseCode::Button5
#define PINE_MOUSE_BUTTON_6 ::pine::MouseCode::Button6
#define PINE_MOUSE_BUTTON_7 ::pine::MouseCode::Button7
#define PINE_MOUSE_BUTTON_LAST ::pine::MouseCode::ButtonLast
#define PINE_MOUSE_BUTTON_LEFT ::pine::MouseCode::ButtonLeft
#define PINE_MOUSE_BUTTON_RIGHT ::pine::MouseCode::ButtonRight
#define PINE_MOUSE_BUTTON_MIDDLE ::pine::MouseCode::ButtonMiddle
