#pragma once
#include <filesystem>

#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Menu.h"
#include "Renderer/Viewer.h"
// #include "CompareManager.h"
#include "Filesystem/FolderManager.h"

class Explorer : public Application
{
  public:
    Explorer(std::string title, int w, int h, int argc, char const *argv[]);
    ~Explorer();
    inline static Explorer &Get() { return *s_Instance; }

    void Start() override;
    void Update() override;

    void OpenFolder();
    void SaveSession();

    virtual void OnDrop(std::vector<std::string> files) override; // dropping files from OS

    void SetBasePath(std::filesystem::path path);

  public:
    // getters
    inline Viewer &GetViewer() { return m_Viewer; }
    inline FolderManager &GetFolderManager() { return m_FolderManager; }

  private:
    Menu m_Menu;
    Viewer m_Viewer;
    // CompareManager m_CompareManager;
    FolderManager m_FolderManager;

  private:
    static Explorer *s_Instance;
};
