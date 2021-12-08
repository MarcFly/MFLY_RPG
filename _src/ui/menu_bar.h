#ifndef _MENU_BAR_
#define _MENU_BAR_

#include "ui.h"
#include <vector>

struct SubMenu
{
    const char* name;
    std::vector<uint32_t> items;
    std::vector<uint32_t> sub_menus;
};

struct MenuItem
{
    bool* active_state;
    const char* name;
    std::vector<uint32_t> sub_items;
};

class MenuBar : public UI_Item
{
public:
    std::vector<uint32_t> menus;
    std::vector<MenuItem> items;

    void Init() final{}

    void UpdateMenuItem(MenuItem& item)
    {
        if (item.sub_items.size() == 0)
        {
            bool prev = *item.active_state;
            ImGui::MenuItem(item.name, "", item.active_state);
            UI_RequireUpdate(!(*item.active_state != prev));
        }
        else if (ImGui::BeginMenu(item.name))
        {
            for (uint16_t i = 0; i < item.sub_items.size(); ++i)
                UpdateMenuItem(items[item.sub_items[i]]);

            ImGui::EndMenu();
        }
    }

    void Update(float dt) final
    {
        ImGui::BeginMainMenuBar();

        for(uint16_t i = 0; i < menus.size(); ++i)
        {
            MenuItem& curr = items[menus[i]];
            if(ImGui::BeginMenu(curr.name))
            {
                for(uint16_t j = 0; j < curr.sub_items.size(); ++j)
                    UpdateMenuItem(items[curr.sub_items[j]]);

                ImGui::EndMenu();
            }
        }

        //// Should a main menu register and be a class/struct by itself or should it be standalone
        //if (ImGui::BeginMenu("Menu1"))
        //{
        //    ImGui::EndMenu();
        //}
//
        //if (ImGui::BeginMenu("Menu2"))
        //{
        //    ImGui::EndMenu();
        //}
//
        //if (ImGui::BeginMenu("UI WIndows"))
        //{
        //    for (uint16_t i = 0; i < base_items.size(); ++i)
        //    {
        //        UpdateMenuItem(items[base_items[i]]);
        //    }
//
        //    ImGui::EndMenu();
        //}

        ImGui::EndMainMenuBar();
    }

    void CleanUp() final {}

    uint32_t RegisterMenu(bool* item_active, const char* name)
    {
        MenuItem mi;
        mi.active_state = item_active;
        mi.name = name;
        items.push_back(mi);
        uint32_t ret = items.size() - 1;
        menus.push_back(ret);
        return ret;
    }
    uint32_t RegisterMenuItem(bool* item_active, const char* name, uint32_t sub_item = -1)
    {
        MenuItem mi;
        mi.active_state = item_active;
        mi.name = name;
        items.push_back(mi);
        uint32_t ret = items.size() - 1;
        if(sub_item != -1) items[sub_item].sub_items.push_back(ret);
        return ret;
    }
};

#endif