#pragma once
#include <filesystem>

#include "Log.h"
#include "Application.h"
#include "Menu.h"
#include "Viewer.h"
#include "CompareManager.h"

class Explorer : public Application
{
  public:
    Explorer(std::string title, int w, int h, int argc, char const *argv[]);
    ~Explorer();
    inline static Explorer &Get() { return *s_Instance; }

    void Start() override;
    void Update() override;

    void Open();
    void Save();
    void SaveAs();

    virtual void OnDrop(std::vector<std::string> files) override; // dropping files from OS

  private:
    std::filesystem::path m_CurrentFile;
    std::string m_Content;

  private:
    Menu m_Menu;
    Viewer m_Viewer;
    CompareManager m_CompareManager;

  private:
    static Explorer *s_Instance;
};
