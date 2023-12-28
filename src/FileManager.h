#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <imgui.h>

struct DirectoryNode
{
    std::string FullPath;
    std::string FileName;
    std::vector<DirectoryNode> Children;
    bool IsDirectory;
};

typedef void (*DisplayDirectoryNodeCallback)(DirectoryNode &outputNode);

class FileManager
{
  public:
    FileManager() {}
    ~FileManager() {}

    void SetRoot(const std::filesystem::path &rootPath)
    {
        m_rootPath = rootPath;
        Update();
    }
    const std::filesystem::path &GetRoot() const { return m_rootPath; }
    const DirectoryNode &GetRootNode() const { return m_rootNode; }
    void Update() { m_rootNode = CreateDirectryNodeTreeFromPath(m_rootPath); }

    static bool RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode,
                                                DisplayDirectoryNodeCallback callback,
                                                DirectoryNode &selectedNode)
    {
        bool clicked = false;
        ImGui::PushID(&parentNode);
        if (parentNode.IsDirectory)
        {
            if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
            {
                for (const DirectoryNode &childNode : parentNode.Children)
                    RecursivelyDisplayDirectoryNode(childNode, callback, selectedNode);
                ImGui::TreePop();
            }
        } else
        {
            if (ImGui::TreeNodeEx(parentNode.FileName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                                                   ImGuiTreeNodeFlags_Leaf |
                                                                   ImGuiTreeNodeFlags_SpanFullWidth))
            {
                // ImGuiTreeNodeFlags_Selected
                // ... handle file click interaction
                // selectedNode = parentNode;
                // callback(selectedNode);
                // clicked = true;
            }
        }
        ImGui::PopID();
        return clicked;
    }

    static void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
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

    static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath)
    {
        DirectoryNode rootNode;
        rootNode.FullPath = rootPath.u8string();
        rootNode.FileName = rootPath.filename().u8string();
        if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
            RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

        return rootNode;
    }

  private:
    std::filesystem::path m_rootPath;
    DirectoryNode m_rootNode;
};