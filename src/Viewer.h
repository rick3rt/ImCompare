#pragma once
#include <vector>
#include <string>
#include <memory>
#include <imgui.h>
#include <implot.h>
#include "ImageLoader.h"

struct ViewData
{
    std::string name;
    std::string info;
    std::shared_ptr<ImageTexture> texture;
};

class Viewer
{
  public:
    Viewer();
    ~Viewer();

    void DrawItem(const int index);

    void DrawSideBySide();

    void DrawTabbed();

    void Update();

  private:
    bool m_SideBySide = false;
    std::vector<ViewData> m_Items;
    std::shared_ptr<ImageTexture> m_ImageTexture;
    ImPlotRect m_AxesLimits;
};