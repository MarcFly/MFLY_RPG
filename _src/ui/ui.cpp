#include "ui.h"

#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ITEMS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UI_Item::Init()
{
    UI_RegisterItem(this);
}

void RM_Popup::CheckToOpen()
{
    rm_was_clicked = (ImGui::IsWindowHovered(flags) && ImGui::IsMouseClicked(ImGuiMouseButton_Right) || rm_was_clicked);

    if (rm_was_clicked && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::OpenPopup(container_name);
        rm_was_clicked = false;
    }    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI DEFAULTS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UI_DefaultTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Text Color
    style.Colors[0] = ImVec4(.9f, .9f, .9f, 1); // Default
    style.Colors[1] = ImVec4(1, 0, 186 / 255.f, .4f); // Disabled
    // Popup Background
    style.Colors[4] = ImVec4(30 / 255.f, 40 / 255.f, 60 / 255.f, 1);
    // Border Colors
    style.Colors[5] = ImVec4(1, 1, 1, .5f); // Default
    style.Colors[6] = ImVec4(1, .6f, .9f, .5f); // Shadow
    // Frame Background
    style.Colors[7] = ImVec4(.2f, .4f, .9f, .2f); // Default
    style.Colors[8] = ImVec4(.2f, .6f, 1, .6f); // Hovered
    style.Colors[9] = ImVec4(.2f, .6f, 1, .9f); // Click
    // Title Colors
    style.Colors[10] = ImVec4(0, .4f, 1.f, .1f); // Inactive
    style.Colors[11] = ImVec4(.2f, .3f, .5f, .7f); // Active
    style.Colors[12] = style.Colors[10]; // Collapsed
    // MenuBar Background
    style.Colors[13] = ImVec4(.1f, .1f, .1f, 1);
    // ScrollBar Colors
    style.Colors[14] = ImVec4(1, 1, 1, .1f); // Background
    style.Colors[15] = ImVec4(1, 1, 1, .4f); // Default
    style.Colors[16] = ImVec4(1, 1, 1, .6f); // Hover
    style.Colors[17] = ImVec4(1, 1, 1, .9f); // Active
    // Checkbox Color
    style.Colors[18] = ImVec4(0, 1, 1, 1);
    // Slider
    style.Colors[19] = ImVec4(.8f, .6f, .9f, .8f); // Default
    style.Colors[20] = ImVec4(.8f, .6f, .9f, 1); // Grabbed
    // Button
    style.Colors[21] = style.Colors[7];
    style.Colors[22] = style.Colors[8];
    style.Colors[23] = style.Colors[9];
    // Headers
    style.Colors[24] = ImVec4(.2f, .4f, .9f, .5f); // Default
    style.Colors[25] = ImVec4(.2f, .6f, 1, .8f); // Hovered
    style.Colors[26] = ImVec4(.2f, .6f, 1, .9f); // Click
    // Separator
    style.Colors[27] = ImVec4(.55f, .55f, .75f, .7f); // Default
    style.Colors[28] = ImVec4(.55f, .55f, .75f, .9f); // Hovered
    style.Colors[29] = ImVec4(.55f, .55f, .75f, 1); // Active
    // Resize Mark
    style.Colors[30] = style.Colors[5]; // Default
    style.Colors[31] = ImVec4(1, 1, 1, .8f); // Hovered
    style.Colors[32] = ImVec4(1, 1, 1, 1); // Active
    // Tabs
    style.Colors[33] = style.Colors[7]; // Default
    style.Colors[34] = style.Colors[8]; // Hovered
    style.Colors[35] = style.Colors[9]; // Active
    style.Colors[36] = style.Colors[7];
    style.Colors[37] = ImVec4(0, 0, 0, 0);
    // Window Colors
    style.Colors[ImGuiCol_WindowBg].w = 1.f;
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0, 52/255.f, 102/255.f, 1);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(96 / 255.f, 46 / 255.f, 1, 1);
    // TODO: Docking... 
    // Plots will go with ImPlot when implemented
    // continue...
}
void UI_DefaultStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Main
    
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(10, 5);
    style.ItemSpacing = ImVec2(10, 5);
    style.ItemInnerSpacing = ImVec2(5, 5);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 20;
    style.ScrollbarSize = 20;
    style.GrabMinSize = 20;

    // Border
    style.FrameBorderSize = 1;
    style.WindowBorderSize = 2;
    style.PopupBorderSize = 1.5f;
    style.TabBorderSize = 1;
    style.ChildBorderSize = 1;

    // Rounding
    style.GrabRounding = (style.FrameRounding = 6) - 2;
    style.WindowRounding = style.FrameRounding * 2;
    style.ChildRounding = 0;
    style.PopupRounding = style.FrameRounding;
    style.ScrollbarRounding = 4;
    style.TabRounding = style.FrameRounding;

    // Alignment
    style.WindowTitleAlign = ImVec2(0.02f, 0.5f);
    style.WindowMenuButtonPosition = 0;
    style.ColorButtonPosition = 0;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0, 0.5f);
    style.DisplaySafeAreaPadding = ImVec2(0, 0);
}

void UI_SetFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = false;
    icons_config.PixelSnapH = true;

    io.FontDefault = io.Fonts->AddFontFromFileTTF((std::string(GetWorkingDirectory()) + "/Cousine-Regular.ttf").c_str(), 20);
    io.Fonts->AddFontFromFileTTF((std::string(GetWorkingDirectory()) + "/MaterialIcons-Regular.ttf").c_str(), 40, &icons_config, icons_ranges);
    //io.Fonts->Fonts[io.Fonts->Fonts.size()-1]->Scale /= 3.f;
    io.Fonts->Build();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI INIT ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "menu_bar.h"

std::vector<UI_Item*> items;
std::vector<UI_ID> active_items;
static MenuBar menu_bar;

static ImGuiDockNodeFlags base_dockflags = ImGuiDockNodeFlags_NoWindowMenuButton;

void UI_Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    UI_DefaultStyle();
    UI_DefaultTheme();
    UI_SetFonts();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigInputTextCursorBlink = false;
    io.ConfigDragClickToInputText = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // MenuBar Default starts
    menu_bar.Init();
}

void UI_RegisterItem(UI_Item* item)
{
    items.push_back(item);
    item->id.pos = items.size() - 1;
    if (item->active) active_items.push_back(item->id);
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI UPDATE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool require_update = false;

void UI_UpdateActives()
{
    active_items.clear();
    uint32_t s = items.size();
    for (uint32_t i = 0; i < s; ++i)
        if (items[i]->active) active_items.push_back(items[i]->id);

    require_update = false;
}

void UI_RequireUpdate(bool window_state)
{
    require_update = require_update || !window_state;
}

static bool dragging = false;
static ItemDraggable* draggable = NULL;

bool ItemDraggable::CheckStartDrag()
{
    return ImGui::IsItemHovered() && !dragging && ImGui::IsMouseDown(0);
}

void UI_StartDraggable(ItemDraggable* item)
{
    draggable = item;
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

ItemDraggable* UI_CheckDroppedDraggable(const ImVec2 min, const ImVec2 max)
{
    if (!ImGui::IsMouseDown(0) && draggable != NULL 
        && ImGui::IsMouseHoveringRect(min, max, false))
        return draggable;
    return NULL;
}

void UI_Update(float dt)
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    
    ImGui::NewFrame();
    
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), NULL);

    ImGui::ShowDemoWindow();

    menu_bar.Update(dt);

    for(uint16_t i = 0; i < active_items.size(); ++i)
        items[active_items[i].pos]->Update(dt);

    if(require_update) UI_UpdateActives();

    if (!ImGui::IsMouseDown(0) && draggable != NULL)
    {
        draggable->CleanUp();
        delete draggable;
        draggable = NULL;
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
    }

    ImGui::ShowDemoWindow();
}

void UI_Render()
{
    // ImGui Draw    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI_CleanUp()
{
    ImGui::DestroyContext();
    UI_CleanActions();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENGINE_UI UNDO/REDO ACTIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Obviously none of this is thread safe...
std::vector<Executer*> redo_list;
std::vector<Executer*> undo_list;

void UI_ExecuteAction(Executer* action)
{
    action->Execute();
    undo_list.push_back(action);
}

void UI_RedoLastAction()
{
    if (redo_list.size() == 0) return;
    Executer* a = redo_list[redo_list.size() - 1];
    a->Redo();
    undo_list.push_back(a);
    redo_list.pop_back();
}
void UI_UndoLastAction()
{
    if (undo_list.size() == 0) return;
    Executer* a = undo_list[undo_list.size() - 1];
    a->Undo();
    redo_list.push_back(a);
    undo_list.pop_back();
}

void UI_CleanActions()
{
    for (uint32_t i = 0; i < redo_list.size(); ++i)
        delete redo_list[i];
    for (uint32_t i = 0; i < undo_list.size(); ++i)
        delete undo_list[i];

    redo_list.clear();
    undo_list.clear();
}