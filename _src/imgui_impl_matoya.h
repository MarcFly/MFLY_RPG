#ifndef IMGUI_IMPL_MATOYA
#define IMGUI_IMPL_MATOYA
#include "imgui.h"
#include "matoya.h"

IMGUI_IMPL_API void     ImGui_ImplMatoya_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplMatoya_NewFrame(MTY_App* mty_app);
IMGUI_IMPL_API void     ImGui_ImplMatoya_Init(MTY_App* mty_app, MTY_Renderer* mty_renderer);
IMGUI_IMPL_API void     ImGui_ImplMatoya_EventFunc(const MTY_Event* evt, MTY_App* opaque);
#endif