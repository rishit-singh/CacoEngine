#ifndef OBJECTS_H_
#define OBJECTS_H_

namespace CacoEngine
{
    struct RGB
    {
        int R;
        int G;
        int B;

        RGB(int = 0, int = 0, int = 0);
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

        RGB Color;

        Vertex2D(Vector2D = Vector2D(), RGB = RGB());
    };

    enum class Color
    {
        Red,
        Blue,
        Green
    };

    extern RGB Colors[3];
}

#endif // OBJECTS_H_
