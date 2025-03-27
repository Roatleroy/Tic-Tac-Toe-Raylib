#include <raylib.h>
#include <iostream>
#include "rlgl.h"
#include "raymath.h"

Texture2D XSprite;
Texture2D XSpriteS;

Texture2D OSprite;
Texture2D OSpriteS;

Texture2D GridSprit;

Vector2 Xint = {300, -128};
Vector2 XintS = {300, -128};

Vector2 Oint = {300, 128};
Vector2 OintS = {300, 128};

Vector2 ScreenParams;

Rectangle Rect[9];

Rectangle XRect;
Rectangle ORect;

bool ButtonDownX = false;
bool ButtonDownO = false;

void Draw();
void OnUpdate();
void CenterSprite(Texture2D Text, Vector2 Vec);
void OnStart();
Vector2 GetMousePositionScreenSpace();

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


    XSprite = LoadTexture("resources/X.png");
    OSprite = LoadTexture("resources/O.png");

    XSpriteS = LoadTexture("resources/X.png");
    OSpriteS = LoadTexture("resources/O.png");

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


    UnloadTexture(XSprite);
    UnloadTexture(OSprite);
    
    CloseWindow();

}

void Draw()
{
    for (int i = 0; i < 9; i++)
    {
        DrawRectangleLinesEx(Rect[i], 4, BLACK);
    }

    CenterSprite(XSprite, Xint);
    CenterSprite(OSprite, Oint);

    CenterSprite(XSpriteS, XintS);
    CenterSprite(OSpriteS, OintS);
    
}

void OnStart()
{
    for (int i = 0; i < 9; i++)
    {
        float tilesize = 128.0f;

        int Row = i / 3;
        int Col = i % 3;

        float x = ((float)((Row * tilesize) - tilesize)) - 200;
        float y = ((float)((Col * tilesize) - tilesize));

        
        Rect[i].x = x - (float)(tilesize / 2);
        Rect[i].y = y - (float)(tilesize / 2);

        Rect[i].height = tilesize;
        Rect[i].width = tilesize;
    }

    // Creates Barrier around X Sprite
    XRect.x = (float)(Xint.x - (XSprite.height/2));
    XRect.y = (float)(Xint.y - (XSprite.width/2));
    XRect.width = XSprite.width;
    XRect.height = XSprite.height;

    // Creates Barrier around Y Sprite
    ORect.x = (float)(Oint.x - (OSprite.height/2));
    ORect.y = (float)(Oint.y - (OSprite.width/2));
    ORect.width = OSprite.width;
    ORect.height = OSprite.height;
}

void CenterSprite(Texture2D Text, Vector2 Vec)
{
    Vector2 origin = {(float)(Text.width/2), (float)(Text.height/2)};
    Vector2 centeredPosition = {Vec.x - origin.x, Vec.y - origin.y};
    DrawTextureEx(Text, centeredPosition, 0.0f, 1.0f, WHITE);

}

void UpdatePosition(Vector2& INT, Vector2& Mouse, Rectangle& Rect)
{
    INT.x = Mouse.x;
    INT.y = Mouse.y;

    Rect.x = (float)(INT.x - (XSprite.height/2));
    Rect.y = (float)(INT.y - (XSprite.width/2));

}

void OnUpdate()
{ 
    Vector2 Mouse = GetMousePositionScreenSpace();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, XRect) && ButtonDownO != true)
    {
        ButtonDownX = true;
    }
        
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ButtonDownX)
    {
        UpdatePosition(Xint, Mouse, XRect);
    }
    else
    {
        Xint = {300, -128};
        XRect.x = (float)(Xint.x - (XSprite.height/2));
        XRect.y = (float)(Xint.y - (XSprite.width/2));
        ButtonDownX = false;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, ORect) && ButtonDownX != true)
    {
        ButtonDownO = true;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ButtonDownO)
    {
        UpdatePosition(Oint, Mouse, ORect);
    }
    else
    {
        Oint = {300, 128};
        ORect.x = (float)(Oint.x - (XSprite.height/2));
        ORect.y = (float)(Oint.y - (XSprite.width/2));
        ButtonDownO = false;
    }
    
}


Vector2 GetMousePositionScreenSpace()
{
    Vector2 MousePosition = GetMousePosition();
    MousePosition.x -= (ScreenParams.x / 2);
    MousePosition.y -= (ScreenParams.y / 2);
    return MousePosition;
}