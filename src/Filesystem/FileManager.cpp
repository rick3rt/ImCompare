#include <imgui.h>
#include "Core/Log.h"
#include "Filesystem/FileManager.h"

void FileManager::SetRoot(const std::filesystem::path &rootPath)
{
    m_rootPath = rootPath;
    // check if * wildcard is in rootpath
    if (m_rootPath.filename().string().find("*") != std::string::npos)
    {
        m_Filter = m_rootPath.filename().u8string();
        // remove * from filter
        m_Filter.erase(std::remove(m_Filter.begin(), m_Filter.end(), '*'), m_Filter.end());
        m_rootPath = m_rootPath.parent_path();
        LOG_TRACE("Filter: {}", m_Filter);
        LOG_TRACE("RootPath: {}", m_rootPath);
    } else
    {
        m_Filter = "";
    }
    // check if path exists
    if (!std::filesystem::exists(m_rootPath))
    {
        LOG_ERROR("Path does not exist: {}", m_rootPath);
        return;
    }
    m_rootPath = std::filesystem::canonical(m_rootPath);
    m_rootNode = CreateDirectryNodeTreeFromPath(m_rootPath);
}

// returns true if an item is clicked
bool FileManager::RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level)
{
    bool node_clicked = false;
    const ImGuiTreeNodeFlags folder_flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                            ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                            ImGuiTreeNodeFlags_SpanAvailWidth;
    const ImGuiTreeNodeFlags file_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                          ImGuiTreeNodeFlags_SpanAvailWidth;
    const DirectoryNode *clickedNode = nullptr;
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1.0f);
    ImGui::PushID(&parentNode);
    if (parentNode.IsDirectory)
    {
        // render direcotry node
        ImGuiTreeNodeFlags node_flags = folder_flags;
        if (level == 0) node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
        // check if parentNode is in m_Selection
        if (std::find(m_Selection.begin(), m_Selection.end(), &parentNode) != m_Selection.end())
            node_flags |= ImGuiTreeNodeFlags_Selected;
        bool node_open = ImGui::TreeNodeEx(parentNode.FileName.c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) clickedNode = &parentNode;
        if (node_open)
        {
            for (const DirectoryNode &childNode : parentNode.Children)
                node_clicked |= RecursivelyDisplayDirectoryNode(childNode, level + 1);
            ImGui::TreePop();
        }
    } else
    {
        // render file node

        bool show_file = true;
        // check if parentNode.FileName matches filter
        if (!m_Filter.empty()) show_file = (parentNode.FileName.find(m_Filter) != std::string::npos);
        ImGuiTreeNodeFlags node_flags = file_flags;
        if (std::find(m_Selection.begin(), m_Selection.end(), &parentNode) != m_Selection.end())
            node_flags |= ImGuiTreeNodeFlags_Selected;

        if (show_file)
        {
            ImGui::TreeNodeEx(parentNode.FileName.c_str(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) clickedNode = &parentNode;
        }
    }
    ImGui::PopStyleVar();
    ImGui::PopID();

    if (clickedNode)
    {
        // if control was clicked, add node to selection
        if (ImGui::GetIO().KeyCtrl) m_Selection.push_back(clickedNode);
        // otherwise, clear selection and add node to selection
        else
        {
            m_Selection.clear();
            m_Selection.push_back(clickedNode);
        }
        LOG_TRACE("Added node to selection: {}", clickedNode->FileName);
        node_clicked = true;
    }
    return node_clicked;
}

std::vector<std::string> FileManager::RecursivelyGetFilesStripRoot()
{
    std::vector<DirectoryNode> files_nodes;
    RecursivelyGetFiles(m_rootNode, files_nodes);

    std::vector<std::string> files_strings;
    for (auto &file : files_nodes)
    {
        std::string filepath = file.FullPath;
        filepath.erase(0, m_rootNode.FullPath.size() + 1);
        files_strings.push_back(filepath);
    }
    return files_strings;
}

void FileManager::RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                               std::filesystem::directory_iterator directoryIterator)
{
    for (const std::filesystem::directory_entry &entry : directoryIterator)
    {
        DirectoryNode &childNode = parentNode.Children.emplace_back();
        childNode.FullPath = entry.path().u8string();
        childNode.FileName = entry.path().filename().u8string();
        if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
            RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
    }

    auto moveDirectoriesToFront = [](const DirectoryNode &a, const DirectoryNode &b) {
        return (a.IsDirectory > b.IsDirectory);
    };
    std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode FileManager::CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath)
{
    DirectoryNode rootNode;
    rootNode.FullPath = rootPath.u8string();
    rootNode.FileName = rootPath.filename().u8string();
    if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
        RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

    return rootNode;
}

void FileManager::RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files)
{
    if (!node.IsDirectory)
    {
        files.push_back(node);
        return;
    }
    for (const DirectoryNode &child : node.Children)
        RecursivelyGetFiles(child, files);
}
