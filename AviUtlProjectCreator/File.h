#pragma once
#define _CRT_SECURE_NO_WARNINGS //std::localtimeを使えるようにする

#include <iostream>
#include <string>
#include <ctime>

#include <fstream>
#include <Windows.h> //Windows API
#include <imagehlp.h>//Windows API
#include <shlwapi.h> //Windows API

#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "shlwapi.lib")

namespace File {
std::string toDirectoryPath(std::string path);                                          //ディレクトリパスの後ろに \ を付ける
int         makeDirectories(const std::string& path);                                   //ディレクトリを作成する
int         changeCurrentDirectory(const std::string& path);                            //カレントディレクトリを移動する

std::tm     getTm() noexcept;                                                           //tm構造体をスマートに取得する
int         writeContentsText(const std::string& path, const std::string& projectName); //概要を書き込んだテキストを作成する
} //namespace File

