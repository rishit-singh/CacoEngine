#ifndef VERTEX_H_
#define VERTEX_H_

#include <SDL2/SDL.h>

namespace CacoEngine
{
    struct RGBA
    {
        int R;
        int G;
        int B;
        int A;

        RGBA(int = 0, int = 0, int = 0, int = 255);
    };

    struct Vector2D
    {
        int X;
        int Y;

        Vector2D& operator +=(Vector2D);
        Vector2D& operator -=(Vector2D);

        Vector2D(int = 0, int = 0);
    };

    struct Vertex2D
    {
        Vector2D Position;

        Vector2D TextureCoordinates;

        RGBA Color;

        SDL_Vertex GetSDLVertex();

        Vertex2D(Vector2D = Vector2D(), RGBA = RGBA(), Vector2D = Vector2D());
    };

    enum class Color
    {
        Red,
        Blue,
        Green,
        White,
        Black
    };

    extern RGBA Colors[5];
}

#endif // VERTEX_H_
