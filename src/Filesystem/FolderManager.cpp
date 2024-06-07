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
        if (ImGui::Button("Update")) { UpdateRoot(); }
        ImGui::SameLine();
        ImGui::Text(m_rootNode.FullPath.c_str());
        // ImGui::SameLine();
        // ImGui::Text("CurrentMaxLevel: %i", m_CurrentMaxLevel);
        ImGui::SameLine();
        if (ImGui::Checkbox("Hide Empty Folders", &m_HideEmptyFolders)) { UpdateRoot(); }
        ImGui::SameLine();
        if (ImGui::Button("Print Selection")) { PrintSelection(); }

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
    m_FolderHistory.addFolder(rootPath.string());
    m_rootPath = rootPath;
    UpdateRoot();
}

void FolderManager::UpdateRoot()
{
    ClearFileSelection(); // clear references to old nodes
    ClearSelection();     // clear references to old nodes
    m_rootNode = CreateDirectryNodeTreeFromPath(m_rootPath, m_HideEmptyFolders);
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
}

void FolderManager::RenderFileSelection()
{
    auto &viewer = Explorer::Get().GetViewer();
    viewer.ClearItems();
    LOG_TRACE("RenderFileSelection");
    for (auto &sf : m_SelectedFiles)
    {
        LOG_TRACE("Selected file: level {} idx {} name {}", sf.level, sf.index, sf.node->FileName);
        const DirectoryNode *node = sf.node;
        if (!node || node->IsDirectory) { continue; } // skip directories
        std::string filename = node->FileName;
        std::filesystem::path path = node->FullPath;
        std::string info = node->FileName;
        viewer.AddItem(filename, info, path);
    }
}

/**
 * @brief Check if the file is in the selection. return the index if found.
 * Returns -1 if nothing found.
 */
int fileInSelection(const std::vector<SelectedFileRef> &selection, int level, int index,
                    const DirectoryNode *node)
{
    int i = 0;
    for (const auto &file : selection)
    {
        if (file.level == level && file.index == index && file.node == node) { return i; }
        i++;
    }
    return -1;
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

            // check if child is directory and hide empty folders (DOESNT WORK)
            // if (child.IsDirectory && child.Children.empty() && m_HideEmptyFolders) { continue; }

            // const bool show_selected = (is_selected || found);
            if (ImGui::Selectable(child.FileName.c_str(), is_selected))
            {
                bool keyAltDown = ImGui::GetIO().KeyAlt;
                bool keyCtrlDown = ImGui::GetIO().KeyCtrl;
                LOG_TRACE("Control down {0} - Alt down {0}", keyCtrlDown, keyAltDown);

                if (keyCtrlDown) m_SelectedFiles.clear();

                m_Selection[level] = std::make_pair(i, &child);
                PropagateSelection(level); // update other child nodes

                // if child is file, add to selected files
                int found_idx = fileInSelection(m_SelectedFiles, level, i, &child);
                if (!child.IsDirectory && found_idx == -1 && !keyAltDown)
                {
                    m_SelectedFiles.push_back(SelectedFileRef(level, i, &child));
                } else if (!child.IsDirectory && found_idx >= 0 && keyAltDown)
                {
                    // already found, check for alt down to remove entry
                    m_SelectedFiles.erase(m_SelectedFiles.begin() + found_idx);
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

void FolderManager::IncrementFolder(int level, int increment /* = 1 */)
{

    LOG_TRACE("IncrementFolder: level {}", level);
    for (int i = level; i < m_CurrentMaxLevel; i++)
    {
        LOG_TRACE("IncrementFolder: level {} index {} node {}", i, m_Selection[i].first,
                  m_Selection[i].second->FileName);
    }

    const DirectoryNode *parent;
    if (level > 0)
        parent = m_Selection[level - 1].second;
    else
        parent = &m_rootNode;

    int index = (m_Selection[level].first + increment) % parent->Children.size();
    m_Selection[level].first = index;
    m_Selection[level].second = &(parent->Children[index]);

    PropagateSelection(level);
    RenderFileSelection();
}

bool str_replace(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos) return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void FolderManager::PrintSelection()
{

    if (m_SelectedFiles.empty()) return;
    auto sf = m_SelectedFiles[0];
    std::string path = sf.node->FullPath;
    // replace part of path, replace rootPath by nothing
    str_replace(path, m_rootPath.string(), "");
    LOG_INFO("{}", path);
}

// =============================================================================
// static functions
// =============================================================================

DirectoryNode FolderManager::CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath,
                                                            bool ignoreEmpty)
{
    DirectoryNode node;
    node.FullPath = rootPath.string();
    node.FileName = rootPath.filename().string();
    node.IsDirectory = true;

    std::filesystem::directory_iterator it(rootPath);
    RecursivelyAddDirectoryNodes(node, it, ignoreEmpty);

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
                                                 std::filesystem::directory_iterator directoryIterator,
                                                 bool ignoreEmpty)
{
    // directoryIterator
    for (const auto &entry : directoryIterator)
    {
        DirectoryNode node;
        node.FullPath = entry.path().string();
        node.FileName = entry.path().filename().string();
        node.IsDirectory = entry.is_directory();
        // -optional- dont add empty nodes

        bool isempty = std::filesystem::is_empty(entry.path());
        if (ignoreEmpty && isempty) { continue; }

        parentNode.Children.push_back(node);
        if (entry.is_directory())
        {
            std::filesystem::directory_iterator it(entry);
            RecursivelyAddDirectoryNodes(parentNode.Children.back(), it, ignoreEmpty);
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
