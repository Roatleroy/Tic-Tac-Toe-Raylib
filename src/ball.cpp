#include "ball.h"
#include <raylib.h>

X::X()
{
    XSprite = LoadTexture("resources/Sprite.png");
}

O::O()
{
    OSprite = LoadTexture("resources/Circle-icon-22/png");
}

Grid::Grid()
{
    GridSprite = LoadTexture("resources/Tic-tac-toe.png");
}

void X::XDraw()
{
    DrawTextureEx(XSprite, Xposition, 0.0f, 1.0f, WHITE);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        
        Xposition = GetMousePosition();

        Xposition.x = Xposition.x - 100;
        Xposition.y = Xposition.y - 100;

    }
    else{
        Xposition.x = 800;
        Xposition.y = 500;
    }
}


void O::ODraw()
{
    DrawTextureEx(OSprite, Oposition, 0.0f, 1.0f, WHITE);
}

void Grid::GridDraw()
{
    DrawTextureEx(GridSprite, GridPosition, 0.0f, 1.0f, WHITE);
}

//DrawTextureEx(OSprite, position2, 0.0f, 0.8f, WHITE);
//DrawTextureEx(GRIDSprite, position, 0.0f, 1.2f, WHITE);