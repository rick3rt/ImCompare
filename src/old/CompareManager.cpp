#include "CompareManager.h"
#include "Core.h"
#include "FileManager.h"
#include "misc/cpp/imgui_stdlib.h"
#include "GuiHelpers.h"

// ==================================================================================

CompareManager::CompareManager()
{ // SetBasePath(std::filesystem::current_path());
    SetBasePath(
        "C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/build/Debug/resource/example/folders");
    // SetBasePath("C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/build/Debug/resource/example/files");

    m_RegexComparer.SetRegexString(R"((Medium\d)\\(Apod\d))");
}

CompareManager::~CompareManager() {}

void CompareManager::SetBasePath(std::filesystem::path path)
{
    m_BasePath = path.string();
    m_FileManager.SetRoot(path);
    m_RegexComparer.SetFiles(m_FileManager.RecursivelyGetFilesStripRoot());
}

void CompareManager::Update()
{
    if (ImGui::Begin("Compare Manager"))
    {
        if (ImGui::InputText("Base Path", &m_BasePath)) { m_FileManager.SetRoot(m_BasePath); }
        if (ImGui::Button("Clear"))
        {
            Explorer::Get().GetViewer().ClearItems();
            m_FileManager.ClearSelection();
        }
        // ImGui::SameLine();
    }
    ImGui::End();

    if (ImGui::Begin("Compare Manager (Advanced)"))
    {
        if (ImGui::InputText("Base Path", &m_BasePath))
        {
            m_FileManager.SetRoot(m_BasePath);
            m_RegexComparer.SetFiles(m_FileManager.RecursivelyGetFilesStripRoot());
        }
        if (ImGui::Button("Clear"))
        {
            Explorer::Get().GetViewer().ClearItems();
            m_FileManager.ClearSelection();
        }

        if (ImGui::InputText("Regex", m_RegexComparer.GetRegexString())) { m_RegexComparer.Compile(); }
        ImGui::SameLine();
        bool regex_valid = m_RegexComparer.GetRegexValid();
        ImGui::Checkbox("Regex Valid", &regex_valid);

        ImGui::BeginDisabled(!regex_valid);

        if (regex_valid)
        {
            m_RegexComparer.Compute();
            std::vector<std::set<std::string>> options = m_RegexComparer.GetOptions();

            int max_rows = 0;
            int num_groups = options.size();
            for (auto &option : options)
                if (option.size() > max_rows) { max_rows = option.size(); }

            // render buttons in a table
            if (num_groups > 0 &&
                ImGui::BeginTable("optionTable", num_groups, ImGuiTableFlags_SizingFixedFit))
            {
                for (int row = 0; row < max_rows; row++)
                {
                    ImGui::TableNextRow();
                    for (int col = 0; col < num_groups; col++)
                    {
                        ImGui::TableSetColumnIndex(col);
                        // ImGui::Text("test r%i - c%i", row, col);
                        if (options[col].size() > row)
                        {
                            std::string lbl = *std::next(options[col].begin(), row);
                            ImGui::Button(lbl.c_str());
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndDisabled();
        ImGui::End();

        if (ImGui::Begin("File Manager"))
        {
            bool selection_changed =
                m_FileManager.RecursivelyDisplayDirectoryNode(m_FileManager.GetRootNode());
            if (selection_changed)
            {
                const std::vector<const DirectoryNode *> &m_Selection = m_FileManager.GetSelection();
                auto &v = Explorer::Get().GetViewer();
                v.ClearItems(); // clear last items, and add new
                for (auto &node : m_Selection)
                {
                    if (node->IsDirectory) { continue; } // skip directories
                    std::string filename = node->FileName;
                    std::filesystem::path path = node->FullPath;
                    std::string info = node->FileName;
                    v.AddItem(filename, info, path);
                }
            }
        }
        ImGui::End();
    }
}
