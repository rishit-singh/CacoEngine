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


CacoEngine::Vector2D &CacoEngine::Vector2D::operator +=(Vector2D difference)
{
    this->X += difference.X;
    this->Y += difference.Y;

    return *this;
}

CacoEngine::Vector2D &CacoEngine::Vector2D::operator -=(Vector2D difference)
{
    this->X -= difference.X;
    this->Y -= difference.Y;

    return *this;
}

SDL_Vertex CacoEngine::Vertex2D::GetSDLVertex()
{
    return {
        SDL_FPoint { (float)this->Position.X, (float)this->Position.Y },
        SDL_Color { (uint8_t)this->Color.R, (uint8_t)this->Color.B, (uint8_t)this->Color.G, (uint8_t)this->Color.A },
        SDL_FPoint { (float)this->TextureCoordinates.X, (float)this->TextureCoordinates.Y }
    };
}

SDL_Point CacoEngine::Vertex2D::GetSDLPoint()
{
    return SDL_Point { this->Position.X, this->Position.Y };
}

SDL_FPoint CacoEngine::Vertex2D::GetSDLPointF()
{
    return SDL_FPoint { (float)this->Position.X, (float)this->Position.Y };
}
