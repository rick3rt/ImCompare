#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include "DirectoryNode.h"

typedef std::pair<int, const DirectoryNode *> indexed_selection_t;
typedef std::array<indexed_selection_t, 20> selection_t;

struct SelectedFileRef
{
    int level;
    int index;
    const DirectoryNode *node;
    SelectedFileRef() : level(0), index(0), node(nullptr) {}
    SelectedFileRef(int level_, int index_, const DirectoryNode *node_)
        : level(level_), index(index_), node(node_)
    {
    }
};

class FolderManager
{
  private:
    char m_CurrentPath[256] = {0};

    std::filesystem::path m_rootPath; // root path, e.g. C:\Users\<user>\Documents\*
    std::string m_Filter;             // filter, extracted from root path, e.g. *.png
    DirectoryNode m_rootNode;
    selection_t m_Selection;
    std::vector<SelectedFileRef> m_SelectedFiles;

  public:
    FolderManager()
    {
        ClearSelection();
        SetRoot("resource");
    }
    ~FolderManager() {}

    void Start() {}
    bool Update();

    void SetRoot(const std::filesystem::path &rootPath);

    const std::filesystem::path &GetRoot() const { return m_rootPath; }
    const DirectoryNode &GetRootNode() const { return m_rootNode; }

    void ClearSelection();
    void ClearFileSelection() { m_SelectedFiles.clear(); }
    const selection_t &GetSelection() const { return m_Selection; }

    // returns true if an item is clicked
    bool RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level = 0);

    void PropagateSelection(int level);

    // static functions
    static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath);
    static void RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files);
    static void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                             std::filesystem::directory_iterator directoryIterator);

    static void RecursivelyPrintDirectoryNode(const DirectoryNode &parentNode, int level = 0);
};
