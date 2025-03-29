// Injector.cpp
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <wchar.h>

// Helper function to find the target process ID by its executable name.
DWORD FindProcessId(const wchar_t* processName) {
    DWORD processId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hSnapshot, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, processName) == 0) {
                    processId = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    return processId;
}

int wmain() {
    const wchar_t* targetProcessName = L"inZOI-Win64-Shipping.exe";
    DWORD processID = 0;

    wprintf(L"Please, start InZOI to enable the mod...");
    // Wait until the target process is launched.
    while ((processID = FindProcessId(targetProcessName)) == 0) {
        Sleep(100);
    }

    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
                                  FALSE, processID);
    if (!hProcess) {
        MessageBoxW(NULL, L"Failed to open target process.", L"Error", MB_OK);
        return 1;
    }

    wchar_t dllPath[MAX_PATH] = {0};
    if (GetModuleFileNameW(NULL, dllPath, MAX_PATH) == 0) {
        MessageBoxW(NULL, L"Failed to get module file name.", L"Error", MB_OK);
        CloseHandle(hProcess);
        return 1;
    }

    wchar_t* lastSlash = wcsrchr(dllPath, L'\\');
    if (lastSlash) {
        *(lastSlash + 1) = L'\0';
    }
    wcscat_s(dllPath, L"InzoiKeybindsFix.dll");

    size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteMemory) {
        MessageBoxW(NULL, L"Failed to allocate memory in target process.", L"Error", MB_OK);
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, dllPathSize, NULL)) {
        MessageBoxW(NULL, L"Failed to write DLL path in target process.", L"Error", MB_OK);
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
    if (!pLoadLibraryW) {
        MessageBoxW(NULL, L"Failed to get address of LoadLibraryW.", L"Error", MB_OK);
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)pLoadLibraryW, pRemoteMemory, 0, NULL);
    if (!hRemoteThread) {
        MessageBoxW(NULL, L"Failed to create remote thread.", L"Error", MB_OK);
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);

    MessageBoxW(NULL, L"Mod successfully injected in the game, enjoy ;)", L"Injection Successful", MB_OK);
    
    return 0;
}
