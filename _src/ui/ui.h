#ifndef _UI_H_
#define _UI_H_

#include "raylib.h"

#include "imgui.h"
#include "IconsMaterialDesign.h"
#include <cstdint>

class Executer
{
public:
    const char* name = "Default Action Name";
    virtual void Execute() {};
    virtual void Redo() {};
    virtual void Undo() {};
};

struct UI_ID
{
    uint32_t id;
    uint16_t pos;
};

class UI_Item
{
public:
    UI_ID id;
    const char* name;
    uint16_t style_ind;
    bool active;

    virtual void Init();
    virtual void Update(float dt) {};
    virtual void CleanUp() {};
};

class ItemDraggable
{
    uint32_t id;
    uint32_t type;
public:
    static bool CheckStartDrag();
    virtual void SetUp() {};
    virtual void Receive(ItemDraggable* item) {};
    virtual void CleanUp() {};
};

class RM_Popup
{
    bool rm_was_clicked = false;
public:
    ImGuiHoveredFlags flags;
    const char* container_name;

    void CheckToOpen();
};

void UI_Init();
void UI_DefaultTheme();
void UI_RegisterItem(UI_Item* item);

void UI_RequireUpdate(bool window_state);
void UI_StartDraggable(ItemDraggable* item);
ItemDraggable* UI_CheckDroppedDraggable(const ImVec2 min, const ImVec2 max);
void UI_Update(float dt);
void UI_Render();

void UI_ExecuteAction(Executer* action);
void UI_RedoLastAction();
void UI_UndoLastAction();

void UI_CleanActions();

void UI_CleanUp();

#endif