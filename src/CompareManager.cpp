#include "CompareManager.h"
#include "Core.h"

#include "FileManager.h"
#include "misc/cpp/imgui_stdlib.h"

// ==================================================================================
void DirectoryNodeCallback(DirectoryNode &outputNode)
{
    LOG_INFO("DirectoryNodeCallback: {}", outputNode.FullPath);
}

// ==================================================================================

CompareManager::CompareManager()
{
    m_BasePath = std::filesystem::current_path().string();
    m_FileManager.SetRoot(m_BasePath);
}

CompareManager::~CompareManager() {}

// void CompareManager::UpdateDirectory() { WalkDirectory(); }

void CompareManager::FixBasePath()
{
    // replace all / with \ in basepath
    std::replace(m_BasePath.begin(), m_BasePath.end(), '/', '\\');

    // if basepath does not end with / or \, add it
    if (m_BasePath.back() != '\\' && m_BasePath.back() != '\\') { m_BasePath += '\\'; }
}

void CompareManager::Update()
{
    // struct UpdateCallbacks
    // {
    //     static int PathUpdateCallback(ImGuiInputTextCallbackData *data)
    //     {
    //         CompareManager *ptr = (CompareManager *)data->UserData;
    //         ptr->WalkDirectory();
    //         return 0;
    //     }
    // };

    if (ImGui::Begin("Compare Manager"))
    {
        if (ImGui::InputText("Base Path", &m_BasePath, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // updated. walk directory
            FixBasePath();
            // WalkDirectory();
            m_FileManager.SetRoot(m_BasePath);
        }
        // for (auto &path : m_FolderContents)
        // {
        //     ImGui::Text(path.c_str());
        // }
        DirectoryNode selectedNode;
        if (m_FileManager.RecursivelyDisplayDirectoryNode(m_FileManager.GetRootNode(),
                                                          DirectoryNodeCallback, selectedNode))
        {
            LOG_INFO("RecursivelyDisplayDirectoryNode Clicked");
            LOG_INFO("Selected Node: {}", selectedNode.FullPath);
        }
    }
    ImGui::End();

    if (ImGui::Begin("Demo")) {}
    ImGui::End();
}

// void CompareManager::WalkDirectory()
// {
//     m_FolderContents.clear();
//     // check if basepath exists
//     if (!std::filesystem::exists(m_BasePath))
//     {
//         LOG_ERROR("Base path does not exist: {0}", m_BasePath);
//         return;
//     }
//     std::filesystem::recursive_directory_iterator dirIter(m_BasePath);
//     for (const auto &entry : dirIter)
//     {
//         std::string path = entry.path().string();
//         // Strip the base path
//         path = path.substr(m_BasePath.size());
//         // items that are directories and start with a .
//         if (entry.is_directory() && path[0] == '.') { continue; }
//         m_FolderContents.push_back(path);
//     }
// }
