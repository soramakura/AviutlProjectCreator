#pragma once

#include <string>
#include <filesystem>

#include <Windows.h>
#include "resource.h"

class MainWindow
{
    std::wstring projectPath;
    std::wstring seriesName ;
    std::wstring projectName;

    std::wstring getDlgItemTextW(HWND hDlg, UINT nIDDlgItem);

    bool handle_IDOK(HWND hDlg, WPARAM wParam, LPARAM lParam);
    void handle_BOOT_EXPLORER(HWND hDlg);

public:
    static INT_PTR CALLBACK dialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow();
};

