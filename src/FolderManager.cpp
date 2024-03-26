#include "FolderManager.h"
#include "FileDialog.h"
#include "Log.h"
#include <imgui.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// selection
//         0        0       0       1
// depth   0        1       2       3
//         resource
//         |        example
//         |        |       filename
//         |        |       folders
//         |        |       |       Medium1
//         |        |       |       Medium2
//         |        |       blank.jpg
//         |        fonts
//         |        img

// std::vector<const DirectoryNode &> selection

bool FolderManager::Update()
{
    if (ImGui::Begin("Explorer"))
    {
        if (ImGui::Button("Set Root"))
        {
            std::filesystem::path rootPath = FileDialog::OpenFolder();
            SetRoot(rootPath);
        }
        ImGui::SameLine();
        ImGui::Text(m_rootNode.FullPath.c_str());
        RecursivelyDisplayDirectoryNode(GetRootNode());
    }
    ImGui::End();
    return true;
}

void FolderManager::SetRoot(const std::filesystem::path &rootPath)
{
    // check if root path exists
    if (!std::filesystem::exists(rootPath))
    {
        LOG_ERROR("Root path does not exist: {0}", m_rootPath.string());
        return;
    }
    m_rootPath = rootPath;
    m_rootNode = CreateDirectryNodeTreeFromPath(m_rootPath);
}

bool FolderManager::RecursivelyPrintDirectoryNode(const DirectoryNode &parentNode, int level)
{
    for (const auto &child : parentNode.Children)
    {
        std::string indent(level * 4, ' ');
        LOG_INFO("{}{}", indent, child.FileName);
        if (child.Children.size() > 0) { RecursivelyPrintDirectoryNode(child, level + 1); }
    }
    return true;
}

void FolderManager::PropagateSelection(int level)
{
    for (int i = level + 1; i < m_Selection.size(); i++)
    {
        int prev_index = m_Selection[i - 1].first;
        const DirectoryNode *parent = m_Selection[i - 1].second;
        if (parent == nullptr) { continue; }
        const std::vector<DirectoryNode> &children = parent->Children;
        if (children.empty()) { continue; }
        int index = MIN(m_Selection[i].first, children.size() - 1);
        m_Selection[i].first = index;
        m_Selection[i].second = &children[index];
    }
}

bool FolderManager::RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level)
{
    bool clicked = false;
    std::string listbox_label = parentNode.FileName;
    if (listbox_label.empty()) { listbox_label = "Root"; }
    listbox_label = "##" + listbox_label;

    if (ImGui::BeginListBox(listbox_label.c_str(), ImVec2(400, 250)))
    {
        for (int i = 0; i < parentNode.Children.size(); i++)
        {
            const DirectoryNode &child = parentNode.Children[i];
            const bool is_selected = (m_Selection[level].first == i);
            // optional todo:
            //  move files to the bottom. either in render or sort when constructing the tree
            if (ImGui::Selectable(child.FileName.c_str(), is_selected))
            {
                m_Selection[level] = std::make_pair(i, &child);
                clicked = true;
                PropagateSelection(level); // update other child nodes but keep
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::SameLine();
    const DirectoryNode *child = m_Selection[level].second;
    if (child != nullptr && child->IsDirectory) RecursivelyDisplayDirectoryNode(*child, level + 1);
    return clicked;
}

void FolderManager::RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                                 std::filesystem::directory_iterator directoryIterator)
{
    for (const auto &entry : directoryIterator)
    {
        DirectoryNode node;
        node.FullPath = entry.path().string();
        node.FileName = entry.path().filename().string();
        node.IsDirectory = entry.is_directory();
        parentNode.Children.push_back(node);
        if (entry.is_directory())
        {
            std::filesystem::directory_iterator it(entry);
            RecursivelyAddDirectoryNodes(parentNode.Children.back(), it);
        }
    }
}

DirectoryNode FolderManager::CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath)
{
    DirectoryNode node;
    node.FullPath = rootPath.string();
    node.FileName = rootPath.filename().string();
    node.IsDirectory = true;

    std::filesystem::directory_iterator it(rootPath);
    RecursivelyAddDirectoryNodes(node, it);

    return node;
}

void FolderManager::RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files)
{
    for (const auto &child : node.Children)
    {
        if (child.IsDirectory)
        {
            RecursivelyGetFiles(child, files);
        } else
        {
            files.push_back(child);
        }
    }
}

// Path: src/FileManager.cpp
// Compare this snippet from src/main.cpp:
// #include "Log.h"
// #include "Explorer.h"
//
// void runApp()
// {
//     Explorer app("DataExplorer", 1280, 760, 0, nullptr);
//     app.Run();
// }
//
// int main()
// {
//     Log::Init(); // initialize logger
//
//     // runApp();
//
//     LOG_INFO("Hello World!");
//
//
//     return 0;
// }
// #include "FileManager.h"
