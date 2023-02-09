#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <SDL2/SDL.h>
#include <vector>
#include <array>

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

        Vector2D(int = 0, int = 0);
    };

    struct Vertex2D
    {
        Vector2D Position;

        RGBA Color;

        SDL_Vertex GetSDLVertex();

        Vertex2D(Vector2D = Vector2D(), RGBA = RGBA());
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

    class Object
    {
    public:
            int ID;

            std::vector<Vertex2D> Vertices;

            std::vector<SDL_Vertex> GetBuffer();

            void AddVertex(Vertex2D);

            Object();
            ~Object();
    };

    class Triangle : public Object
    {
    public:
            Triangle(Vertex2D, Vertex2D, Vertex2D, RGBA);
            ~Triangle();
    };
}

#endif // OBJECTS_H_
