#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include <imgui.h>
#include <implot.h>

#include "Core/Log.h"
#include "Renderer/ImageLoader.h"

struct ViewData
{
    std::string name;
    std::string info;
    std::filesystem::path path;
    ImageTexture texture;

    ViewData(const std::string &name_, const std::string &info_, const std::filesystem::path &path_)
        : name(name_), info(info_), path(path_), texture(path_)
    {
    }
    ~ViewData() {}
    bool isValid() const { return texture.IsInitialized(); }
};

class Viewer
{
  public:
    Viewer();
    ~Viewer();

    // item management
    void ListItems();
    void ClearItems();
    void AddItem(const std::string &name, const std::string &info, const std::filesystem::path &path);

    void Update(); // draw the viewer

  private: // internal drawing methods
    void DrawItem(const int index);
    void DrawSideBySide();
    void DrawTabbed();

  public:
    // some functions for keyboard control
    void ToggleSideBySide() { m_SideBySide = !m_SideBySide; }
    void ToggleAutoFit() { m_AutoFit = !m_AutoFit; }
    void ShowNextTab()
    {
        m_SelectedTab++;
        if (m_SelectedTab >= m_Items.size()) m_SelectedTab = 0;
    }
    void ShowTab(int index)
    {
        if (index >= 0 && index < m_Items.size()) m_SelectedTab = index;
    }

  private:
    bool m_SideBySide = false;
    bool m_AutoFit = true;
    int m_SelectedTab = -1;
    std::vector<std::shared_ptr<ViewData>> m_Items;
    ImPlotRect m_AxesLimits;
};