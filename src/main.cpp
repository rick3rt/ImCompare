#include "Log.h"
#include "Explorer.h"

void runApp()
{
    Explorer app("DataExplorer", 1280, 760, 0, nullptr);
    app.Run();
}

// #include "FileDialog.h"
// #include "FileSystem.h"

// void test()
// {

//     std::filesystem::path open = FileDialog::Open();
//     std::filesystem::path save = FileDialog::SaveAs(open);
//     LOG_INFO("File Open: {}", open);
//     LOG_INFO("File Save: {}", save);

//     std::string demo = "hahahahahhahahahahahaha";
//     FileSystem::Save(save, demo);
// }

int main()
{
    Log::Init(); // initialize logger

    runApp();
    // test();

    return 0;
}