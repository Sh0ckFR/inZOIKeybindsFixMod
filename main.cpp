#include <windows.h>

volatile bool g_Running = true;
HANDLE g_hThread = NULL;

void SimulateKeyDown(BYTE vkKey, BYTE scanCode = 0) {
    keybd_event(vkKey, scanCode, 0, 0);
}

void SimulateKeyUp(BYTE vkKey, BYTE scanCode = 0) {
    keybd_event(vkKey, scanCode, KEYEVENTF_KEYUP, 0);
}

DWORD WINAPI KeyPollingThread(LPVOID lpParam) {
    bool isWActive = false;
    bool isSActive = false;
    bool isAActive = false;
    bool isDActive = false;
    bool isLSHIFTActive = false;

    BYTE leftShiftScan = (BYTE)MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);

    while (g_Running) {
        Sleep(10);

        // Check if the foreground window belongs to our process (the game)
        HWND fgWindow = GetForegroundWindow();
        DWORD fgProcessId = 0;
        GetWindowThreadProcessId(fgWindow, &fgProcessId);
        DWORD currentProcessId = GetCurrentProcessId();
        if (fgProcessId != currentProcessId) {
            // If the game is not in the foreground, release any active keys
            if (isWActive) { SimulateKeyUp('W'); isWActive = false; }
            if (isSActive) { SimulateKeyUp('S'); isSActive = false; }
            if (isAActive) { SimulateKeyUp('A'); isAActive = false; }
            if (isDActive) { SimulateKeyUp('D'); isDActive = false; }
            if (isLSHIFTActive) { SimulateKeyUp(VK_LSHIFT, leftShiftScan); isLSHIFTActive = false; }
            continue;
        }

        if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('Z') & 0x8000)) {
            if (!isWActive) {
                SimulateKeyDown('W');
                isWActive = true;
            }
        } else {
            if (isWActive) {
                SimulateKeyUp('W');
                isWActive = false;
            }
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (!isSActive) {
                SimulateKeyDown('S');
                isSActive = true;
            }
        } else {
            if (isSActive) {
                SimulateKeyUp('S');
                isSActive = false;
            }
        }

        if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('Q') & 0x8000)) {
            if (!isAActive) {
                SimulateKeyDown('A');
                isAActive = true;
            }
        } else {
            if (isAActive) {
                SimulateKeyUp('A');
                isAActive = false;
            }
        }

        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (!isDActive) {
                SimulateKeyDown('D');
                isDActive = true;
            }
        } else {
            if (isDActive) {
                SimulateKeyUp('D');
                isDActive = false;
            }
        }

        // Use Enter key to simulate left shift (to make the character run)
        if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000)) {
            if (!isLSHIFTActive) {
                SimulateKeyDown(VK_LSHIFT, leftShiftScan);
                isLSHIFTActive = true;
            }
        } else {
            if (isLSHIFTActive) {
                SimulateKeyUp(VK_LSHIFT, leftShiftScan);
                isLSHIFTActive = false;
            }
        }
    }

    // Ensure all keys are released when exiting
    if (isWActive) SimulateKeyUp('W');
    if (isSActive) SimulateKeyUp('S');
    if (isAActive) SimulateKeyUp('A');
    if (isDActive) SimulateKeyUp('D');
    if (isLSHIFTActive) SimulateKeyUp(VK_LSHIFT, leftShiftScan);

    return 0;
}

DWORD WINAPI DllMainThread(LPVOID lpParam) {
    HANDLE hThread = CreateThread(NULL, 0, KeyPollingThread, NULL, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        g_Running = true;
        g_hThread = CreateThread(NULL, 0, DllMainThread, NULL, 0, NULL);
        if (!g_hThread)
            return FALSE;
        break;
    case DLL_PROCESS_DETACH:
        g_Running = false;
        if (g_hThread) {
            WaitForSingleObject(g_hThread, 5000);
            CloseHandle(g_hThread);
        }
        break;
    }
    return TRUE;
}
