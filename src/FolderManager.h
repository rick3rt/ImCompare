#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include "DirectoryNode.h"

typedef std::pair<int, const DirectoryNode *> indexed_selection_t;
typedef std::array<indexed_selection_t, 20> selection_t;

class FolderManager
{
  private:
    char m_CurrentPath[256] = {0};

    std::filesystem::path m_rootPath; // root path, e.g. C:\Users\<user>\Documents\*
    std::string m_Filter;             // filter, extracted from root path, e.g. *.png
    DirectoryNode m_rootNode;
    selection_t m_Selection;

  public:
    FolderManager() { ClearSelection(); }
    ~FolderManager() {}

    void Start() {}
    bool Update();

    void SetRoot(const std::filesystem::path &rootPath);

    const std::filesystem::path &GetRoot() const { return m_rootPath; }
    const DirectoryNode &GetRootNode() const { return m_rootNode; }

    void ClearSelection()
    {
        for (int i = 0; i < m_Selection.size(); i++)
        {
            m_Selection[i].first = 0;
            m_Selection[i].second = nullptr;
        }
    }
    // const selection_t &GetSelection() const { return m_Selection; }

    // returns true if an item is clicked
    bool RecursivelyPrintDirectoryNode(const DirectoryNode &parentNode, int level = 0);
    bool RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level = 0);

    void PropagateSelection(int level);
    // std::vector<std::string> RecursivelyGetFilesStripRoot();

    // static functions
    static void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                             std::filesystem::directory_iterator directoryIterator);

    static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath);

    static void RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files);
};
