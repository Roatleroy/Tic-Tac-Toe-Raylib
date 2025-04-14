#include <iostream>
#include <string.h>
#include <vector>
#include "raylib.h"


// FOR Draggable Sprite -----------------------------------
enum SpriteID {
    SPRITE_X,
    SPRITE_O
};

SpriteID user;

struct Sprite {
    Texture2D texture;
    Vector2 position;
    Rectangle collider;
    Vector2 initialPosition;
    SpriteID id;
    bool ButtonDown;
    int Wins;
    bool win;
};
//----------------------------------------------------------

// GRID BOXES______________________________________________
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

// SYSTEM PROTOTYPES_____________________________________________
void DrawMultiplayer();
void DrawSinglePlayer();

void OnUpdate();

void CenterSprite(Texture2D Text, Vector2 Vec);

void OnStart();

bool HasGridCollision(Sprite& Piece, int& index);

Vector2 GetMousePositionScreenSpace();

void CheckWin(GridOwner ID);

bool CheckDraw();

void drawWin();

void resetBoard();

void Choose();

void menu();
void home();

//________________________________________________________________
Vector2 ScreenParams {1280, 720};

// System Variables----------------------------------------------------
Vector2 StaticX = {float(ScreenParams.x * 0.25), -1 * float(ScreenParams.y * 0.16)};
Vector2 StaticO = {float(ScreenParams.x * 0.25), float(ScreenParams.y * 0.16)};

Vector2 Static = {float(ScreenParams.x * 0.25), 0};

Sprite SpriteO;
Sprite SpriteX;

Texture2D House;

Grid GameGrid;

enum pick
{
    Menu,
    SinglePlayer,
    MultiPlayer,
    Choice
};

pick Picker = Menu;

bool XorO = true;

//_________________________________________________________________
int main() 
{

    //Set Color Based on RGB Values
    const Color darkGreen = {20, 160, 133, 255};

    //This sets the windows height and width
    InitWindow(ScreenParams.x, ScreenParams.y, "TIC-TAC-TOE");

    House = LoadTexture("resources/Home.png");

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f};
    camera.offset = (Vector2){ScreenParams.x/2.0f, ScreenParams.y/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    
    OnStart();

    while (!WindowShouldClose())
    {
        if (Picker != Menu) 
        {
            OnUpdate();
        }
           
        BeginDrawing();
            BeginMode2D(camera);

            home();

            switch (Picker)
            {
                case Menu:
                    menu();
                    break;
                case MultiPlayer:
                    ClearBackground(darkGreen);
                    DrawMultiplayer();
                    break;
                case SinglePlayer:
                    ClearBackground(darkGreen);
                    DrawSinglePlayer();
                    break;
                case Choice:
                    Choose();
                    break;
                default:
                    ClearBackground(darkGreen);
                    const char* error = "Single Player"; 
                    DrawText(error, 0, 0, 20, WHITE);
                    break;
            }
                    
        EndDrawing();
    }

    UnloadTexture(SpriteX.texture);
    UnloadTexture(SpriteO.texture);
    
    CloseWindow();

}

// Might want to change Check win to check both O and X at the same time so that
// The AI algorithm can be checked for wins as well. 
void CheckWin(GridOwner ID)
{

    for (int row = 0; row < 3; row++)
    {
        if (GameGrid.GridSquares[row * 3].Owner == ID && GameGrid.GridSquares[(row * 3) + 1].Owner == ID && GameGrid.GridSquares[(row * 3) + 2].Owner == ID)
        {
            if (ID == X)
            {
                SpriteX.Wins +=1;
                SpriteX.win = true;
            }
            else{
                SpriteO.Wins +=1;
                SpriteO.win = true;
            }
        }
    }
    for (int col = 0; col < 3; col++)
    {
        if (GameGrid.GridSquares[col].Owner == ID && GameGrid.GridSquares[col + 3].Owner == ID && GameGrid.GridSquares[col + 6].Owner == ID)
        {
            if (ID == X)
            {
                SpriteX.Wins +=1;
                SpriteX.win = true;
            }
            else{
                SpriteO.Wins +=1;
                SpriteO.win = true;
            }
        }
    }
    if ((GameGrid.GridSquares[0].Owner == ID && GameGrid.GridSquares[4].Owner == ID && GameGrid.GridSquares[8].Owner == ID) ||
        (GameGrid.GridSquares[2].Owner == ID && GameGrid.GridSquares[4].Owner == ID && GameGrid.GridSquares[6].Owner == ID))
    {
        if (ID == X)
            {
                SpriteX.Wins +=1;
                SpriteX.win = true;
            }
            else{
                SpriteO.Wins +=1;
                SpriteO.win = true;
            }
    }

    if (CheckDraw() && (SpriteX.win != true && SpriteO.win != true))
    {
        SpriteO.win = true;
        SpriteX.win = true;
    }

}

bool CheckDraw()
{
    for (int i = 0; i < 9; i++)
    {
        if (GameGrid.GridSquares[i].Owner == EMPTY)
        {
            return false;
        }
    }
    return true;
}

void menu()
{
    Vector2 Mouse = GetMousePositionScreenSpace();
   
    Vector2 buttonSize = {float(ScreenParams.x * 0.2), float(ScreenParams.y * 0.09)};
    Vector2 buttonPosition = {0, 0};
    Rectangle Single = {buttonPosition.x - (buttonSize.x/2) - float(ScreenParams.x * 0.14), buttonPosition.y - (buttonSize.y/2), buttonSize.x, buttonSize.y};
    Rectangle Multi = {buttonPosition.x - (buttonSize.x/2) + float(ScreenParams.x * 0.14), buttonPosition.y - (buttonSize.y/2), buttonSize.x, buttonSize.y};
   
    ClearBackground(DARKGRAY);

    const char* text1 = "SinglePlayer"; 
    DrawRectangle(Single.x, Single.y, buttonSize.x, buttonSize.y, BLACK);
    DrawRectangleLinesEx(Single, 4, RED);
    DrawText(text1, (Single.x + float(ScreenParams.x * 0.023)), (Single.y + float(ScreenParams.x * 0.018)), int((ScreenParams.x + ScreenParams.y)/2) * 0.03, WHITE);

    
    const char* text2 = "Multiplayer";
    DrawRectangle(Multi.x, Multi.y, buttonSize.x, buttonSize.y, BLACK);
    DrawRectangleLinesEx(Multi, 4, RED);
    DrawText(text2, (Multi.x + float(ScreenParams.x * 0.032)), (Multi.y + float(ScreenParams.x * 0.018)), int((ScreenParams.x + ScreenParams.y)/2) * 0.03, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, Single))
    {
        Picker = Choice;
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, Multi))
    {
        Picker = MultiPlayer;
    }
}

void Choose()
{
    Vector2 Mouse = GetMousePositionScreenSpace();
   
    Vector2 buttonSize = {float(ScreenParams.x * 0.125), float(ScreenParams.y * 0.075)};
    Vector2 buttonPosition = {0, 0};
    Rectangle Single = {buttonPosition.x - (buttonSize.x/2) - float(ScreenParams.x * 0.125), buttonPosition.y - (buttonSize.y/2), buttonSize.x, buttonSize.y};
    Rectangle Multi = {buttonPosition.x - (buttonSize.x/2) + float(ScreenParams.x * 0.125), buttonPosition.y - (buttonSize.y/2), buttonSize.x, buttonSize.y};

    ClearBackground(DARKGRAY);

    const char* Chooses = "What character do you want?";
    DrawText(Chooses, (-1 * (ScreenParams.x * 0.18)), (-1 * (ScreenParams.y * 0.25)), int((ScreenParams.x + ScreenParams.y)/2) * 0.03, WHITE);

    const char* text1 = "X"; 
    DrawRectangle(Single.x, Single.y, buttonSize.x, buttonSize.y, BLACK);
    DrawRectangleLinesEx(Single, 4, RED);
    DrawText(text1, (Single.x + (ScreenParams.x * 0.056)), (Single.y + (ScreenParams.y * 0.02125)), int((ScreenParams.x + ScreenParams.y)/2) * 0.03, WHITE);

    const char* text2 = "O";
    DrawRectangle(Multi.x, Multi.y, buttonSize.x, buttonSize.y, BLACK);
    DrawRectangleLinesEx(Multi, 4, RED);
    DrawText(text2, (Multi.x + (ScreenParams.x * 0.056)), (Multi.y + (ScreenParams.y * 0.02125)), int((ScreenParams.x + ScreenParams.y)/2) * 0.03, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, Single))
    {
        Picker = SinglePlayer;
        SpriteX.position = Static;
        SpriteX.initialPosition = Static;
        user = SPRITE_X;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, Multi))
    {
        Picker = SinglePlayer;
        SpriteO.position = Static;
        SpriteO.initialPosition = Static;
        user = SPRITE_O;
    }
    
}

void home()
{
    Vector2 Mouse = GetMousePositionScreenSpace();
    Vector2 buttonPosition = {float(ScreenParams.x * 0.46), float((ScreenParams.y * -0.437))};

    Rectangle Home = {(float)(buttonPosition.x - ((House.width * (ScreenParams.x + ScreenParams.y)/2) * 0.001)/2), (float)(buttonPosition.y - ((House.height * (ScreenParams.x + ScreenParams.y)/2)
    * 0.001)/2), (float)(House.width * ((ScreenParams.x + ScreenParams.y)/2) * 0.00117), (float)(House.height * ((ScreenParams.x + ScreenParams.y)/2) * 0.00116)};

    Vector2 Adjust = {float (buttonPosition.x - (House.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2) + float(ScreenParams.x * 0.004)), 
    float(buttonPosition.y - (House.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2) + float(ScreenParams.y * 0.00625))};

    DrawRectangleLinesEx(Home, 4, BLACK);
    DrawTextureEx(House, Adjust, 0.0f, float((ScreenParams.x + ScreenParams.y)/2) * 0.001, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Mouse, Home))
    {
        Picker = Menu;
        resetBoard();
    }
}

void resetBoard()
{
    for (int i = 0; i < 9; i ++)
    {
        GameGrid.GridSquares[i].Owner = EMPTY;
    }
    SpriteO.win = false;
    SpriteX.win = false;
    SpriteO.Wins = 0;
    SpriteX.Wins = 0;
    SpriteX.initialPosition = StaticX;
    SpriteO.initialPosition = StaticO;
    XorO = true;
}

void DrawMultiplayer()
{
    for (int i = 0; i < 9; i++)
    {
        DrawRectangleLinesEx(GameGrid.GridSquares[i].Collider, 4, BLACK);

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

    //DrawRectangleLinesEx(SpriteX.collider, 4, RED);
    //DrawRectangleLinesEx(SpriteO.collider, 4, RED);

    CenterSprite(SpriteX.texture, SpriteX.position);
    CenterSprite(SpriteO.texture, SpriteO.position);

    CenterSprite(SpriteX.texture, StaticX);
    CenterSprite(SpriteO.texture, StaticO);

   if (SpriteO.win || SpriteX.win )
   {
        drawWin();
   }
    
}

void drawWin()
{
    if (SpriteO.win && SpriteX.win)
    {
        const char* Draw_Text = "You've Drawn";
        DrawText(Draw_Text, (ScreenParams.x * -0.0833), (ScreenParams.y * -0.375), float((ScreenParams.x + ScreenParams.y)/2) * 0.04, WHITE);
    }
    else if (SpriteX.win)
    {
        const char* WinText = "YOU WIN X";
        DrawText(WinText, (ScreenParams.x * -0.0833), (ScreenParams.y * -0.375), float((ScreenParams.x + ScreenParams.y)/2) * 0.04, WHITE);
    }
    else if (SpriteO.win)
    {
        const char* WinText = "YOU WIN O";
        DrawText(WinText, (ScreenParams.x * -0.0833) , (ScreenParams.y * -0.375), float((ScreenParams.x + ScreenParams.y)/2) * 0.04, WHITE);
    }
}

void DrawSinglePlayer()
{
    for (int i = 0; i < 9; i++)
    {
        DrawRectangleLinesEx(GameGrid.GridSquares[i].Collider, 4, BLACK);

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

    if (user == SPRITE_X)
    {
        CenterSprite(SpriteX.texture, SpriteX.position);
        CenterSprite(SpriteX.texture, Static);
        //DrawRectangleLinesEx(SpriteX.collider, 4, RED);
    }
    else
    {
        CenterSprite(SpriteO.texture, SpriteO.position);
        CenterSprite(SpriteO.texture, Static);
        //DrawRectangleLinesEx(SpriteO.collider, 4, RED);
    }

    if (SpriteO.win || SpriteX.win )
    {
        drawWin();
    }

}

void OnStart()
{
    SpriteX.texture = LoadTexture("resources/X.png");
    SpriteX.position = StaticX;
    SpriteX.initialPosition = StaticX;
    SpriteX.id = SPRITE_X;
    SpriteX.ButtonDown = false;
    SpriteX.Wins = 0;
    SpriteX.win = false;

    SpriteO.texture = LoadTexture("resources/O.png");
    SpriteO.position = StaticO;
    SpriteO.initialPosition = StaticO;
    SpriteO.id = SPRITE_O;
    SpriteO.ButtonDown = false;
    SpriteO.Wins = 0;
    SpriteO.win = false;

    for (int i = 0; i < 9; i++)
    {
        float tilesize = float((ScreenParams.x + ScreenParams.y)/2) * 0.128;

        GameGrid.GridSquares[i].Owner = EMPTY;

        int Row = i / 3;
        int Col = i % 3;

        float x = ((float)((Row * tilesize) - tilesize)) - (ScreenParams.x * 0.166);
        float y = ((float)((Col * tilesize) - tilesize));

        GameGrid.GridSquares[i].Position = {x, y};
        
        GameGrid.GridSquares[i].Collider.x = x - (float)(tilesize / 2);
        GameGrid.GridSquares[i].Collider.y = y - (float)(tilesize / 2);

        GameGrid.GridSquares[i].Collider.height = tilesize;
        GameGrid.GridSquares[i].Collider.width = tilesize;
    }

    // Creates Barrier around X Sprite
    SpriteX.collider.x = (float)(SpriteX.position.x - (SpriteX.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
    SpriteX.collider.y = (float)(SpriteX.position.y - (SpriteX.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
    SpriteX.collider.width = SpriteX.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001);
    SpriteX.collider.height = SpriteX.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001);

    // Creates Barrier around Y Sprite
    SpriteO.collider.x = (float)(SpriteO.position.x - (SpriteO.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
    SpriteO.collider.y = (float)(SpriteO.position.y - (SpriteO.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
    SpriteO.collider.width = SpriteO.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001);
    SpriteO.collider.height = SpriteO.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001);
}

//Centers Drawn Textures
void CenterSprite(Texture2D Texture, Vector2 Position)
{
    //Vector2 origin = {(float)(Texture.width/2), (float)(Texture.height/2)};
    Vector2 centeredPosition = {(float)(Position.x - (Texture.width * (((ScreenParams.x + ScreenParams.y) / 2) * 0.001)/2)), 
        (float)(Position.y - (Texture.width * (((ScreenParams.x + ScreenParams.y) / 2) * 0.001)/2))};

    DrawTextureEx(Texture, centeredPosition, 0.0f, float(((ScreenParams.x + ScreenParams.y)/2) * 0.001), WHITE);

}

void OnUpdate()
{  

   Vector2 Mouse = GetMousePositionScreenSpace();

   if (Picker == SinglePlayer)
   {
        if(user == SPRITE_X)
        {
            SpriteX.collider.x = (float)(SpriteX.position.x - (SpriteX.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
            SpriteX.collider.y = (float)(SpriteX.position.y - (SpriteX.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
        }
        else
        {
            SpriteO.collider.x = (float)(SpriteO.position.x - (SpriteO.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
            SpriteO.collider.y = (float)(SpriteO.position.y - (SpriteO.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
        }
   }
   else 
   {
        SpriteX.collider.x = (float)(SpriteX.position.x - (SpriteX.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
        SpriteX.collider.y = (float)(SpriteX.position.y - (SpriteX.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
        SpriteO.collider.x = (float)(SpriteO.position.x - (SpriteO.texture.height * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
        SpriteO.collider.y = (float)(SpriteO.position.y - (SpriteO.texture.width * (((ScreenParams.x + ScreenParams.y)/2) * 0.001)/2));
   }
  
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
        if (HasGridCollision(SpriteX, index) && GameGrid.GridSquares[index].Owner == EMPTY && XorO == true)
        {
            GameGrid.GridSquares[index].Owner = X;
            XorO = false;
            CheckWin(X);
        }
        if (HasGridCollision(SpriteO, index) && GameGrid.GridSquares[index].Owner == EMPTY && XorO == false)
        {
            GameGrid.GridSquares[index].Owner = O;
            XorO = true;
            CheckWin(O);
        }

        SpriteX.ButtonDown = false;
        SpriteO.ButtonDown = false;
    }

    SpriteX.position = SpriteX.ButtonDown ? Mouse : SpriteX.initialPosition;
    SpriteO.position = SpriteO.ButtonDown ? Mouse : SpriteO.initialPosition;

}

bool HasGridCollision(Sprite& Piece, int& index)
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