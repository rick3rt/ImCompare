#pragma once
#include <filesystem>
#include <string>
#include <fstream>

#include "Log.h"

class FileSystem
{
  public:
    static void Save(std::filesystem::path path, std::string content)
    {
        std::ofstream file(path);
        file << content;
        file.close();
    }

    static void Open(std::filesystem::path path, std::string &content)
    {
        // check if file exists
        if (!std::filesystem::exists(path))
        {
            LOG_ERROR("File does not exist: {}", path.string());
            return;
        }

        content.clear();
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line))
        {
            content += line + '\n';
        }
        file.close();
    }
};