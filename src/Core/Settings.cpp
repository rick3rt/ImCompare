#include "Settings.h"
#include "Explorer.h"

void Settings::Start()
{

    static auto fonts = Application::Get().GetFonts();
    // get the names from the fonts
    for (auto &font : fonts)
        m_fontNames.push_back(font.first);
}

void Settings::Update()
{

    if (ImGui::Begin("Settings"))
    {

        if (ImGui::Button("bla")) { LOG_INFO("bla"); }

        // add a listbox to select one of the fonts in m_fontNames
        static int item_current_idx = 0;
        const char *combo_preview_value = m_fontNames[item_current_idx].c_str();
        if (ImGui::BeginCombo("Fonts", combo_preview_value))
        {
            for (int n = 0; n < m_fontNames.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(m_fontNames[n].c_str(), is_selected))
                {
                    item_current_idx = n;
                    Application::Get().SetFont(m_fontNames[n]);
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
}