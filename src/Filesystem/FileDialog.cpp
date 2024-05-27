#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <shlobj.h>
#include "Filesystem/FileDialog.h"

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
    LPCWSTR initialPathWstr = std::wstring(initialPath.begin(), initialPath.end()).c_str();

    IFileDialog *pFileDialog;
    HRESULT hr =
        CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (SUCCEEDED(hr))
    {
        DWORD dwOptions;
        pFileDialog->GetOptions(&dwOptions);
        pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

        COMDLG_FILTERSPEC filterSpec[] = {{L"All Files", L"*.*"}};
        pFileDialog->SetFileTypes(ARRAYSIZE(filterSpec), filterSpec);

        pFileDialog->SetTitle(L"Select Folder");
        pFileDialog->SetFileName(initialPathWstr);

        hr = pFileDialog->Show(NULL);
        if (SUCCEEDED(hr))
        {
            IShellItem *pShellItem;
            hr = pFileDialog->GetResult(&pShellItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFolderPath;
                hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
                if (SUCCEEDED(hr))
                {
                    std::wstring folderPathWstr(pszFolderPath);
                    // std::string folderPath(folderPathWstr.begin(), folderPathWstr.end());
                    CoTaskMemFree(pszFolderPath);
                    pShellItem->Release();
                    pFileDialog->Release();
                    return std::filesystem::path(pszFolderPath);
                }
                pShellItem->Release();
            }
        }
        pFileDialog->Release();
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
