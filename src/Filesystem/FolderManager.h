#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include "DirectoryNode.h"
#include "FolderHistory.h"

// indexed_selection_t constist of an index in the preceding folder and a pointer to the selected
// DirectoryNode
typedef std::pair<int, const DirectoryNode *> indexed_selection_t;
// selection_t is an array that contains the selected folder for all levels. Supports max 20 levels. That
// is way too much and doesnt fit the screen, but resistant up to a certain point to click spamming.
typedef std::array<indexed_selection_t, 20> selection_t;

// SelectedFileRef is a struct that contains the level, index and a pointer to the selected
// DirectoryNode. level and index keeping helps to update the selected files when the folder structure is
// changed. If the index in a higher folder is changed, the selected files are updated to files that
// match the old files index. If there is no file at that index in that new folder, then the entry is
// removed from the selected file list.
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
    int m_CurrentMaxLevel = 0;
    bool m_FlattenDirFiles = false;
    bool m_OnlyShowImageFiles = true;
    std::vector<SelectedFileRef> m_SelectedFiles;
    FolderHistory m_FolderHistory;

  public:
    FolderManager()
    {
        ClearSelection();
        // SetRoot("resource"); // set root to demo. history manager will set the last used folder
        // SetRoot("\\\\tudelft.net\\staff-umbrella\\tfUSBrain\\processed_data\\TC_batch");
        SetRoot(m_FolderHistory.getLatestFolder());
    }
    ~FolderManager() {}

    void Start() {}
    bool Update();

    void SetRoot(const std::filesystem::path &rootPath);
    void UpdateRoot();

    const std::filesystem::path &GetRoot() const { return m_rootPath; }
    const DirectoryNode &GetRootNode() const { return m_rootNode; }

    void ClearSelection();
    void ClearFileSelection() { m_SelectedFiles.clear(); }
    const selection_t &GetSelection() const { return m_Selection; }

    // returns true if an item is clicked
    bool RecursivelyDisplayDirectoryNode(const DirectoryNode &parentNode, int level = 0);

    void IncrementFolder(int level, int increment = 1);
    void PropagateSelection(int level);

    void RenderFileSelection();
    void PrintSelection();

    // getter for FolderHistory
    FolderHistory &GetFolderHistory() { return m_FolderHistory; }

    // static functions
    static DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath);
    static void RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files);
    static void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                             std::filesystem::directory_iterator directoryIterator);

    static void RecursivelyPrintDirectoryNode(const DirectoryNode &parentNode, int level = 0);
};
