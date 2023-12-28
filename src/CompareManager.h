#pragma once
#include <filesystem>
#include <vector>
#include <string>

#include "FileManager.h"

class CompareManager
{
  public:
    CompareManager();
    ~CompareManager();

    void FixBasePath();
    // void WalkDirectory();
    // void UpdateDirectory();
    void Update();

  private:
    FileManager m_FileManager;
    std::string m_BasePath;
    std::vector<std::string> m_FolderContents;
};
