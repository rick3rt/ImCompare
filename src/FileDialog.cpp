#include "FileDialog.h"

#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <shlobj.h>

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

    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = NULL;
    bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
    bi.lpszTitle = "Select Folder";
    bi.lpfn = NULL;
    bi.lParam = (LPARAM)initialPath.c_str();

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL)
    {
        char szPath[MAX_PATH];
        if (SHGetPathFromIDList(pidl, szPath))
        {
            std::string folderPath = szPath;
            return std::filesystem::path(folderPath);
        }
        CoTaskMemFree(pidl);
    }

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

//  OPENFILENAME ofn;
//     char szFile[MAX_PATH] = {0};

//     ZeroMemory(&ofn, sizeof(ofn));
//     ofn.lStructSize = sizeof(ofn);
//     ofn.hwndOwner = NULL;
//     ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
//     ofn.lpstrFile = szFile;
//     ofn.nMaxFile = sizeof(szFile);
//     ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
//     ofn.lpstrInitialDir = initialPath.c_str();
//     if (GetOpenFileName(&ofn) == TRUE) { return std::filesystem::path(ofn.lpstrFile); }
