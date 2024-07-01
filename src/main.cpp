#include "Base.h"
#include "Filesystem/FolderManager.h"

void runApp()
{
    Explorer app("ImCompare", 1280, 760, 0, nullptr);
    app.Run();
}

void test_operators();

int main()
{
    Log::Init(); // initialize logger
    Log::SetLogLevel(spdlog::level::debug);

    runApp();
    return 0;

    // LOG_INFO("Testing FolderManager");
    // FolderManager folderManager;
    // folderManager.SetRoot("C:/Users/verasonics/Documents/Rick/REPOS/ImCompare/resource/example/folders");
    // const DirectoryNode &rootNode = folderManager.GetRootNode();
    // // // folderManager.RecursivelyPrintDirectoryNode(rootNode);

    // DirectoryNode node = rootNode; // copy
    // DirectoryNode nodeflat = flattenFiles(node);

    // LOG_INFO("== ORIGINAL: == ");
    // folderManager.RecursivelyPrintDirectoryNode(node);
    // LOG_INFO("== FLATTENED: == ");
    // folderManager.RecursivelyPrintDirectoryNode(nodeflat);

    // LOG_INFO("== ORIGINAL ROOT NODE: == ");
    // folderManager.RecursivelyPrintDirectoryNode(rootNode);

    // DirectoryNode node = FolderManager::CreateDirectryNodeTreeFromPath(
    //     "C:/Users/rwaasdorp1/Documents/REPOS/DataExplorer/resource");
    // FolderManager::RecursivelyPrintDirectoryNode(node);

    // std::vector<DirectoryNode> files;
    // folderManager.RecursivelyGetFiles(rootNode, files);
    // for (const auto &file : files)
    //     LOG_INFO("File: {0}", file.FullPath);

    // test_operators();

    return 0;
}
