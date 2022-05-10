#include "ProjectCreate.h"

#include <regex>
#include <sstream>
#include <iostream>

bool createProject
( const std::string& projectName
, const std::string& projectPath
, const std::string& seriesName
)
{
    namespace fs = std::filesystem;
    try {
        const auto sourcePath = fs::current_path();
        if (projectName.empty() || projectPath.empty()) {
            std::cout << "必須項目の入力がされていません。\n";
            return false;
        }

        fs::current_path(projectPath);

        if (!seriesName.empty()) {
            fs::create_directories(seriesName);
            fs::current_path(seriesName);
        }

        if (fs::exists(projectName)) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "すでにプロジェクトが存在していますが、プロジェクトを作成しますか? (y/N)\n";
            char ch;
            if (!(std::cin >> ch)) {
                std::cerr << "入力に失敗しました。\n";
                return false;
            }
            if (!(ch == 'Y' || ch == 'y')) {
                return false;
            }
        }
        fs::create_directory(projectName);
        fs::current_path(projectName);

        for (const auto path : read_adddir_file(std::ifstream{ sourcePath / "config" / "adddir.txt" })) {
            fs::create_directories(replaceTags(path, projectName, projectPath, seriesName));
        }

        createMarkdown(sourcePath, projectName, projectPath, seriesName);
        std::cout << "無事にプロジェクトを作成できました。\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        return false;
    }
    return true;
}
std::vector<std::string> read_adddir_file(std::ifstream&& file)
{
    std::vector<std::string> addCreateDirList;
    for (;;) {
        if (file.fail()) {
            return addCreateDirList;
        }
        std::string dir;
        std::getline(file, dir);
        if (!dir.empty()) {
            addCreateDirList.push_back(dir);
        }
    }
    return addCreateDirList;
}

void createMarkdown
( const std::filesystem::path& basePath
, const std::string&          projectName
, const std::string&          projectPath
, const std::string&          seriesName
)
{
    std::ifstream temp{ basePath / "config" / "template.md" };
    if (!temp.is_open()) {
        throw std::runtime_error{ "テンプレートMarkdownファイルの読み込みに失敗しました。" };
    }
    std::ofstream output{ "contents.md" };
    if (!output.is_open()) {
        throw std::runtime_error{ "テンプレートMarkdownファイルの読み込みに失敗しました。" };
    }

    while (!temp.eof()) {
        std::string line;
        std::getline(temp, line);
        line = replaceTags(line, projectName, projectPath, seriesName);

        output << line << '\n';
    }
}
std::string replaceTags
(       std::string  line
, const std::string& projectName
, const std::string& projectPath
, const std::string& seriesName
)
{
    const std::time_t currentTime = std::time(nullptr);
    std::tm timeStruct;
    localtime_s(&timeStruct, &currentTime);

    line = std::regex_replace(line, std::regex(R"(%DATE%)"), (std::ostringstream()
        << timeStruct.tm_year + 1900 << "年"
        << timeStruct.tm_mon  + 1    << "月"
        << timeStruct.tm_mday        << "日"
        ).str()
    );
    line = std::regex_replace(line, std::regex(R"(%DATEISO%)"), (std::ostringstream()
        << timeStruct.tm_year + 1900 << "-"
        << timeStruct.tm_mon  + 1    << "-"
        << timeStruct.tm_mday        << "-"
        ).str()
    );
    line = std::regex_replace(line, std::regex(R"(%YEAR%)"  ), std::to_string(timeStruct.tm_year + 1900));
    line = std::regex_replace(line, std::regex(R"(%MONTH%)" ), std::to_string(timeStruct.tm_mon  + 1   ));
    line = std::regex_replace(line, std::regex(R"(%DAY%)"   ), std::to_string(timeStruct.tm_mday       ));
    line = std::regex_replace(line, std::regex(R"(%HOUR%)"  ), std::to_string(timeStruct.tm_hour       ));
    line = std::regex_replace(line, std::regex(R"(%MINUTE%)"), std::to_string(timeStruct.tm_min        ));
    line = std::regex_replace(line, std::regex(R"(%SECOND%)"), std::to_string(timeStruct.tm_sec        ));
    line = std::regex_replace(line, std::regex(R"(%TIME_T%)"), std::to_string(currentTime              ));

    line = std::regex_replace(line, std::regex(R"(%PROJNAME%)"  ), projectName);
    line = std::regex_replace(line, std::regex(R"(%PROJPATH%)"  ), projectPath);
    line = std::regex_replace(line, std::regex(R"(%SERIESNAME%)"), seriesName );

    return line;
}
