#include "Explorer.h"
#include "Core.h"

Explorer *Explorer::s_Instance = nullptr;

Explorer::Explorer(std::string title, int w, int h, int argc, char const *argv[])
    : Application(title, w, h, argc, argv)
{
    s_Instance = this;

    // debug
    m_CurrentFile = "C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/src/Explorer.cpp";
    FileSystem::Open(m_CurrentFile, m_Content);
    LOG_INFO("Current File: {}", m_CurrentFile);
}

Explorer::~Explorer() { LOG_INFO("Deleting Explorer"); }

void Explorer::Start() {}

void Explorer::Update()
{
    m_Menu.Update();

    // Path Manager / Pattern Manager / Compare Manger
    m_CompareManager.Update();

    // File browser

    // File Viewer
    m_Viewer.Update();

    // ImGui::Begin("Explorer");
    // ImGui::PushFont(m_fonts["mono"]);
    // ImGui::Text(m_Content.c_str());
    // ImGui::PopFont();
    // ImGui::End();

    // if (ImGui::Begin("demo"))
    // {
    ImGui::ShowDemoWindow();
    ImPlot::ShowDemoWindow();
    // }
    // ImGui::End();
}

void Explorer::Open()
{
    m_CurrentFile = FileDialog::Open();
    LOG_INFO("Opening File: {}", m_CurrentFile);
    FileSystem::Open(m_CurrentFile, m_Content);
}

void Explorer::Save()
{
    LOG_INFO("File Save: {}", m_CurrentFile);
    FileSystem::Save(m_CurrentFile, m_Content);
}

void Explorer::SaveAs()
{
    std::filesystem::path path = FileDialog::SaveAs(m_CurrentFile);
    LOG_INFO("File Save As: {}", path);
    FileSystem::Save(path, m_Content);
}

void Explorer::OnDrop(std::vector<std::string> files)
{
    if (files.size() > 0)
    {
        m_CurrentFile = files[0];
        FileSystem::Open(m_CurrentFile, m_Content);
    }
}