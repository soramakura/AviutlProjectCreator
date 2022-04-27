#pragma once

#include <string>
#include <filesystem>

#include <Windows.h>
#include "resource.h"

class MainWindow
{
    std::wstring projectPath;
    std::wstring seriesName;
    std::wstring projectName;


    inline static const wchar_t WindowNameW[] = L"Aviutl Project Creator GUI";
    inline static const char    WindowNameA[] =  "Aviutl Project Creator GUI";

    std::wstring getDlgItemTextW(HWND hDlg, UINT nIDDlgItem);

    std::vector<std::wstring> readAddDirFile(std::wifstream&& file);
    std::wstring replaceReplacer(std::wstring line);
    void         createMarkdown (const std::filesystem::path& basePath);
    bool         createProject  (HWND hDlg);

    bool handle_IDOK(HWND hDlg, WPARAM wParam, LPARAM lParam);
    void handle_BOOT_EXPLORER(HWND hDlg);

public:
    static INT_PTR CALLBACK dialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

