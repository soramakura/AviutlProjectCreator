#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <Windows.h>

bool createProjectDir
(       HWND          hDlg
, const std::wstring& projectName
, const std::wstring& projectPath
, const std::wstring& seriesName
);
std::vector<std::wstring> read_adddir_file(std::wifstream&& file);
void createMarkdown
( const std::filesystem::path& basePath
, const std::wstring&          projectName
, const std::wstring&          projectPath
, const std::wstring&          seriesName
);
std::wstring replaceTags
(       std::wstring  line
, const std::wstring& projectName
, const std::wstring& projectPath
, const std::wstring& seriesName
);
