#include <windows.h>
#include <stdio.h>
#include <stdint.h>


#define PORT "\\\\.\\COM5"

HANDLE microcontrollerPort;
uint8_t keyPressed;
DWORD bytesWritten;

/*
* @brief Callback function to get and send keystrokes
* @params nCode indicates whether or not this is a real keyboard event
* @params wParam indicates what type of keyboard even occurrect
* @params lParam pointer to struct with keyboard information
*/
// Callback function to get and send keystrokes
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* keyboard = (KBDLLHOOKSTRUCT*)lParam;
        keyPressed = keyboard->vkCode;

        BOOL success = WriteFile(microcontrollerPort, &keyPressed, 1, &bytesWritten, NULL);

        if (!success) {
            printf("Error when writing data: %lu\n", (unsigned long)GetLastError());
        }
        else if (bytesWritten < 1) {
            printf("Incomplete write\n");
        }
        else {
            printf("Sent: %u (%lu bytes)\n", keyPressed, bytesWritten);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Create file for communicating with port microcontroller is connected to
    microcontrollerPort = CreateFile(PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    DCB dcb = {0};
    COMMTIMEOUTS timeouts = {0};

    // Error when creating file for port
    if (microcontrollerPort == INVALID_HANDLE_VALUE) {
        printf("Error when opening port: %lu\n", (unsigned long)GetLastError());
        return 1;
    }

    dcb.DCBlength = sizeof(DCB);

    // Error when trying to get current settings
    if (!GetCommState(microcontrollerPort, &dcb)) {
        printf("GetCommState error: %lu\n", (unsigned long)GetLastError());
        CloseHandle(microcontrollerPort);
        return 1;
    }

    // Configuring serial communication settings
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;

    // Error when setting communication settings
    if (!SetCommState(microcontrollerPort, &dcb)) {
        printf("SetCommState error: %lu\n", (unsigned long)GetLastError());
        CloseHandle(microcontrollerPort);
        return 1;
    }


    // Set timeouts for writing to port
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    // Error when setting timeouts
    if (!SetCommTimeouts(microcontrollerPort, &timeouts)) {
        printf("SetCommTimeout error: %lu\n", (unsigned long)GetLastError());
        CloseHandle(microcontrollerPort);
        return 1;
    }

    // Set hook to get and send keystrokes
    HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        continue;
    }

    UnhookWindowsHookEx(hKeyboardHook);
    return 0;
}
