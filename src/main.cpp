#include <raylib.h>
#include <iostream>
#include "rlgl.h"
#include "raymath.h"

Vector2 ScreenParams;

Texture2D XSprite;
Texture2D OSprite;

enum GridOwner
{
    X, O, EMPTY
};

struct GridSquare
{
    GridOwner Owner;
    Vector2 Position;
    Rectangle Collider;
};

struct Grid
{
    GridSquare GridSquares[9];
};

Grid GameGrid;

void DrawSpriteCentered(Texture2D sprite, Vector2 position)
{
    Vector2 spriteCenter = {(float)(sprite.width / 2), (float)(sprite.height / 2)};
    Vector2 centeredPosition = {position.x - spriteCenter.x, position.y - spriteCenter.y};
    DrawTextureEx(sprite, centeredPosition, 0.0f, 1.0f, WHITE);
}

Vector2 GetMousePositionScreenSpace()
{
    Vector2 MousePosition = GetMousePosition();
    MousePosition.x -= (ScreenParams.x / 2);
    MousePosition.y -= (ScreenParams.y / 2);
    MousePosition.x /= 2;
    MousePosition.y /= 2;
    return MousePosition;
}

void OnStart()
{
    // Initialize all grid squares as empty
    for (int i = 0; i < 9; i++)
    {
        GameGrid.GridSquares[i].Owner = EMPTY; // Initialize all grid squares as empty

        float tileSize = 64.0f; // Set the size of the grid square
        
        // Calculate the row and column for positioning
        int row = i / 3; // if divisible by 3, then it is the last row
        int col = i % 3; // if modulo 3 is 0, then it is the first column (modulo is the remainder of the division)
        
        // Set up the collider rectangle for each grid square
        float x = (float)(col * tileSize - tileSize); // subtract to center
        float y = (float)(row * tileSize - tileSize); // subtract to center

        GameGrid.GridSquares[i].Position = Vector2{x,y}; // Set the position of the grid square
        
        // Create a rectangle collider centered on the grid position
        // Size matches the EmptyTileSprite dimensions
        GameGrid.GridSquares[i].Collider = Rectangle{
            x - tileSize/2.0f, // Moves it from the center, to the top left corner
            y - tileSize/2.0f,
            tileSize, //provide width and height
            tileSize
        };
    }
}

void OnUpdate()
{
    Vector2 MousePosition = GetMousePositionScreenSpace();

    for (int i = 0; i < 9; i++)
    {
        if (CheckCollisionPointRec(MousePosition, GameGrid.GridSquares[i].Collider))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                GameGrid.GridSquares[i].Owner = X;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                GameGrid.GridSquares[i].Owner = O;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
            {
                GameGrid.GridSquares[i].Owner = EMPTY;
            }
        }
    }
}

void OnDraw()
{
    for (int i = 0; i < 9; i++)
    {
        // Draw Grid
        DrawRectangleLinesEx(GameGrid.GridSquares[i].Collider, 2, RED);

        // Draw Owners
        Vector2 tilePosition = GameGrid.GridSquares[i].Position;
        switch (GameGrid.GridSquares[i].Owner)
        {
            case X:
                DrawSpriteCentered(XSprite, tilePosition);
            break;

            case O:
                DrawSpriteCentered(OSprite, tilePosition);
            break;

            case EMPTY:
            break;
        }
    }
}

int main() 
{
    //Set Color Based on RGB Values
    const Color darkGreen = {20, 160, 133, 255};

    //this sets the variables for the windows height and width
    ScreenParams.x = 1200;
    ScreenParams.y = 800;

    //This sets the windows height and width
    InitWindow(ScreenParams.x, ScreenParams.y, "TIC-TAC-TOE");

    Camera2D camera = { 0 };
    camera.target = Vector2{ 0.0f, 0.0f };                          // World position to follow
    camera.offset = Vector2{ ScreenParams.x/2.0f, ScreenParams.y/2.0f }; // Center of screen
    camera.rotation = 0.0f;
    camera.zoom = 2.0f; // <== This acts like orthographic size inverse (zoom in/out)

    XSprite = LoadTexture("resources/X.png");
    OSprite = LoadTexture("resources/O.png");

    SetTargetFPS(60);

    OnStart();
    
    while (!WindowShouldClose())
    {
        OnUpdate();
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(darkGreen);
                OnDraw();
            EndMode2D();
        EndDrawing();
    }

    UnloadTexture(XSprite);
    UnloadTexture(OSprite);
    
    CloseWindow();
}