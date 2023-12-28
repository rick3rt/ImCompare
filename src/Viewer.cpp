#include "Viewer.h"
#include "Core.h"

Viewer::Viewer()
{
    m_Items.push_back({"Viewer1", "Data1", std::make_shared<ImageTexture>("resource/img/img3.png")});
    m_Items.push_back({"Viewer2", "Data2", std::make_shared<ImageTexture>("resource/img/img3.png")});
    m_Items.push_back({"Viewer3", "Data3", std::make_shared<ImageTexture>("resource/img/img3.png")});

    for (auto &item : m_Items)
    {
        m_AxesLimits.X.Min = 0;
        m_AxesLimits.Y.Min = 0;
        m_AxesLimits.X.Max = item.texture->width;
        m_AxesLimits.Y.Max = item.texture->height;
    }
}
Viewer::~Viewer() {}

void Viewer::DrawItem(const int index)
{
    const char *info = m_Items[index].info.c_str();
    ImGui::Text(info);
    ImageTexture *imgtex = m_Items[index].texture.get();
    ImPlotPoint bounds[2] = {{0, 0}, {(double)imgtex->width, (double)imgtex->height}};
    char buf[32];
    sprintf(buf, "##image%d", index);
    ImPlotFlags flags = ImPlotFlags_Equal | ImPlotFlags_NoTitle;
    ImPlotAxisFlags axis_flags = (ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickLabels |
                                  ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel);
    if (ImPlot::BeginPlot(buf, ImVec2(-1, -1), flags))
    {
        ImPlot::SetupAxes(NULL, NULL, axis_flags, axis_flags);
        ImPlot::SetupAxisLinks(ImAxis_X1, &m_AxesLimits.X.Min, &m_AxesLimits.X.Max);
        ImPlot::SetupAxisLinks(ImAxis_Y1, &m_AxesLimits.Y.Min, &m_AxesLimits.Y.Max);
        ImPlot::PlotImage(info, (void *)(intptr_t)imgtex->texture, bounds[0], bounds[1]);
        ImPlot::EndPlot();
    }
}

void Viewer::DrawSideBySide()
{
    float width = ImGui::GetContentRegionAvail().x / m_Items.size();
    for (int i = 0; i < m_Items.size(); i++)
    {
        const char *name = m_Items[i].name.c_str();
        char buf[32];
        sprintf(buf, "ViewerPort%d", i);
        ImGui::BeginChild(buf, ImVec2(width, 0));
        if (ImGui::BeginTabBar("ViewerTab"))
        {
            if (ImGui::BeginTabItem(name))
            {
                DrawItem(i);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();
        ImGui::SameLine();
    }
}

void Viewer::DrawTabbed()
{
    if (ImGui::BeginTabBar("ViewerTab"))
    {
        for (size_t i = 0; i < m_Items.size(); i++)
        {
            const char *name = m_Items[i].name.c_str();
            if (ImGui::BeginTabItem(name))
            {
                DrawItem(i);
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

void Viewer::Update()
{
    if (ImGui::Begin("View Manager"))
    {
        ImGui::Checkbox("Side by Side", &m_SideBySide);
        if (m_SideBySide)
            DrawSideBySide();
        else
            DrawTabbed();
    }
    ImGui::End();
}
