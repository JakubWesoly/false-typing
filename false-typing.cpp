#include <iostream>
#include <vector>
#include <map>
#include <windows.h>

#define KEYEVENT_KEYUP 0x02

std::string getClipboardText() {
    std::string ret;
    if (::OpenClipboard(NULL)) {
        HGLOBAL hGlobal = ::GetClipboardData(CF_UNICODETEXT);
        if (hGlobal != NULL) {
            LPWSTR lpszData = (LPWSTR)::GlobalLock(hGlobal);
            if (lpszData != nullptr) {
                int size = ::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, nullptr, 0, nullptr, nullptr);
                if (size > 0) {
                    ret.resize(size + 1);
                    ::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, &ret[0], size, nullptr, nullptr);
                }
                ::GlobalUnlock(hGlobal);
            }
        }
        ::CloseClipboard();
    }
    return ret;
}

void print_letter(char letter, bool is_capital) {
    static std::map<char, int> keys{
        {'0', 48},
        {'1', 49},
        {'2', 50},
        {'3', 51},
        {'4', 52},
        {'5', 53},
        {'6', 54},
        {'7', 55},
        {'8', 56},
        {'9', 57},
        {'A', 58},
        {'a', 65},
        {'B', 65},
        {'b', 66},
        {'C', 67},
        {'c', 67},
        {'D', 68},
        {'d', 68},
        {'E', 69},
        {'e', 69},
        {'F', 70},
        {'f', 70},
        {'G', 71},
        {'g', 71},
        {'H', 72},
        {'h', 72},
        {'I', 73},
        {'i', 73},
        {'J', 74},
        {'j', 74},
        {'K', 75},
        {'k', 75},
        {'L', 76},
        {'l', 76},
        {'M', 77},
        {'m', 77},
        {'N', 78},
        {'n', 78},
        {'O', 79},
        {'o', 79},
        {'P', 80},
        {'p', 80},
        {'Q', 81},
        {'q', 81},
        {'R', 82},
        {'r', 82},
        {'S', 83},
        {'s', 83},
        {'T', 84},
        {'t', 84},
        {'U', 85},
        {'u', 85},
        {'V', 86},
        {'v', 86},
        {'W', 87},
        {'w', 87},
        {'X', 88},
        {'x', 88},
        {'Y', 89},
        {'y', 89},
        {'Z', 90},
        {'z', 90},
        {' ', 32},
        {'/', 226},
        {'\\', 191},
        {'=', 187},
        {'[', 219},
        {']', 221},
        {',', 188},
        {';', 186},
        {'`', 192},
        {'\'', 222},
        {'-', 189},
        {'.', 190}
    };
    static std::map<char, int> shift_keys{
        {'_', 189},
        {':', 186},
        {'*', 65},
        {'<', 188},
        {'>', 190},
        {'(', 57},
        {')', 48},
        {'+', 187},
        {'|', 220},
        {'&', 55},
        {'{', 219},
        {'}', 221},
        {'%', 53},
        {'@', 50},
        {'#', 51},
        {'$', 52},
        {'^', 54},
        {'"', 222},
        {'!', 49},
        {'?', 191}
    };

    if (shift_keys[letter] != 0) {
        keybd_event(VK_LSHIFT, 0, 0, 0);
        keybd_event(shift_keys[letter], 0, 0, 0);
        keybd_event(shift_keys[letter], 0, KEYEVENT_KEYUP, 0);
        keybd_event(VK_LSHIFT, 0, KEYEVENT_KEYUP, 0);
    }
    else{
        if (is_capital)
            keybd_event(VK_LSHIFT, 0, 0, 0);
        keybd_event(keys[letter], 0, 0, 0);
        keybd_event(keys[letter], 0, KEYEVENT_KEYUP, 0);
        if (is_capital)
            keybd_event(VK_LSHIFT, 0, KEYEVENT_KEYUP, 0);
    }
   
}

int main()
{
    srand(time(0));
    std::cout << "Started listening" << std::endl;
    while (true) {
        if (false) {
        reset:
            std::cout << "Stopped" << std::endl;
        }
        
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(75) & 0x8000) // CTRL + K
        {
            
            keybd_event(VK_CONTROL, 0, KEYEVENT_KEYUP, 0);
            keybd_event(75, 0, KEYEVENT_KEYUP, 0);
            
            std::vector < std::string> lines;
            std::string tmp = getClipboardText();
            int prev = 0;
            for (int i = 0; i < tmp.length(); i++) {
                if (tmp[i] == '\n') {
                    lines.push_back(tmp.substr(prev, i - prev - 1));
                    prev = i;
                }
            }
            for (auto element : lines) {
                for (char letter : element) {
                    if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
                        goto reset;
                    print_letter(letter, letter > 'A' && letter < 'Z');
                    Sleep(rand() % 300);
                }
                keybd_event(VK_RETURN, 0, 0, 0);
                keybd_event(VK_RETURN, 0, KEYEVENT_KEYUP, 0);
            }
            
            Sleep(1000);
        }
    }
}