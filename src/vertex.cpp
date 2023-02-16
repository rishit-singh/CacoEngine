#include "vertex.hpp"

CacoEngine::Vector2D::Vector2D(int x, int y) :  X(x), Y(y)
{
}

CacoEngine::Vertex2D::Vertex2D(Vector2D position, RGBA color, Vector2D textureCoordinates) : Position(position), Color(color), TextureCoordinates(textureCoordinates)
{
}

CacoEngine::RGBA::RGBA(int r, int g, int b, int a) : R(r), G(g), B(b), A(a)
{
}

CacoEngine::RGBA CacoEngine::Colors[5] = {RGBA(255, 0, 0), RGBA(0, 255, 0),
                                         RGBA(0, 0, 255), RGBA(255, 255, 255),
                                         RGBA(0, 0, 0)};


CacoEngine::Vector2D &CacoEngine::Vector2D::operator+=(Vector2D difference)
{
    this->X += difference.X;
    this->Y += difference.Y;

    return *this;
}
