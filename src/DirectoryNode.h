#pragma once

#include <string>
#include <vector>

struct DirectoryNode
{
    std::string FullPath;
    std::string FileName;
    std::vector<DirectoryNode> Children;
    bool IsDirectory;
};
