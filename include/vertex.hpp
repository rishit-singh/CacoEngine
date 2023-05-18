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

        Vector2D operator +(Vector2D);
        Vector2D operator -(Vector2D);
        Vector2D operator *(Vector2D);
        Vector2D operator /(Vector2D);
        Vector2D& operator +=(Vector2D);
        Vector2D& operator -=(Vector2D);
        Vector2D& operator *=(Vector2D);
        Vector2D& operator /=(Vector2D);

        Vector2D operator +(int);
        Vector2D operator -(int);
        Vector2D operator *(int);
        Vector2D operator /(int);
        Vector2D& operator +=(int);
        Vector2D& operator -=(int);
        Vector2D& operator *=(int);
        Vector2D& operator /=(int);

        bool operator ==(Vector2D&);

        Vector2D(int = 0, int = 0);
    };

    struct Vector2Df
    {
        double X;
        double Y;


        Vector2Df operator +(Vector2Df);
        Vector2Df operator -(Vector2Df);
        Vector2Df operator *(Vector2Df);
        Vector2Df operator /(Vector2Df);

        Vector2Df& operator +=(Vector2Df);
        Vector2Df& operator -=(Vector2Df);
        Vector2Df& operator *=(Vector2Df);
        Vector2Df& operator /=(Vector2Df);

        Vector2Df operator +(double);
        Vector2Df operator -(double);
        Vector2Df operator *(double);
        Vector2Df operator /(double);
        Vector2Df& operator +=(double);
        Vector2Df& operator -=(double);
        Vector2Df& operator *=(double);
        Vector2Df& operator /=(double);

        Vector2Df(double = 0, double = 0);
    };

    struct Vertex2D
    {
        Vector2D Position;

        Vector2D TextureCoordinates;

        RGBA Color;

        SDL_Vertex GetSDLVertex();
        
        SDL_FPoint GetSDLPointF();
        SDL_Point GetSDLPoint();

        Vertex2D(Vector2D = Vector2D(), RGBA = RGBA(), Vector2D = Vector2D());
    };

    struct Vertex2Df
    {
        Vector2Df Position;

        Vector2Df TextureCoordinates;

        RGBA Color;

        SDL_Vertex GetSDLVertex();

        SDL_FPoint GetSDLPoint();

        Vertex2Df(Vector2Df = Vector2Df(), RGBA = RGBA(), Vector2Df = Vector2Df());
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
