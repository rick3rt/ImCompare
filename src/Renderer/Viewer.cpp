#include "Renderer/Viewer.h"
#include "Core/Core.h"

Viewer::Viewer() : m_SideBySide(true), m_AutoFit(true) {}

Viewer::~Viewer() {}

void Viewer::ListItems()
{
    LOG_TRACE("Available items:");
    for (auto &item : m_Items)
        LOG_TRACE("    Item: {}", item->name);
}

void Viewer::ClearItems() { m_Items.clear(); }

void Viewer::AddItem(const std::string &name, const std::string &info, const std::filesystem::path &path)
{
    // LOG_TRACE("Adding item: {}", name);
    auto item = std::make_shared<ViewData>(name, info, path);
    if (item->isValid())
    {
        m_Items.push_back(item);
        m_SelectedTab = m_Items.size() - 1;
    }

    for (auto &item : m_Items)
    {
        m_AxesLimits.X.Min = 0;
        m_AxesLimits.Y.Min = 0;
        m_AxesLimits.X.Max = item->texture.Width();
        m_AxesLimits.Y.Max = item->texture.Height();
    }

    // ListItems();
    // LOG_TRACE("Added item: {}", name);
}

void Viewer::DrawItem(const int index)
{
    const char *info = m_Items[index]->info.c_str();
    ImGui::Text(info);
    ImageTexture &imgtex = m_Items[index]->texture;
    ImPlotPoint bounds[2] = {{0, 0}, {(double)imgtex.Width(), (double)imgtex.Height()}};
    char buf[32];
    sprintf(buf, "##image%d", index);
    ImPlotFlags flags = ImPlotFlags_Equal | ImPlotFlags_NoTitle;
    ImPlotAxisFlags axis_flags = (ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickLabels |
                                  ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel);
    if (m_AutoFit) axis_flags |= ImPlotAxisFlags_AutoFit;
    if (ImPlot::BeginPlot(buf, ImVec2(-1, -1), flags))
    {
        ImPlot::SetupAxes(NULL, NULL, axis_flags, axis_flags);
        ImPlot::SetupAxisLinks(ImAxis_X1, &m_AxesLimits.X.Min, &m_AxesLimits.X.Max);
        ImPlot::SetupAxisLinks(ImAxis_Y1, &m_AxesLimits.Y.Min, &m_AxesLimits.Y.Max);
        ImPlot::PlotImage(info, (void *)(intptr_t)imgtex.Texture(), bounds[0], bounds[1]);
        ImPlot::EndPlot();
    }
}

void Viewer::DrawSideBySide()
{
    float width = ImGui::GetContentRegionAvail().x / m_Items.size();
    for (int i = 0; i < m_Items.size(); i++)
    {
        const char *name = m_Items[i]->name.c_str();
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
            const char *name = m_Items[i]->name.c_str();
            ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
            if (i >= 0 && i == m_SelectedTab) flags |= ImGuiTabItemFlags_SetSelected;
            if (ImGui::BeginTabItem(name, 0, flags))
            {
                if (ImGui::IsItemClicked()) m_SelectedTab = i;
                DrawItem(i);
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked()) m_SelectedTab = i;
        }
        ImGui::EndTabBar();
    }
}

void Viewer::Update()
{
    if (ImGui::Begin("View Manager"))
    {
        ImGui::Checkbox("Side by Side", &m_SideBySide);
        ImGui::SameLine();
        ImGui::Checkbox("Autofit", &m_AutoFit);
        if (m_SideBySide)
            DrawSideBySide();
        else
            DrawTabbed();
    }
    ImGui::End();
}
