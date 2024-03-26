#include "Explorer.h"
#include "Core.h"

Explorer *Explorer::s_Instance = nullptr;

Explorer::Explorer(std::string title, int w, int h, int argc, char const *argv[])
    : Application(title, w, h, argc, argv)
{
    s_Instance = this;
}

Explorer::~Explorer() { LOG_INFO("Deleting Explorer"); }

void Explorer::Start()
{

    m_FolderManager.SetRoot("C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/resource/example/");
}

void Explorer::Update()
{
    // m_Menu.Update();

    // // Path Manager / Pattern Manager / Compare Manger
    // m_CompareManager.Update();

    // // Image Viewer
    // m_Viewer.Update();

    m_FolderManager.Update();

    if (ImGui::Begin("View Manager"))
    {
        static bool test = false;
        ImGui::Checkbox("Side by Side", &test);
    }
    ImGui::End();

    ImGui::ShowDemoWindow();
    // ImPlot::ShowDemoWindow();
}

void Explorer::OpenFolder() { m_CompareManager.SetBasePath(FileDialog::OpenFolder()); }

void Explorer::SaveSession()
{
    // LOG_INFO("File Save: {}", m_CurrentFile);
    // FileSystem::Save(m_CurrentFile, m_Content);
}

// void Explorer::SaveAs()
// {
//     std::filesystem::path path = FileDialog::SaveAs(m_CurrentFile);
//     LOG_INFO("File Save As: {}", path);
//     FileSystem::Save(path, m_Content);
// }

void Explorer::OnDrop(std::vector<std::string> files)
{
    if (files.size() > 0) { m_CompareManager.SetBasePath(files[0]); }
}