#include "ProjectCreate.h"

#include <regex>
#include <sstream>

#include "config.h"

bool createProjectDir
(       HWND          hDlg
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

        for (const auto path : read_adddir_file(std::wifstream{ sourcePath / L"config" / L"adddir.txt" })) {
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
std::vector<std::wstring> read_adddir_file(std::wifstream&& file)
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
    line = std::regex_replace(line, std::wregex(LR"(%DATEISO%)"), std::format
        ( L"{:0>4}-{:0>2}-{:0>2}\n"
        , timeStruct.tm_year + 1900
        , timeStruct.tm_mon  + 1   
        , timeStruct.tm_mday       
        )
    );
    line = std::regex_replace(line, std::wregex(LR"(%YEAR%)"  ), std::format(L"{:0>4}", timeStruct.tm_year + 1900));
    line = std::regex_replace(line, std::wregex(LR"(%MONTH%)" ), std::format(L"{:0>2}", timeStruct.tm_mon  + 1   ));
    line = std::regex_replace(line, std::wregex(LR"(%DAY%)"   ), std::format(L"{:0>2}", timeStruct.tm_mday       ));
    line = std::regex_replace(line, std::wregex(LR"(%HOUR%)"  ), std::format(L"{:0>2}", timeStruct.tm_hour       ));
    line = std::regex_replace(line, std::wregex(LR"(%MINUTE%)"), std::format(L"{:0>2}", timeStruct.tm_min        ));
    line = std::regex_replace(line, std::wregex(LR"(%SECOND%)"), std::format(L"{:0>2}", timeStruct.tm_sec        ));
    line = std::regex_replace(line, std::wregex(LR"(%TIME_T%)"), std::format(L"{:0>1}", currentTime              ));

    line = std::regex_replace(line, std::wregex(LR"(%PROJNAME%)"  ), projectName);
    line = std::regex_replace(line, std::wregex(LR"(%PROJPATH%)"  ), projectPath);
    line = std::regex_replace(line, std::wregex(LR"(%SERIESNAME%)"), seriesName );

    return line;
}
