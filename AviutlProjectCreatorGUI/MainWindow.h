#pragma once

#include <string>

#include <Windows.h>
#include "resource.h"

class MainWindow
{
    std::wstring projectPath;
    std::wstring seriesName ;
    std::wstring projectName;

    std::wstring GetDlgItemTextW(HWND hDlg, UINT nIDDlgItem);

    bool handle_IDOK(HWND hDlg);
    void handle_BOOT_EXPLORER(HWND hDlg);

public:
    static INT_PTR CALLBACK dialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow();
};

