#include <imgui.h>
#include "Core/Log.h"
#include "Explorer.h"
#include "Filesystem/FolderManager.h"
#include "Filesystem/FileDialog.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

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
        ImGui::SameLine();
        ImGui::Text("CurrentMaxLevel: %i", m_CurrentMaxLevel);

        bool file_clicked = RecursivelyDisplayDirectoryNode(GetRootNode());
        if (file_clicked) { RenderFileSelection(); }
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
    ClearFileSelection(); // clear references to old nodes
    ClearSelection();     // clear references to old nodes
    m_rootNode = CreateDirectryNodeTreeFromPath(m_rootPath);
}

void FolderManager::ClearSelection()
{
    // clear and reset
    for (auto &selection : m_Selection)
        selection = std::make_pair(0, nullptr);
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

    // update file selection
    for (auto &file : m_SelectedFiles)
    {
        const auto &new_node = m_Selection[file.level - 1].second;
        if (new_node)
        {
            const auto &children = new_node->Children;
            if (children.size() > file.index)
            {
                file.node = &children[file.index];
                continue;
            }
        }

        // delete file from m_SelectedFiles vector
        for (auto it = m_SelectedFiles.begin(); it != m_SelectedFiles.end();)
        {
            it = m_SelectedFiles.erase(it);
        }
    }

    // for (int i = level + 1; i < m_Selection.size()-1; i++)
    // {
    //     const DirectoryNode *children = m_Selection[i].second;
    //     if (children == nullptr) { continue; }

    // }
}

void FolderManager::RenderFileSelection()
{
    auto &viewer = Explorer::Get().GetViewer();
    viewer.ClearItems();
    LOG_DEBUG("RenderFileSelection");
    for (auto &sf : m_SelectedFiles)
    {
        LOG_DEBUG("Selected file: level {} idx {} name {}", sf.level, sf.index, sf.node->FileName);
        const DirectoryNode *node = sf.node;
        if (!node || node->IsDirectory) { continue; } // skip directories
        std::string filename = node->FileName;
        std::filesystem::path path = node->FullPath;
        std::string info = node->FileName;
        viewer.AddItem(filename, info, path);
    }
}

bool fileInSelection(const std::vector<SelectedFileRef> &selection, int level, int index,
                     const DirectoryNode *node)
{
    for (const auto &file : selection)
    {
        if (file.level == level && file.index == index && file.node == node) { return true; }
    }
    return false;
}

bool FolderManager::RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level)
{
    bool clicked = false;
    std::string listbox_label = parentNode.FileName;
    if (listbox_label.empty()) { listbox_label = "Root"; }
    listbox_label = "##" + listbox_label;

    if (ImGui::BeginListBox(listbox_label.c_str(), ImVec2(200, 150)))
    {
        for (int i = 0; i < parentNode.Children.size(); i++)
        {
            const DirectoryNode &child = parentNode.Children[i];
            const bool is_selected = (m_Selection[level].first == i);
            // const bool show_selected = (is_selected || found);
            if (ImGui::Selectable(child.FileName.c_str(), is_selected))
            {
                m_Selection[level] = std::make_pair(i, &child);
                PropagateSelection(level); // update other child nodes

                // if child is file, add to selected files
                const bool found = fileInSelection(m_SelectedFiles, level, i, &child);
                if (!child.IsDirectory && !found)
                {
                    // detect if cntrl is pressed
                    if (!ImGui::GetIO().KeyCtrl)
                        m_SelectedFiles.push_back(SelectedFileRef(level, i, &child));
                    else
                    {
                        // remove file from selection
                        for (auto it = m_SelectedFiles.begin(); it != m_SelectedFiles.end();)
                        {
                            if (it->level == level && it->index == i && it->node == &child)
                                it = m_SelectedFiles.erase(it);
                            else
                                ++it;
                        }
                    }
                }
                clicked = true;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::SameLine();
    const DirectoryNode *child = m_Selection[level].second;
    if (child != nullptr && child->IsDirectory)
        clicked |= RecursivelyDisplayDirectoryNode(*child, level + 1);
    else
        m_CurrentMaxLevel = level; // set max level
    return clicked;
}

void FolderManager::IncrementFolder(int level)
{
    // typedef std::pair<int, const DirectoryNode *> indexed_selection_t;
    // typedef std::array<indexed_selection_t, 20> selection_t;
    // selection_t m_Selection;
    LOG_DEBUG("IncrementFolder: level {}", level);
    for (int i = level; i < m_CurrentMaxLevel; i++)
    {
        LOG_DEBUG("IncrementFolder: level {} index {} node {}", i, m_Selection[i].first,
                  m_Selection[i].second->FileName);
    }

    const DirectoryNode *parent;
    if (level > 0)
        parent = m_Selection[level - 1].second;
    else
        parent = &m_rootNode;

    int index = (m_Selection[level].first + 1) % parent->Children.size();
    m_Selection[level].first = index;
    m_Selection[level].second = &(parent->Children[index]);

    PropagateSelection(level);
    RenderFileSelection();
}

// static functions

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

void FolderManager::RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                                 std::filesystem::directory_iterator directoryIterator)
{
    // directoryIterator
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

    // sort parentNode.Children so directories are first
    auto moveDirectoriesToFront = [](const DirectoryNode &a, const DirectoryNode &b) {
        return (a.IsDirectory > b.IsDirectory);
    };
    std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);

    // does nork work because need to recreate the iterator
    //   // directoryIterator
    // // loop over all files in the directory
    // // first handle directories
    // for (const auto &entry : directoryIterator)
    // {
    //     if (!entry.is_directory()) { continue; }
    //     DirectoryNode node;
    //     node.FullPath = entry.path().string();
    //     node.FileName = entry.path().filename().string();
    //     node.IsDirectory = entry.is_directory();
    //     parentNode.Children.push_back(node);
    //     std::filesystem::directory_iterator it(entry);
    //     RecursivelyAddDirectoryNodes(parentNode.Children.back(), it);
    // }
    // // then files
    // for (const auto &entry : directoryIterator)
    // {
    //     if (entry.is_directory()) { continue; }
    //     DirectoryNode node;
    //     node.FullPath = entry.path().string();
    //     node.FileName = entry.path().filename().string();
    //     node.IsDirectory = entry.is_directory();
    //     parentNode.Children.push_back(node);
    // }
}

void FolderManager::RecursivelyPrintDirectoryNode(const DirectoryNode &parentNode, int level)
{
    for (const auto &child : parentNode.Children)
    {
        std::string indent(level * 4, ' ');
        LOG_INFO("{}{}", indent, child.FileName);
        if (child.Children.size() > 0) { RecursivelyPrintDirectoryNode(child, level + 1); }
    }
}
