#include "FileDialog.h"

#include <filesystem>
#include <iostream>
#include <Windows.h>

std::filesystem::path FileDialog::Open()
{
    std::string initialPath = std::filesystem::current_path().string();

    OPENFILENAME ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrInitialDir = initialPath.c_str();
    if (GetOpenFileName(&ofn) == TRUE) { return std::filesystem::path(ofn.lpstrFile); }

    return "";
}

std::filesystem::path FileDialog::OpenFolder()
{
    std::string initialPath = std::filesystem::current_path().string();

    OPENFILENAME ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrInitialDir = initialPath.c_str();
    if (GetOpenFileName(&ofn) == TRUE) { return std::filesystem::path(ofn.lpstrFile); }

    return "";
}

std::filesystem::path FileDialog::SaveAs(const std::filesystem::path &defaultPath)
{
    std::string defaultPathString = defaultPath.string();

    OPENFILENAME ofn;
    char szFile[MAX_PATH] = {0};
    strcpy(szFile, defaultPathString.c_str());

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) { return std::filesystem::path(ofn.lpstrFile); }

    return "";
}
