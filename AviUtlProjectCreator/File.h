#pragma once
#define _CRT_SECURE_NO_WARNINGS //std::localtime���g����悤�ɂ���

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
std::string toDirectoryPath(std::string path);                                          //�f�B���N�g���p�X�̌��� \ ��t����
int         makeDirectories(const std::string& path);                                   //�f�B���N�g�����쐬����
int         changeCurrentDirectory(const std::string& path);                            //�J�����g�f�B���N�g�����ړ�����

std::tm     getTm() noexcept;                                                           //tm�\���̂��X�}�[�g�Ɏ擾����
int         writeContentsText(const std::string& path, const std::string& projectName); //�T�v���������񂾃e�L�X�g���쐬����
} //namespace File

