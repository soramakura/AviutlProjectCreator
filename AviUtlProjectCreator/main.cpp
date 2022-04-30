#include <iostream>
#include <string>
#include <stdexcept>

#define NOMINMAX //std::numeric_limits<std::streamsize>::max()�� #define ��������Ďg���Ȃ�����
#include <Windows.h>
#include <conio.h>

#include "ProjectCreate.h"

std::string input(const std::string& prompt)
{
    std::string s;
    std::cout << prompt << std::flush;
    if (!std::getline(std::cin, s)) {
        throw std::runtime_error{ "���͂Ɏ��s���܂����B" };
    }
    return s;
}

int main()
{
    SetConsoleTitleA("Aviutl Project Creator");


    try {
        const std::string projectPath = input("�쐬����ꏊ����͂��Ă��������B\n");
        const std::string seriesName  = input("�V���[�Y������͂��Ă������� (�Ȃ��Ă���)�B\n");
        const std::string projectName = input("�v���W�F�N�g������͂��Ă��������B\n");

        if (!createProject(projectName, projectPath, seriesName)) {
            std::cerr << "�v���W�F�N�g�̍쐬�Ɏ��s���܂����B\n";
            return EXIT_FAILURE;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    std::cout << "�쐬�͏I�����܂����B\n\n";
    (void)_getch();
}
