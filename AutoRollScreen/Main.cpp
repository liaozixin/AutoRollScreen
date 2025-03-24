#include <iostream>
#include <Windows.h>
#include <vector>

HHOOK hHook;
std::vector<DWORD> keyboardStack;
UINT_PTR timeID = 0;

void SendPageDown() {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_NEXT;

    SendInput(1, &input, sizeof(INPUT));
    std::cout << "·­Ò³" << std::endl;
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    SendPageDown();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = pKey->vkCode;

        if (vkCode == 0x53 ) {
            if (wParam == WM_KEYDOWN) {
                keyboardStack.push_back(vkCode); 
                if (keyboardStack.size() == 1) {
                    std::cout << "Æô¶¯×Ô¶¯·­Ò³³ÌÐò..." << std::endl;
                    timeID = SetTimer(NULL, 1, 1500, (TIMERPROC)TimerProc);
                }
                else if (keyboardStack.size() == 2) {
                    keyboardStack.clear();
                    std::cout << "Í£Ö¹×Ô¶¯·­Ò³³ÌÐò..." << std::endl;
                    KillTimer(NULL, timeID);
                }
            } 
            return 1;
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void SetHook() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (!hHook) {
        std::cout << "Hook error!" << std::endl;
    }
}

void ReleaseHook() {
    UnhookWindowsHookEx(hHook);
}

int main() {
    SetHook();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    ReleaseHook();
    return 0;
}
