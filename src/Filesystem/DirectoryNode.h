#pragma once

#include <string>
#include <vector>

struct DirectoryNode
{
    std::string FullPath;
    std::string FileName;
    std::vector<DirectoryNode> Children;
    bool IsDirectory;

    DirectoryNode() : FullPath(""), FileName(""), IsDirectory(false) {}
    DirectoryNode(const std::string &fileName, const std::string &fullPath, bool isDirectory = false)
        : FullPath(fullPath), FileName(fileName), IsDirectory(isDirectory)
    {
    }

    bool isDirectory() const { return IsDirectory; }
};
