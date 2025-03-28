#include <raylib.h>
#include <iostream>
#include "rlgl.h"
#include "raymath.h"
// ------------------------------------

Vector2 StaticX = {300, -128};
Vector2 StaticO = {300, 128};

enum SpriteID {
    SPRITE_X,
    SPRITE_O,
};

// Example Usage
struct Sprite {
    Texture2D texture;
    Vector2 position;
    Rectangle collider;
    Vector2 initialPosition;
    SpriteID id;
    bool ButtonDown;
};

Sprite SpriteO;
Sprite SpriteX;
// -----------------------------------

Vector2 ScreenParams;
//-----------------------------------

void Draw();
void OnUpdate();
void CenterSprite(Texture2D Text, Vector2 Vec);
void OnStart();
bool UpdateLastCollision(Sprite& Piece, int& index);
Vector2 GetMousePositionScreenSpace();

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

//------------------------------------
Grid GameGrid;

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
    camera.target = (Vector2){ 0.0f, 0.0f};
    camera.offset = (Vector2){ScreenParams.x/2.0f, ScreenParams.y/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    
    OnStart();

    while (!WindowShouldClose())
    {
        OnUpdate();
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(darkGreen);
            Draw();
        EndDrawing();
    }


    UnloadTexture(SpriteX.texture);
    UnloadTexture(SpriteO.texture);
    
    CloseWindow();

}

void Draw()
{
    for (int i = 0; i < 9; i++)
    {
        DrawRectangleLinesEx(GameGrid.GridSquares[i].Collider, 4, BLACK);
        DrawRectangleLinesEx(SpriteX.collider, 4, RED);
        DrawRectangleLinesEx(SpriteO.collider, 4, RED);

        Vector2 Position1 = GameGrid.GridSquares[i].Position;

        switch (GameGrid.GridSquares[i].Owner)
        {
            case X:
                CenterSprite(SpriteX.texture, Position1);
                break;
            case O:
                CenterSprite(SpriteO.texture, Position1);
                break;
            case EMPTY:
                break;
        }
    }

    CenterSprite(SpriteX.texture, SpriteX.position);
    CenterSprite(SpriteO.texture, SpriteO.position);

    CenterSprite(SpriteX.texture, StaticX);
    CenterSprite(SpriteO.texture, StaticO);

}

void OnStart()
{
    SpriteX.texture = LoadTexture("resources/X.png");
    SpriteO.texture = LoadTexture("resources/O.png");
    SpriteX.position = StaticX;
    SpriteO.position = StaticO;
    SpriteX.initialPosition = StaticX;
    SpriteO.initialPosition = StaticO;
    SpriteX.id = SPRITE_X;
    SpriteO.id = SPRITE_O;

    for (int i = 0; i < 9; i++)
    {
        float tilesize = 128.0f;

        GameGrid.GridSquares[i].Owner = EMPTY;

        int Row = i / 3;
        int Col = i % 3;

        float x = ((float)((Row * tilesize) - tilesize)) - 200;
        float y = ((float)((Col * tilesize) - tilesize));

        GameGrid.GridSquares[i].Position = {x, y};
        
        GameGrid.GridSquares[i].Collider.x = x - (float)(tilesize / 2);
        GameGrid.GridSquares[i].Collider.y = y - (float)(tilesize / 2);

        GameGrid.GridSquares[i].Collider.height = tilesize;
        GameGrid.GridSquares[i].Collider.width = tilesize;
    }

    // Creates Barrier around X Sprite
    SpriteX.collider.x = (float)(SpriteX.position.x - (SpriteX.texture.height/2));
    SpriteX.collider.y = (float)(SpriteX.position.y - (SpriteX.texture.width/2));
    SpriteX.collider.width = SpriteX.texture.width;
    SpriteX.collider.height = SpriteX.texture.height;

    // Creates Barrier around Y Sprite
    SpriteO.collider.x = (float)(SpriteO.position.x - (SpriteO.texture.height/2));
    SpriteO.collider.y = (float)(SpriteO.position.y - (SpriteO.texture.width/2));
    SpriteO.collider.width = SpriteO.texture.width;
    SpriteO.collider.height = SpriteO.texture.height;
}

//Centers Drawn Textures
void CenterSprite(Texture2D Texture, Vector2 Position)
{
    Vector2 origin = {(float)(Texture.width/2), (float)(Texture.height/2)};
    Vector2 centeredPosition = {Position.x - origin.x, Position.y - origin.y};
    DrawTextureEx(Texture, centeredPosition, 0.0f, 1.0f, WHITE);

}

void OnUpdate()
{  

   Vector2 Mouse = GetMousePositionScreenSpace();

   SpriteX.collider.x = (float)(SpriteX.position.x - (SpriteX.texture.height/2));
   SpriteX.collider.y = (float)(SpriteX.position.y - (SpriteX.texture.width/2));
   SpriteO.collider.x = (float)(SpriteO.position.x - (SpriteO.texture.height/2));
   SpriteO.collider.y = (float)(SpriteO.position.y - (SpriteO.texture.width/2));

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    { 
        if(CheckCollisionPointRec(Mouse, SpriteX.collider))
            SpriteX.ButtonDown = true;
        else if (CheckCollisionPointRec(Mouse, SpriteO.collider))
            SpriteO.ButtonDown = true;
    }
   
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        int index = 0;
        if (UpdateLastCollision(SpriteX, index) && GameGrid.GridSquares[index].Owner == EMPTY)
        {
            GameGrid.GridSquares[index].Owner = X;
        }
        if (UpdateLastCollision(SpriteO, index) && GameGrid.GridSquares[index].Owner == EMPTY)
        {
            GameGrid.GridSquares[index].Owner = O;
        }

        SpriteX.ButtonDown = false;
        SpriteO.ButtonDown = false;
    }

    SpriteX.position = SpriteX.ButtonDown ? Mouse : SpriteX.initialPosition;
    SpriteO.position = SpriteO.ButtonDown ? Mouse : SpriteO.initialPosition;

}

bool UpdateLastCollision(Sprite& Piece, int& index)
{
    for(int i = 0; i < 9; i++)
        {
            if (CheckCollisionPointRec(Piece.position, GameGrid.GridSquares[i].Collider))
            {
                index = i;
                return true;
            }
        }
        return false;
}


Vector2 GetMousePositionScreenSpace()
{
    Vector2 MousePosition = GetMousePosition();
    MousePosition.x -= (ScreenParams.x / 2);
    MousePosition.y -= (ScreenParams.y / 2);
    return MousePosition;
}