#include "ProjectCreate.h"

#include <regex>
#include <sstream>

#include "config.h"

bool createProject
( HWND hDlg
, const std::wstring& projectName
, const std::wstring& projectPath
, const std::wstring& seriesName
)
{
    namespace fs = std::filesystem;
    try {
        const auto sourcePath = fs::current_path();
        if (projectName.empty() || projectPath.empty()) {
            MessageBoxW(hDlg, L"�K�{���ڂ̓��͂�����Ă��܂���B", WindowNameW, MB_OK | MB_ICONWARNING);
            return false;
        }

        fs::current_path(projectPath);

        if (!seriesName.empty()) {
            fs::create_directories(seriesName);
            fs::current_path(seriesName);
        }

        if (fs::exists(projectName)) {
            if (MessageBoxW(hDlg, L"���łɃv���W�F�N�g�����݂��Ă��܂����A�v���W�F�N�g���쐬���܂���?"
                , WindowNameW, MB_YESNO | MB_ICONQUESTION)
            == IDNO) {
                return false;
            }
        }
        fs::create_directory(projectName);
        fs::current_path(projectName);

        for (const auto path : readAddDirFile(std::wifstream{ sourcePath / L"config" / L"adddir.txt" })) {
            fs::create_directories(replaceTags(path, projectName, projectPath, seriesName));
        }

        createMarkdown(sourcePath, projectName, projectPath, seriesName);
        MessageBoxW(hDlg, L"�����Ƀv���W�F�N�g���쐬�ł��܂����B", WindowNameW, MB_OK | MB_ICONINFORMATION);
    } catch (const std::exception& e) {
        MessageBoxA(hDlg, e.what(), WindowNameA, MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}
std::vector<std::wstring> readAddDirFile(std::wifstream&& file)
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

void createMarkdown
( const std::filesystem::path& basePath
, const std::wstring&          projectName
, const std::wstring&          projectPath
, const std::wstring&          seriesName
)
{
    std::wifstream temp{ basePath / L"config" / L"template.md" };
    if (!temp.is_open()) {
        throw std::runtime_error{ "�e���v���[�gMarkdown�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" };
    }
    std::wofstream output{ L"contents.md" };
    if (!output.is_open()) {
        throw std::runtime_error{ "�e���v���[�gMarkdown�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" };
    }
    temp.imbue(std::locale(""));
    output.imbue(std::locale(""));
    while (!temp.eof()) {
        std::wstring line;
        std::getline(temp, line);
        line = replaceTags(line, projectName, projectPath, seriesName);

        output << line << '\n';
    }
}
std::wstring replaceTags
(       std::wstring  line
, const std::wstring& projectName
, const std::wstring& projectPath
, const std::wstring& seriesName
)
{
    const std::time_t currentTime = std::time(nullptr);
    std::tm timeStruct;
    localtime_s(&timeStruct, &currentTime);

    line = std::regex_replace(line, std::wregex(LR"(%DATE%)"), (std::wostringstream()
        << timeStruct.tm_year + 1900 << L"�N"
        << timeStruct.tm_mon  + 1    << L"��"
        << timeStruct.tm_mday        << L"��"
        ).str()
    );
    line = std::regex_replace(line, std::wregex(LR"(%PROJNAME%)"  ), projectName);
    line = std::regex_replace(line, std::wregex(LR"(%PROJPATH%)"  ), projectPath);
    line = std::regex_replace(line, std::wregex(LR"(%SERIESNAME%)"), seriesName );

    return line;
}
