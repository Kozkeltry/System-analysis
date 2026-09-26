#include <iostream>
#include <windows.h>
#include <string>

void ShowDiskInfo(const char* driveName) {
    ULARGE_INTEGER freeForUser, total, freeTotal;

    if (GetDiskFreeSpaceExA(driveName, &freeForUser, &total, &freeTotal)) {
        std::cout << "Всего: " << (double)total.QuadPart / (1024*1024*1024) << " GB\n";
        std::cout << "Свободно: " << (double)freeTotal.QuadPart / (1024*1024*1024) << " GB\n";
        std::cout << "Доступно: " << (double)freeForUser.QuadPart / (1024*1024*1024) << " GB\n";
    }
}

void ShowMemoryInfo() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        ULONGLONG usedRam = memStatus.ullTotalPhys - memStatus.ullAvailPhys;
        double gb = 1024.0 * 1024.0 * 1024.0;
        
        std::cout << "Всего RAM: " << (double)memStatus.ullTotalPhys / gb << " GB\n";
        std::cout << "Общий размер файла подкачки: " << (double)memStatus.ullTotalPageFile / gb << " GB\n";
        std::cout << "Занято RAM: " << (double)usedRam / gb << " GB\n";
        std::cout << "Занято RAM: " << memStatus.dwMemoryLoad << " %\n";
    }
}

void Windows() {
    typedef NTSTATUS(WINAPI* RtlGetVersion_t)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW info{};
    info.dwOSVersionInfoSize = sizeof(info);

    auto RtlGetVersion = (RtlGetVersion_t)GetProcAddress(
        GetModuleHandle("ntdll"), "RtlGetVersion");

    RtlGetVersion(&info);

    const char* name = (info.dwBuildNumber >= 22000) ? "Windows 11" : "Windows 10";
    printf("%s (build %lu)\n", name, (unsigned long)info.dwBuildNumber);
}

void video() {
    DISPLAY_DEVICEA dd;
    dd.cb = sizeof(dd);
    DWORD i = 0;

    while (EnumDisplayDevicesA(NULL, i, &dd, 0))
    {
        if ((dd.StateFlags & DISPLAY_DEVICE_ACTIVE) && !(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)) {
            if (strlen(dd.DeviceString) > 0) {
                std::cout << "Карта " << i << ": " << dd.DeviceString << std::endl;
            }
        }

        ZeroMemory(&dd, sizeof(dd));
        dd.cb = sizeof(dd);
        i++;
    }
}

void process() {
    SYSTEM_INFO si;

    GetSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        std::cout << "Архитектура: 64 BIT" << std::endl;
    }
    else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        std::cout << "Архитектура: 32 BIT" << std::endl;
    }

    std::cout << "Ядер процессора: " << si.dwNumberOfProcessors;
}

int main() {
    ShowDiskInfo("C:\\");
    ShowMemoryInfo();
    Windows();
    video();
    process();

    return 0;
}