#pragma once

#include <imgui.h>

#include "Explorer.h"
#include "Core/Application.h"

#define checkKey(x) ImGui::IsKeyPressed(x)

void Menu::KeyboardShortcuts()
{
    bool keyCtrl = ImGui::GetIO().KeyCtrl;
    // check if control Q is pressed and exit
    // if (checkKey(ImGuiKey_Q) && keyCtrl) { Application::Get().Stop(); }
    if (checkKey(ImGuiKey_O) && keyCtrl) { Explorer::Get().OpenFolder(); }
    if (checkKey(ImGuiKey_S) && keyCtrl) { Explorer::Get().SaveSession(); }

    if (checkKey(ImGuiKey_F1)) { Explorer::Get().GetViewer().ShowNextTab(); }
    if (checkKey(ImGuiKey_F2)) { Explorer::Get().GetViewer().ToggleAutoFit(); }
    if (checkKey(ImGuiKey_F3)) { Explorer::Get().GetViewer().ToggleSideBySide(); }
    if (checkKey(ImGuiKey_A)) { Explorer::Get().GetViewer().ToggleAutoFit(); }
    if (checkKey(ImGuiKey_S)) { Explorer::Get().GetViewer().ToggleSideBySide(); }
    if (checkKey(ImGuiKey_Z)) { Explorer::Get().GetViewer().ClearItems(); }

    if (checkKey(ImGuiKey_1)) { Explorer::Get().GetViewer().ShowTab(0); }
    if (checkKey(ImGuiKey_2)) { Explorer::Get().GetViewer().ShowTab(1); }
    if (checkKey(ImGuiKey_3)) { Explorer::Get().GetViewer().ShowTab(2); }
    if (checkKey(ImGuiKey_4)) { Explorer::Get().GetViewer().ShowTab(3); }
    if (checkKey(ImGuiKey_5)) { Explorer::Get().GetViewer().ShowTab(4); }

    int increment = keyCtrl ? -1 : 1;
    if (checkKey(ImGuiKey_Q)) { Explorer::Get().GetFolderManager().IncrementFolder(0, increment); }
    if (checkKey(ImGuiKey_W)) { Explorer::Get().GetFolderManager().IncrementFolder(1, increment); }
    if (checkKey(ImGuiKey_E)) { Explorer::Get().GetFolderManager().IncrementFolder(2, increment); }
    if (checkKey(ImGuiKey_R)) { Explorer::Get().GetFolderManager().IncrementFolder(3, increment); }
    if (checkKey(ImGuiKey_T)) { Explorer::Get().GetFolderManager().IncrementFolder(4, increment); }
    if (checkKey(ImGuiKey_Y)) { Explorer::Get().GetFolderManager().IncrementFolder(5, increment); }

    // if (checkKey(ImGuiKey_S) && keyCtrl && ImGui::GetIO().KeyShift)
    //     Explorer::Get().SaveAs();
}

void Menu::DrawMenu()
{
    bool keyCtrl = ImGui::GetIO().KeyCtrl;

    // add imgui menu with file
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Folder", "Ctrl+O")) { Explorer::Get().OpenFolder(); }
            if (ImGui::MenuItem("Save Session", "Ctrl+S")) { Explorer::Get().SaveSession(); }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Get().Stop(); }
            ImGui::EndMenu();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Clear Folder Selection", "Ctrl+C"))
            Explorer::Get().GetFolderManager().ClearSelection();
        if (ImGui::MenuItem("Clear Image Selection", "Ctrl+V"))
        {
            Explorer::Get().GetFolderManager().ClearFileSelection();
            Explorer::Get().GetViewer().ClearItems();
        }

        if (ImGui::BeginMenu("History"))
        {
            if (ImGui::MenuItem("Clear"))
            {
                Explorer::Get().GetFolderManager().GetFolderHistory().clearHistory();
            }
            ImGui::Separator();
            auto history = Explorer::Get().GetFolderManager().GetFolderHistory().getHistory();
            for (auto &folder : history)
            {
                if (ImGui::MenuItem(folder.c_str()))
                {
                    if (!keyCtrl)
                        Explorer::Get().GetFolderManager().SetRoot(folder);
                    else
                        Explorer::Get().GetFolderManager().GetFolderHistory().removeFolder(folder);
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Menu::Update()
{
    DrawMenu();
    KeyboardShortcuts();
}