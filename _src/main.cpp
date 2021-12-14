
#include "raylib.h"
#include "ui/ui.h"



#include "asset_management/asset_manager.h"

#define MAX_TOUCH_POINTS 10
#define _GLFW_HANDLE
int main(int argc, char **argv)
{
	
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - input multitouch");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
    Vector2 touchPositions[MAX_TOUCH_POINTS] = { 0 };

    SetTargetFPS(144);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

	UI_Init();
    
    float dt = 16.6;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		
		
        // Update
        //----------------------------------------------------------------------------------
        

        UI_Update(dt);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
            

        
        
        UI_Render();
        EndDrawing();
        //----------------------------------------------------------------------------------
		
	}

    UI_CleanUp();
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}