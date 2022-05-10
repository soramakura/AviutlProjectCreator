#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

bool createProject
( const std::string& projectName
, const std::string& projectPath
, const std::string& seriesName
);
std::vector<std::string> read_adddir_file(std::ifstream&& file);
void createMarkdown
( const std::filesystem::path& basePath
, const std::string&          projectName
, const std::string&          projectPath
, const std::string&          seriesName
);
std::string replaceTags
(       std::string  line
, const std::string& projectName
, const std::string& projectPath
, const std::string& seriesName
);
