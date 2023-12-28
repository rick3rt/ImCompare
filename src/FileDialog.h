#include <filesystem>
#include <iostream>

class FileDialog
{
  public:
    static std::filesystem::path Open();
    static std::filesystem::path OpenFolder();
    static std::filesystem::path SaveAs(const std::filesystem::path &defaultPath = "");
};