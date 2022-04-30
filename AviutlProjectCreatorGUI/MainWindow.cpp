#include "MainWindow.h"

#include <process.h>

#include "resource.h"
#include "config.h"
#include "ProjectCreate.h"

std::wstring MainWindow::getDlgItemTextW(HWND hDlg, UINT nIDDlgItem)
{
    std::wstring str(4095, L'\0');
    str.resize(GetDlgItemTextW
    ( /*HWND   hDlg      */hDlg
    , /*int    nIDDlgItem*/nIDDlgItem
    , /*LPWSTR lpString  */&str.front()
    , /*int    cchMax    */static_cast<int>(str.size() + 1)
    ));
    return str;
}

bool MainWindow::handle_IDOK(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    s = getDlgItemTextW(hDlg, PROJECT_PATH_EDIT);
    seriesName  = getDlgItemTextW(hDlg, SERIES_NAME_EDIT );
    projectName = getDlgItemTextW(hDlg, PROJECT_NAME_EDIT);
    return createProject(hDlg, projectName, s, seriesName);
}

void MainWindow::handle_BOOT_EXPLORER(HWND hDlg)
{
    STARTUPINFOW startupInfo{ .cb{sizeof(startupInfo)} };
    PROCESS_INFORMATION processInfo;
    if (!CreateProcessW
    ( /*_In_opt_    LPCWSTR               lpApplicationName   */LR"(C:\Windows\explorer.exe)"
    , /*_Inout_opt_ LPWSTR                lpCommandLine       */nullptr
    , /*_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes */nullptr
    , /*_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes  */nullptr
    , /*_In_        BOOL                  bInheritHandles     */false
    , /*_In_        DWORD                 dwCreationFlags     */0
    , /*_In_opt_    LPVOID                lpEnvironment       */nullptr
    , /*_In_opt_    LPCWSTR               lpCurrentDirectory  */nullptr
    , /*_In_        LPSTARTUPINFOW        lpStartupInfo       */&startupInfo
    , /*_Out_       LPPROCESS_INFORMATION lpProcessInformation*/&processInfo
    )) {
        MessageBoxW(hDlg, L"エクスプローラーを起動できませんでした。", WindowNameW, MB_OK | MB_ICONERROR);
        return;
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
}

INT_PTR CALLBACK MainWindow::dialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, static_cast<LONG_PTR>(lParam));
        return true;

    case WM_CLOSE:
        EndDialog(hDlg, true);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case BOOT_EXPLORER:
            if (const auto p = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hDlg, GWLP_USERDATA))) {
                p->handle_BOOT_EXPLORER(hDlg);
            }
            return true;

        case IDOK:
            if (const auto p = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hDlg, GWLP_USERDATA))) {
                if (p->handle_IDOK(hDlg, wParam, lParam)) {
                    EndDialog(hDlg, true);
                }
            }
            return true;

        default:
            return false;
        }

    default:
        return false;
    }
}
MainWindow::MainWindow()
{
    DialogBoxParamW
    ( /*_In_opt_ HINSTANCE hInstance     */nullptr
    , /*_In_     LPCWSTR   lpTemplateName*/MAKEINTRESOURCEW(DIALOG1)
    , /*_In_opt_ HWND      hWndParent    */nullptr
    , /*_In_opt_ DLGPROC   lpDialogFunc  */MainWindow::dialogProc
    , /*_In_     LPARAM    dwInitParam   */reinterpret_cast<LPARAM>(this)
    );
}
