#include "Explorer.h"
#include "Core/Core.h"

Explorer *Explorer::s_Instance = nullptr;

Explorer::Explorer(std::string title, int w, int h, int argc, char const *argv[])
    : Application(title, w, h, argc, argv)
{
    s_Instance = this;

    // set default path
}

Explorer::~Explorer() { LOG_INFO("Deleting Explorer"); }

void Explorer::Start()
{

    m_FolderManager.SetRoot("C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/resource/example/");
}

void Explorer::Update()
{
    m_Menu.Update();
    m_FolderManager.Update();
    m_Viewer.Update();

    // ImGui::ShowDemoWindow();
    // ImPlot::ShowDemoWindow();
}

void Explorer::OpenFolder() { m_FolderManager.SetRoot(FileDialog::OpenFolder()); }

void Explorer::SaveSession()
{
    // LOG_INFO("File Save: {}", m_CurrentFile);
    std::filesystem::path path = FileDialog::SaveAs();
    LOG_INFO("File Save As: {}", path);
    // FileSystem::Save("test.txt", "blablabla");
}

// void Explorer::SaveAs()
// {
//     std::filesystem::path path = FileDialog::SaveAs(m_CurrentFile);
//     LOG_INFO("File Save As: {}", path);
//     FileSystem::Save(path, m_Content);
// }

void Explorer::OnDrop(std::vector<std::string> files)
{
    if (files.size() > 0) { m_FolderManager.SetRoot(files[0]); }
}