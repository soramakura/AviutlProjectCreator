// Copyright ? 2022 Soramakura. All rights reserved.
// 
// Aviutl�œ�������Ƃ��K�v�ȃt�H���_���쐬����v���W�F�N�g
// �F�X�ȂƂ���ɂ���΂߂��Ă���t�H���_����C�ɍ쐬���܂�
// ���� ver.2.0.0

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#define NOMINMAX //std::numeric_limits<std::streamsize>::max()�� #define ��������Ďg���Ȃ�����
#include <Windows.h>

#include "File.h"

enum VoiceDirectory
{ None     = 0
, AiVoice  = 1
, Yukkuri  = 2
, Voicevox = 4
};

int readSettingFile(const std::string& fileName, std::string& movieBasePath)
{
    std::ifstream settingFile("AviUtlProjectCreatorSetup.txt");
    if (settingFile.is_open()) {
        std::getline(settingFile, movieBasePath);
        if (movieBasePath == "" || settingFile.fail())
            return -1;
    }
    else
        return -2;

    return 0;
}

int main()
{
    // ��������Ƃ��Ɋ�{�K�w�̈�ԏ�ɂȂ�p�X
    std::string movieBasePath;
    switch (readSettingFile("AviUtlProjectCreatorSetup.txt", movieBasePath)) {
    case -1:
        std::cerr << "\aAviUtlProjectCreatorSetup.txt�̒��g����ł����B\n";
        return -1;
    case -2:
        std::cerr << "\aAviUtlProjectCreatorSetup.txt���J���܂���ł����B\n";
        return -2;
    default:
        std::cout << "BasePath = " << movieBasePath << "\n\n";
    }

    //�v���O�����̊�{�|�W�V�����܂ňړ�
    switch (File::changeCurrentDirectory(movieBasePath)) {
    case -1:
        std::cerr << "\a���炩�̗��R�Ńf�B���N�g�����ړ��ł��܂���ł����B\n";
        return -3;
    case -2:
        std::cerr << "\a�쐬����ꏊ�ƂȂ�t�H���_������܂���B\n����Ă�����s���Ă��������B\n";
        return -4;
    }

    //���������̒��ō쐬����t�H���_��I��
    int createVoiceDirectories = 0;
    std::cout << "�쐬���鉹���t�H���_����I�����Ă��������B(0�ɂ���Ɖ����쐬����܂���)\n"
              << "A.I,VOICE >> 1, Yukkuri >> 2, VoiceVox >> 4\n\n"
              << "C++ >> ";
    if (std::cin >> createVoiceDirectories) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else {
        std::cerr << "�s���ȓ��͂�����܂����B\n";
        createVoiceDirectories = 0;
    }
        
    //�V���[�Y���̓���
    std::string seriesPath;
    while (seriesPath == "") {
        std::cout << "�V���[�Y�̃p�X����͂��Ă�������(�V���[�Y��[\\�V���[�Y��\\...])�B\n\n"
                 << "C++ >> ";
        std::getline(std::cin, seriesPath);
    }
    

    //�v���W�F�N�g���̓���
    std::string projectName;
    std::cout << "�v���W�F�N�g�̖��O����͂��Ă�������(\\�͎g���܂��� / �󕶎���ɂ���ƃf�t�H���g�̖��O�ɂȂ�܂�)�B\n\n"
        << "C++ >> ";
    std::getline(std::cin, projectName);
    if (projectName == "") {
        //�󕶎��񂾂����ꍇ�̓f�t�H���g�̖��O�ɂ���
        std::tm now = File::getTm();

        projectName = std::to_string(now.tm_year + 1900) + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday)
        + "_" + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec);
    }

    //�v���W�F�N�g���ƃV���[�Y�����쐬����t�H���_�̃��X�g
    const std::vector<std::string> ProjectNameHasDirectoriesList
    { movieBasePath + "project\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName)
    , movieBasePath + "project\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName) + "finish\\"
    };

    //�t�H���_�̍쐬
    for (auto& path : ProjectNameHasDirectoriesList)
        File::makeDirectories(path);

    //���������p�t�H���_�̍쐬
    if (createVoiceDirectories & AiVoice)
        File::makeDirectories(movieBasePath + "material\\voice\\aivoice\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));
    else if (createVoiceDirectories & Yukkuri)
        File::makeDirectories(movieBasePath + "material\\voice\\yukkuri\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));
    else if (createVoiceDirectories & Voicevox)
        File::makeDirectories(movieBasePath + "material\\voice\\voicevox\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));

    //���e�̃e�L�X�g�t�@�C��
    File::writeContentsText(movieBasePath + "project\\" + File::toDirectoryPath(seriesPath), projectName);

    std::cout << "�쐬�͏I�����܂����B\n\n";
    std::system("pause");
}
