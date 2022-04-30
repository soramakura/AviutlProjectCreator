#include <iostream>
#include <string>
#include <stdexcept>

#define NOMINMAX //std::numeric_limits<std::streamsize>::max()が #define 扱いされて使えないため
#include <Windows.h>
#include <conio.h>

#include "ProjectCreate.h"

std::string input(const std::string& prompt)
{
    std::string s;
    std::cout << prompt << std::flush;
    if (!std::getline(std::cin, s)) {
        throw std::runtime_error{ "入力に失敗しました。" };
    }
    return s;
}

int main()
{
    SetConsoleTitleA("Aviutl Project Creator");


    try {
        const std::string projectPath = input("作成する場所を入力してください。\n");
        const std::string seriesName  = input("シリーズ名を入力してください (なくても可)。\n");
        const std::string projectName = input("プロジェクト名を入力してください。\n");

        if (!createProject(projectName, projectPath, seriesName)) {
            std::cerr << "プロジェクトの作成に失敗しました。\n";
            return EXIT_FAILURE;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    std::cout << "作成は終了しました。\n\n";
    (void)_getch();
}
