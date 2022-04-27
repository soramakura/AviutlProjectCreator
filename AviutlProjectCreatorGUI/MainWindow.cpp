#include "MainWindow.h"

#include <format>
#include <regex>
#include <sstream>
#include <fstream>

#include <process.h>

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

std::vector<std::wstring> MainWindow::readAddDirFile(std::wifstream&& file)
{
    std::vector<std::wstring> addCreateDirList;
    for (;;) {
        if (file.fail()) {
            return addCreateDirList;
        }
        std::wstring dir;
        std::getline(file, dir);
        if (!dir.empty()) {
            addCreateDirList.push_back(dir);
        }
    }
    return addCreateDirList;
}
std::wstring MainWindow::replaceReplacer(std::wstring line)
{
    const std::wregex Date      (LR"(%DATE%)"      );
    const std::wregex ProjName  (LR"(%PROJNAME%)"  );
    const std::wregex ProjPath  (LR"(%PROJPATH%)"  );
    const std::wregex SeriesName(LR"(%SERIESNAME%)");

    const std::time_t currentTime = std::time(nullptr);
    std::tm timeStruct;
    localtime_s(&timeStruct, &currentTime);

    line = std::regex_replace(line, Date, std::format
    (L"{:0>4}年{:0>2}月{:0>2}日"
    , timeStruct.tm_year + 1900
    , timeStruct.tm_mon  + 1
    , timeStruct.tm_mday
    ));
    line = std::regex_replace(line, ProjName, projectName);
    line = std::regex_replace(line, ProjPath, projectPath);
    line = std::regex_replace(line, SeriesName, seriesName);

    return line;
}
void MainWindow::createMarkdown(const std::filesystem::path& basePath)
{
    std::wifstream temp{ basePath / L"config" / L"template.md" };
    if (!temp.is_open()) {
        throw std::runtime_error{ "テンプレートMarkdownファイルの読み込みに失敗しました。" };
    }
    std::wofstream output{ L"contents.md" };
    if (!output.is_open()) {
        throw std::runtime_error{ "テンプレートMarkdownファイルの読み込みに失敗しました。" };
    }

    while (!temp.eof()) {
        std::wstring line;
        std::getline(temp, line);
        line = replaceReplacer(line);

        output << line << '\n';
    }
}
bool MainWindow::createProject(HWND hDlg)
{
    namespace fs = std::filesystem;
    try {
        const auto sourcePath = fs::current_path();
        if (projectName.empty() || projectPath.empty()) {
            MessageBoxW(hDlg, L"必須項目の入力がされていません。", WindowNameW, MB_OK | MB_ICONWARNING);
            return false;
        }

        fs::current_path(projectPath);

        if (!seriesName.empty()) {
            fs::create_directories(seriesName);
            fs::current_path(seriesName);
        }

        if (fs::exists(projectName)) {
            if (MessageBoxW(hDlg, L"すでにプロジェクトが存在していますが、プロジェクトを作成しますか?"
                , WindowNameW, MB_OK | MB_ICONQUESTION)
            == IDCANCEL) {
                return false;
            }
        }
        fs::create_directory(projectName);
        fs::current_path(projectName);

        for (const auto path : readAddDirFile(std::wifstream{ sourcePath / L"config" / L"adddir.txt" })) {
            fs::create_directories(replaceReplacer(path));
        }

        createMarkdown(sourcePath);
        MessageBoxW(hDlg, L"無事にプロジェクトを作成できました。", WindowNameW, MB_OK | MB_ICONINFORMATION);
    } catch (const std::exception& e) {
        MessageBoxA(hDlg, e.what(), WindowNameA, MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

bool MainWindow::handle_IDOK(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    projectPath = getDlgItemTextW(hDlg, PROJECT_PATH_EDIT);
    seriesName  = getDlgItemTextW(hDlg, SERIES_NAME_EDIT );
    projectName = getDlgItemTextW(hDlg, PROJECT_NAME_EDIT);
    return createProject(hDlg);
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
