#include "MainWindow.h"

#include <process.h>
#include <shobjidl_core.h>

#include "resource.h"
#include "config.h"
#include "ProjectCreate.h"

std::wstring Win32Wrap::GetDlgItemTextW(HWND hDlg, UINT nIDDlgItem)
{
    std::wstring str(4095, L'\0');
    str.resize(::GetDlgItemTextW
    ( /*HWND   hDlg      */hDlg
    , /*int    nIDDlgItem*/nIDDlgItem
    , /*LPWSTR lpString  */&str.front()
    , /*int    cchMax    */static_cast<int>(str.size() + 1)
    ));
    return str;
}

bool MainWindow::handle_IDOK(HWND hDlg)
{
    projectPath = Win32Wrap::GetDlgItemTextW(hDlg, PROJECT_PATH_EDIT);
    seriesName  = Win32Wrap::GetDlgItemTextW(hDlg, SERIES_NAME_EDIT );
    projectName = Win32Wrap::GetDlgItemTextW(hDlg, PROJECT_NAME_EDIT);
    return createProjectDir(hDlg, projectName, projectPath, seriesName);
}

void MainWindow::handle_BOOT_EXPLORER(HWND hDlg)
{
    //以下サイトからのコピペ
    //https://donadona.hatenablog.jp/entry/2017/01/09/005447

    IFileDialog* pDialog = nullptr;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog)))) {
        return;
    }

    DWORD options;
    pDialog->GetOptions(&options);
    pDialog->SetOptions(options | FOS_PICKFOLDERS);
    

    if (SUCCEEDED(pDialog->Show(nullptr))) {
        IShellItem* pItem = nullptr;
        PWSTR pPath = nullptr;
        if (SUCCEEDED(pDialog->GetResult(&pItem))) {
            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath))) {
                projectPath = pPath;
                // pPathのメモリを開放する
                CoTaskMemFree(pPath);
            }
        }
    }
    pDialog->Release();

    //画面の更新
    SetDlgItemTextW(hDlg, PROJECT_PATH_EDIT, projectPath.c_str());
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
                if (p->handle_IDOK(hDlg)) {
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
