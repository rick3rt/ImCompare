#pragma once

#include <imgui.h>

#include "Explorer.h"
#include "Application.h"

void Menu::KeyboardShortcuts()
{
    // check if control Q is pressed and exit
    if (ImGui::IsKeyPressed(ImGuiKey_Q) && ImGui::GetIO().KeyCtrl) { Application::Get().Stop(); }

    if (ImGui::IsKeyPressed(ImGuiKey_O) && ImGui::GetIO().KeyCtrl) { Explorer::Get().Open(); }
    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::GetIO().KeyCtrl) { Explorer::Get().Save(); }
    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift)
    {
        Explorer::Get().SaveAs();
    }
}

void Menu::DrawMenu()
{
    // add imgui menu with file
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { Explorer::Get().Open(); }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { Explorer::Get().Save(); }
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) { Explorer::Get().SaveAs(); }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Get().Stop(); }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Test", "Ctrl+T")) { ; }
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