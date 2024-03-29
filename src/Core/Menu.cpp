#pragma once

#include <imgui.h>

#include "Explorer.h"
#include "Core/Application.h"

void Menu::KeyboardShortcuts()
{
    // check if control Q is pressed and exit
    if (ImGui::IsKeyPressed(ImGuiKey_Q) && ImGui::GetIO().KeyCtrl) { Application::Get().Stop(); }

    if (ImGui::IsKeyPressed(ImGuiKey_O) && ImGui::GetIO().KeyCtrl) { Explorer::Get().OpenFolder(); }
    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::GetIO().KeyCtrl) { Explorer::Get().SaveSession(); }
    if (ImGui::IsKeyPressed(ImGuiKey_Q)) { Explorer::Get().GetViewer().ShowNextTab(); }
    // if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift)
    //     Explorer::Get().SaveAs();
}

void Menu::DrawMenu()
{
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

        ImGui::EndMainMenuBar();
    }
}

void Menu::Update()
{
    DrawMenu();
    KeyboardShortcuts();
}