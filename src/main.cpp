#include <raylib.h>
#include "ball.h"
#include <iostream>
#include "rlgl.h"
#include "raymath.h"


int main() 
{

    //Set Color Based on RGB Values
    const Color darkGreen = {20, 160, 133, 255};

    //this sets the variables for the windows height and width
    constexpr int screenWidth = 1200;
    constexpr int screenHeight = 800;

  
    //This sets the windows height and width
    InitWindow(screenWidth, screenHeight, "TIC-TAC-TOE");
    
  X Xs;
    O Os;
    Grid Grids;

    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(darkGreen);

            Xs.XDraw();
            Os.ODraw();
            Grids.GridDraw();

        EndDrawing();
    }
    
    CloseWindow();

}