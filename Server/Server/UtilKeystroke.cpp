#include "pch.h"

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>


CString GetKeystrokeName(int vkCode) {
    CString keyName = " ";
    if (vkCode == VK_SHIFT || vkCode == VK_CONTROL || vkCode == VK_MENU) return keyName; // ignore modifier keys
    else if (vkCode == VK_LSHIFT || vkCode == VK_RSHIFT) keyName = "[SHIFT]";
    else if (vkCode == VK_LCONTROL || vkCode == VK_RCONTROL) keyName = "[CTRL]";
    else if (vkCode == VK_LMENU || vkCode == VK_RMENU) keyName = "[ALT]";
    else if (vkCode == VK_LWIN || vkCode == VK_RWIN) keyName = "[WINDOWS]";
    else if (vkCode == VK_CAPITAL) keyName = "[CAPS LOCK]";
    else if (vkCode == VK_BACK) keyName = "[BACKSPACE]";
    else if (vkCode == VK_TAB) keyName = "[TAB]";
    else if (vkCode == VK_CLEAR) keyName = "[DELETE]";
    else if (vkCode == VK_RETURN) keyName = "[ENTER]";
    else if (vkCode == VK_ESCAPE) keyName = "[ESC]";
    else if (vkCode == VK_SPACE) keyName = "[SPACEBAR]";
    else if (vkCode == VK_PRIOR) keyName = "[PAGE UP]";
    else if (vkCode == VK_NEXT) keyName = "[PAGE DOWN]";
    else if (vkCode == VK_END) keyName = "[END]";
    else if (vkCode == VK_HOME) keyName = "[HOME]";
    else if (vkCode == VK_LEFT) keyName = "[LEFT]";
    else if (vkCode == VK_UP) keyName = "[UP]";
    else if (vkCode == VK_RIGHT) keyName = "[RIGHT]";
    else if (vkCode == VK_DOWN) keyName = "[DOWN]";
    else if (vkCode == VK_SNAPSHOT) keyName = "[PRTSC]";
    else if (vkCode == VK_INSERT) keyName = "[INSERT]";
    else if (vkCode == VK_DELETE) keyName = "[DELETE]";
    else if (vkCode == VK_NUMLOCK) keyName = "[NUM LOCK]";
    else if (vkCode == VK_SCROLL) keyName = "[SCROLL LOCK]";
    else if (vkCode == VK_ADD) keyName = "[ADD]";
    else if (vkCode == VK_SUBTRACT) keyName = "[SUBTRACT]";
    else if (vkCode == VK_MULTIPLY) keyName = "[MULTIPLY]";
    else if (vkCode == VK_DIVIDE) keyName = "[DIVIDE]";
    else if (vkCode == VK_DECIMAL) keyName = "[DECIMAL]";
    else if (vkCode == VK_SEPARATOR) keyName = "[SEPARATOR]";
    else if (vkCode >= VK_F1 && vkCode <= VK_F24) {
        int num = vkCode - VK_F1 + 1;
        std::stringstream s;
        s << "[F" << num << "]";
        keyName = CString(s.str().c_str());
    }
    else if (vkCode == VK_OEM_3) keyName = "`";
    else if (vkCode == VK_OEM_MINUS) keyName = "-";
    else if (vkCode == VK_OEM_PLUS) keyName = "=";
    else if (vkCode == VK_OEM_4) keyName = "[";
    else if (vkCode == VK_OEM_5) keyName = "\\";
    else if (vkCode == VK_OEM_6) keyName = "]";
    else if (vkCode == VK_OEM_1) keyName = ";";
    else if (vkCode == VK_OEM_7) keyName = "'";
    else if (vkCode == VK_OEM_COMMA) keyName = ",";
    else if (vkCode == VK_OEM_PERIOD) keyName = ".";
    else if (vkCode == VK_OEM_2) keyName = "/";
    else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
        int num = vkCode - VK_NUMPAD0;
        std::stringstream s;
        s << "[NUMPAD" << num << "]";
        keyName = CString(s.str().c_str());
    }
    else if ((vkCode >= 'A' && vkCode <= 'Z') || (vkCode >= '0' && vkCode <= '9'))
    {
        std::stringstream s;
        s << (char)vkCode;
        keyName = CString(s.str().c_str());
    }
                
    return keyName;
    return keyName;
}


