#include "File.h"

namespace File {
std::string toDirectoryPath(std::string path)
{
    path += '\\';

    return path;
}

int makeDirectories(const std::string& sourcePath)
{
    const auto path = toDirectoryPath(sourcePath);

    if (!::PathFileExistsA(path.c_str())) {
        if (MakeSureDirectoryPathExists(path.c_str())) {
            //作成できた
            std::cout << path.substr(0, path.size() - 1) << "を作成しました。\n";
            return 0;
        }
        else {
            //作成できなかった
            std::cerr << path.substr(0, path.size() - 1) << "の作成に失敗しました。\n";
            return -1;
        }
    }
    else {
        //既に作成されていた
        std::cout << path.substr(0, path.size() - 1) << "は既に作成されています。\n";
        return -2;
    }
}

int changeCurrentDirectory(const std::string& sourcePath)
{
    const auto path = toDirectoryPath(sourcePath);
    if (!::PathFileExistsA(path.c_str())) {
        //ディレクトリが存在しなかった
        std::cerr << path.substr(0, path.size() - 1) << "が存在しませんでした。\n";
        return -2;
    }
    //ディレクトリが存在する
    if (SetCurrentDirectoryA(path.c_str())) {
        //移動できた
        return 0;
    }
    else {
        //移動できなかった
        std::cerr << "カレントディレクトリの移動に失敗しました。\n";
        return -1;
    }
}

std::tm getTm() noexcept
{
    std::time_t nowSec   = std::time(nullptr);
    std::tm*    tmStruct = std::localtime(&nowSec);

    return std::move(*tmStruct);
}

int writeContentsText(const std::string& path, const std::string& projectName)
{
    const std::string filename = path + projectName + ".md";

    //ファイルの有無を確かめる
    if (!::PathFileExistsA(filename.c_str())) {
        if (std::ofstream write { filename }) {
            //書き込めた
            std::cout << filename << "を作成しました。\n";

            std::tm now = getTm();

            //本体書き込み
            write
                << "# " << projectName << "の情報\n\n"
                << "作成日：" << now.tm_year + 1900 << "年" << now.tm_mon + 1 << "月" << now.tm_mday << "日\n\n"

                << "---\n"
                << "## プロジェクト名\n"
                << "- " << projectName << '\n'
                << "## タイトル\n"
                << "- " << projectName << '\n'
                << "## シリーズ\n\n"
                << "- " << path << "  \n"
                << "## 分類\n\n"
                << "- 無分類  \n"
                
                << "---\n"
                << "## アップ可能か\n"
                << "<Yes or No>\n\n"
                << "## 説明\n"
                << "- ここに内容を書く  \n\n"
                << "使った素材\n"
                << "- ここに素材を書く  \n\n"
                ;
            write.close();

            return 0;
        }
        else {
            //書き込めなかった
            std::cerr << filename << "の作成に失敗しました。\n";
            return true;
        }
    }
    else {
        //既に作成されていた
        std::cout << filename << "は既に作成されています。\n";
        return -2;
    }
}
} //namespace File
