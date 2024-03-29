#include "Base.h"
#include "Filesystem/FolderManager.h"

void runApp()
{
    Explorer app("DataExplorer", 1280, 760, 0, nullptr);
    app.Run();
}

void test_operators();

int main()
{
    Log::Init(); // initialize logger

    runApp();

    // LOG_INFO("Testing FolderManager");
    // FolderManager folderManager;
    // folderManager.SetRoot("C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/resource/example/");
    // const DirectoryNode &rootNode = folderManager.GetRootNode();
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

void test_fun(int fn(bool a, bool b))
{
    LOG_INFO("0 0 = {} ", fn(false, false));
    LOG_INFO("1 0 = {} ", fn(true, false));
    LOG_INFO("0 1 = {} ", fn(false, true));
    LOG_INFO("1 1 = {} ", fn(true, true));
}

void test_operators()
{
    LOG_INFO("Testing operators");

    LOG_INFO("Testing AND operator");
    test_fun([](bool a, bool b) { return static_cast<int>(a && b); });

    LOG_INFO("Testing OR operator");
    test_fun([](bool a, bool b) { return static_cast<int>(a || b); });

    LOG_INFO("Testing XOR operator");
    test_fun([](bool a, bool b) { return static_cast<int>(a ^ b); });

    LOG_INFO("Testing XAND operator");
    test_fun([](bool a, bool b) { return static_cast<int>(!(a && b)); });

    LOG_INFO("Testing XNOR operator");
    test_fun([](bool a, bool b) { return static_cast<int>(!(a ^ b)); });

    LOG_INFO("Testing NAND operator");
    test_fun([](bool a, bool b) { return static_cast<int>(!(a || b)); });

    LOG_INFO("Testing ??? operator");
    test_fun([](bool a, bool b) { return static_cast<int>((a && b) || b); });

    LOG_INFO("Testing NOT operator");
}