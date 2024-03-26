#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "DirectoryNode.h"

class FileManager
{
  public:
    FileManager() {}
    ~FileManager() {}

    void SetRoot(const std::filesystem::path &rootPath);

    const std::filesystem::path &GetRoot() const { return m_rootPath; }
    const DirectoryNode &GetRootNode() const { return m_rootNode; }

    void ClearSelection() { m_Selection.clear(); }
    const std::vector<const DirectoryNode *> &GetSelection() const { return m_Selection; }

    // returns true if an item is clicked
    bool RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level = 0);

    std::vector<std::string> RecursivelyGetFilesStripRoot();

    // static functions
    static void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                             std::filesystem::directory_iterator directoryIterator);

    static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath);

    static void RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files);

  private:
    std::filesystem::path m_rootPath; // root path, e.g. C:\Users\<user>\Documents\*
    std::string m_Filter;             // filter, extracted from root path, e.g. *.png
    DirectoryNode m_rootNode;
    std::vector<const DirectoryNode *> m_Selection;
};