#include <raylib.h>
#include <iostream>
#include "rlgl.h"
#include "raymath.h"

Texture2D XSprite;
Texture2D OSprite;
Texture2D GridSprite;
Vector2 Xposition = {800, 0};
Vector2 Oposition = {800, 500}; 
Vector2 GridPosition = {30, 50};

int main() 
{

    //Set Color Based on RGB Values
    const Color darkGreen = {20, 160, 133, 255};

    //this sets the variables for the windows height and width
    constexpr int screenWidth = 1200;
    constexpr int screenHeight = 800;

  
    //This sets the windows height and width
    InitWindow(screenWidth, screenHeight, "TIC-TAC-TOE");

    XSprite = LoadTexture("resources/SPRITE.png");
    OSprite = LoadTexture("resources/Circle-icon-22.png");
    GridSprite = LoadTexture("resources/Tic-tac-toe.png");

    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(darkGreen);

            DrawTextureEx(XSprite, Xposition, 0.0f, 1.0f, WHITE);

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                
                Xposition = GetMousePosition();
        
                Xposition.x = Xposition.x - 100;
                Xposition.y = Xposition.y - 100;
        
            }
            else{
                Xposition.x = 800;
                Xposition.y = 0;
            }
            
        
            DrawTextureEx(OSprite, Oposition, 0.0f, 1.0f, WHITE);
            DrawTextureEx(GridSprite, GridPosition, 0.0f, 1.0f, WHITE);

        EndDrawing();
    }

    UnloadTexture(XSprite);
    UnloadTexture(OSprite);
    UnloadTexture(GridSprite);
    
    CloseWindow();

}