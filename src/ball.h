#pragma once
#include <raylib.h>
#include <iostream>

class X{
    public:

    X();
    //~X();
    void XDraw();

    private:

    Texture2D XSprite;
    Vector2 Xposition = {800, 0};

};

class O{
    public:
    O();
    //~O();
    void ODraw();

    private:
    Texture2D OSprite;
    Vector2 Oposition = {800, 500};
};

class Grid
{
    public:

    Grid();
    //~Grid();
    void GridDraw();

    private:

    Texture2D GridSprite;
    Vector2 GridPosition = {30, 50};

};