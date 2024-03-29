#include "Base.h"

void runApp()
{
    Explorer app("DataExplorer", 1280, 760, 0, nullptr);
    app.Run();
}

int main()
{
    Log::Init(); // initialize logger

    runApp();

    // LOG_INFO("Testing FolderManager");
    // FolderManager folderManager;
    // folderManager.SetRoot("C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/resource/example/");
    // const DirectoryNode &rootNode = folderManager.GetRootNode();
    // folderManager.RecursivelyPrintDirectoryNode(rootNode);

    // std::vector<DirectoryNode> files;
    // folderManager.RecursivelyGetFiles(rootNode, files);
    // for (const auto &file : files)
    //     LOG_INFO("File: {0}", file.FullPath);

    return 0;
}