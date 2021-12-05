#include "imgui_impl_matoya.h"
#include <cstdio>
static void* imgui_pixels = NULL;
static uint32_t screen_w, screen_h;
static MTY_Time last_time;

static ImVec2 mouse_pos(0,0);
static bool mouse_buttons[5] = {false,false,false,false,false};
static float mouse_wheel = .0f;
static float mouse_wheelH = .0f;

void ImGui_ImplMatoya_EventFunc(const MTY_Event* evt, MTY_App* mty_app)
{
	ImGuiIO& io = ImGui::GetIO();
	int32_t x,y;

	MTY_PrintEvent(evt);
	switch(evt->type)
	{
		case MTY_EVENT_KEY:
			io.KeyMap[evt->key.key] = evt->key.pressed;
			break;
		case MTY_EVENT_HOTKEY:
			break;
		case MTY_EVENT_SCROLL:
			io.MouseWheel = evt->scroll.y;
			io.MouseWheelH = evt->scroll.x;
			break;
		case MTY_EVENT_BUTTON:
			MTY_WindowGetPosition(mty_app, 0, &x, &y);
			io.MouseDown[evt->button.button-1] = evt->button.pressed;
			io.MousePos = ImVec2(evt->button.x, evt->button.y);
			break;
		case MTY_EVENT_MOTION:
			MTY_WindowGetPosition(mty_app, 0, &x, &y);
			if(!evt->motion.relative)
				io.MousePos = ImVec2(evt->motion.x, evt->motion.y);
			else
			{
				io.MousePos.x += evt->motion.x;
				io.MousePos.y += evt->motion.y;
			}

			
			break;
	}
}

void ImGui_ImplMatoya_NewFrame(MTY_App* mty_app)
{
    ImGuiIO& io = ImGui::GetIO();

    IM_ASSERT(io.Fonts->IsBuilt());

    MTY_WindowGetScreenSize(mty_app, 0, &screen_w, &screen_h);
    screen_w = (screen_w <= 0) ? 1 : screen_w;
	screen_h = (screen_h <= 0) ? 1 : screen_h;
    //io.DisplaySize = ImVec2(screen_w, screen_h);
    
    uint32_t win_w, win_h;
	//MTY_Window
    MTY_WindowGetSize(mty_app, 0, &win_w, &win_h);
	io.DisplaySize = ImVec2(win_w, win_h);

    MTY_Time curr_time = MTY_GetTime();
    io.DeltaTime = ((double)curr_time - (double)last_time);
    last_time = curr_time;

    // Update Buttons?
	

}

//============================================================================================

void ImGui_ImplMatoya_Init(MTY_App* mty_app, MTY_Renderer* mty_renderer)
{
    last_time = MTY_GetTime();
    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab] = MTY_KEY_TAB;                     
	io.KeyMap[ImGuiKey_LeftArrow] = MTY_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = MTY_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = MTY_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = MTY_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = MTY_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = MTY_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = MTY_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = MTY_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = MTY_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = MTY_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = MTY_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = MTY_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = MTY_KEY_A;
	io.KeyMap[ImGuiKey_C] = MTY_KEY_C;
	io.KeyMap[ImGuiKey_V] = MTY_KEY_V;
	io.KeyMap[ImGuiKey_X] = MTY_KEY_X;
	io.KeyMap[ImGuiKey_Y] = MTY_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = MTY_KEY_Z;

    io.BackendPlatformName = "imgui_impl_libmatoya";

    int w,h;
    int32_t id = 0;

    io.Fonts->GetTexDataAsRGBA32((unsigned char**)&imgui_pixels, &w, &h);
    //MTY_RendererSetUITexture(mty_renderer, MTY_WindowGetGFX(mty_app, 0), MTY_WindowGetDevice(mty_app, 0), MTY_WindowGetContext(mty_app, 0), id, imgui_pixels, w, h);
	MTY_WindowSetUITexture(mty_app, 0, 0, imgui_pixels, w,h);
	io.Fonts->TexID = (ImTextureID)(intptr_t)id;

}

void ImGui_ImplMatoya_Shutdown()
{
    ImGui::GetIO().Fonts->TexID = 0;
    delete imgui_pixels;

    //?
}

