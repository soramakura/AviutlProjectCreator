// Copyright ? 2022 Soramakura. All rights reserved.
// 
// Aviutlで動画を作るとき必要なフォルダを作成するプロジェクト
// 色々なところにちりばめられているフォルダを一気に作成します
// 現在 ver.2.0.0

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#define NOMINMAX //std::numeric_limits<std::streamsize>::max()が #define 扱いされて使えないため
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
    // 動画を作るときに基本階層の一番上になるパス
    std::string movieBasePath;
    switch (readSettingFile("AviUtlProjectCreatorSetup.txt", movieBasePath)) {
    case -1:
        std::cerr << "\aAviUtlProjectCreatorSetup.txtの中身が空でした。\n";
        return -1;
    case -2:
        std::cerr << "\aAviUtlProjectCreatorSetup.txtを開けませんでした。\n";
        return -2;
    default:
        std::cout << "BasePath = " << movieBasePath << "\n\n";
    }

    //プログラムの基本ポジションまで移動
    switch (File::changeCurrentDirectory(movieBasePath)) {
    case -1:
        std::cerr << "\a何らかの理由でディレクトリを移動できませんでした。\n";
        return -3;
    case -2:
        std::cerr << "\a作成する場所となるフォルダがありません。\n作ってから実行してください。\n";
        return -4;
    }

    //合成音声の中で作成するフォルダを選択
    int createVoiceDirectories = 0;
    std::cout << "作成する音声フォルダをを選択してください。(0にすると何も作成されません)\n"
              << "A.I,VOICE >> 1, Yukkuri >> 2, VoiceVox >> 4\n\n"
              << "C++ >> ";
    if (std::cin >> createVoiceDirectories) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else {
        std::cerr << "不正な入力がされました。\n";
        createVoiceDirectories = 0;
    }
        
    //シリーズ名の入力
    std::string seriesPath;
    while (seriesPath == "") {
        std::cout << "シリーズのパスを入力してください(シリーズ名[\\シリーズ名\\...])。\n\n"
                 << "C++ >> ";
        std::getline(std::cin, seriesPath);
    }
    

    //プロジェクト名の入力
    std::string projectName;
    std::cout << "プロジェクトの名前を入力してください(\\は使えません / 空文字列にするとデフォルトの名前になります)。\n\n"
        << "C++ >> ";
    std::getline(std::cin, projectName);
    if (projectName == "") {
        //空文字列だった場合はデフォルトの名前にする
        std::tm now = File::getTm();

        projectName = std::to_string(now.tm_year + 1900) + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday)
        + "_" + std::to_string(now.tm_hour) + std::to_string(now.tm_min) + std::to_string(now.tm_sec);
    }

    //プロジェクト名とシリーズ名を作成するフォルダのリスト
    const std::vector<std::string> ProjectNameHasDirectoriesList
    { movieBasePath + "project\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName)
    , movieBasePath + "project\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName) + "finish\\"
    };

    //フォルダの作成
    for (auto& path : ProjectNameHasDirectoriesList)
        File::makeDirectories(path);

    //合成音声用フォルダの作成
    if (createVoiceDirectories & AiVoice)
        File::makeDirectories(movieBasePath + "material\\voice\\aivoice\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));
    else if (createVoiceDirectories & Yukkuri)
        File::makeDirectories(movieBasePath + "material\\voice\\yukkuri\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));
    else if (createVoiceDirectories & Voicevox)
        File::makeDirectories(movieBasePath + "material\\voice\\voicevox\\" + File::toDirectoryPath(seriesPath) + File::toDirectoryPath(projectName));

    //内容のテキストファイル
    File::writeContentsText(movieBasePath + "project\\" + File::toDirectoryPath(seriesPath), projectName);

    std::cout << "作成は終了しました。\n\n";
    std::system("pause");
}
