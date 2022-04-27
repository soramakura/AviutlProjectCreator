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
            //�쐬�ł���
            std::cout << path.substr(0, path.size() - 1) << "���쐬���܂����B\n";
            return 0;
        }
        else {
            //�쐬�ł��Ȃ�����
            std::cerr << path.substr(0, path.size() - 1) << "�̍쐬�Ɏ��s���܂����B\n";
            return -1;
        }
    }
    else {
        //���ɍ쐬����Ă���
        std::cout << path.substr(0, path.size() - 1) << "�͊��ɍ쐬����Ă��܂��B\n";
        return -2;
    }
}

int changeCurrentDirectory(const std::string& sourcePath)
{
    const auto path = toDirectoryPath(sourcePath);
    if (!::PathFileExistsA(path.c_str())) {
        //�f�B���N�g�������݂��Ȃ�����
        std::cerr << path.substr(0, path.size() - 1) << "�����݂��܂���ł����B\n";
        return -2;
    }
    //�f�B���N�g�������݂���
    if (SetCurrentDirectoryA(path.c_str())) {
        //�ړ��ł���
        return 0;
    }
    else {
        //�ړ��ł��Ȃ�����
        std::cerr << "�J�����g�f�B���N�g���̈ړ��Ɏ��s���܂����B\n";
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

    //�t�@�C���̗L�����m���߂�
    if (!::PathFileExistsA(filename.c_str())) {
        if (std::ofstream write { filename }) {
            //�������߂�
            std::cout << filename << "���쐬���܂����B\n";

            std::tm now = getTm();

            //�{�̏�������
            write
                << "# " << projectName << "�̏��\n\n"
                << "�쐬���F" << now.tm_year + 1900 << "�N" << now.tm_mon + 1 << "��" << now.tm_mday << "��\n\n"

                << "---\n"
                << "## �v���W�F�N�g��\n"
                << "- " << projectName << '\n'
                << "## �^�C�g��\n"
                << "- " << projectName << '\n'
                << "## �V���[�Y\n\n"
                << "- " << path << "  \n"
                << "## ����\n\n"
                << "- ������  \n"
                
                << "---\n"
                << "## �A�b�v�\��\n"
                << "<Yes or No>\n\n"
                << "## ����\n"
                << "- �����ɓ��e������  \n\n"
                << "�g�����f��\n"
                << "- �����ɑf�ނ�����  \n\n"
                ;
            write.close();

            return 0;
        }
        else {
            //�������߂Ȃ�����
            std::cerr << filename << "�̍쐬�Ɏ��s���܂����B\n";
            return true;
        }
    }
    else {
        //���ɍ쐬����Ă���
        std::cout << filename << "�͊��ɍ쐬����Ă��܂��B\n";
        return -2;
    }
}
} //namespace File
