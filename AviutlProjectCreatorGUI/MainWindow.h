#pragma once

#include <string>

#include <Windows.h>
#include "resource.h"

namespace Win32Wrap {
std::wstring GetDlgItemTextW(HWND hDlg, UINT nIDDlgItem);
} //namespace Win32Wrap

class MainWindow
{
    std::wstring projectPath;
    std::wstring seriesName ;
    std::wstring projectName;

    bool handle_IDOK(HWND hDlg);
    void handle_BOOT_EXPLORER(HWND hDlg);

public:
    static INT_PTR CALLBACK dialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow();
};

