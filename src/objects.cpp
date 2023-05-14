#include "objects.hpp"
#include "vertex.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <random>

CacoEngine::Object::Object() : ID(0), Position(Vector2D())
{
}

CacoEngine::Object::Object(const Object &object)
{
    *this = object;
}

CacoEngine::Object& CacoEngine::Object::operator =(const Object& object)
{
    this->ID = object.ID;
    this->Vertices = object.Vertices;
    this->mTexture = object.mTexture;
    this->Position = this->Vertices[0].Position;

    return *this;
}

CacoEngine::Object::~Object() {}

CacoEngine::Triangle::Triangle(CacoEngine::Vertex2D p, CacoEngine::Vertex2D p1,
                               CacoEngine::Vertex2D p2, CacoEngine::RGBA color)
    : Object()
{
    this->AddVertex(p);
    this->AddVertex(p1);
    this->AddVertex(p2);
}

CacoEngine::Triangle::Triangle(const Triangle &triangle)
{
    *this = triangle;
}

CacoEngine::Triangle &CacoEngine::Triangle::operator =(const Triangle& triangle)
{
    this->ID = triangle.ID;
    this->Vertices = std::vector<CacoEngine::Vertex2D>();

    for (int x = 0; x < triangle.Vertices.size(); x++)
        this->Vertices.push_back(triangle.Vertices[x]);
    return *this;
}

CacoEngine::Triangle::~Triangle() {}


SDL_Vertex CacoEngine::Vertex2D::GetSDLVertex()
{
    return {
        SDL_FPoint { (float)this->Position.X, (float)this->Position.Y },
        SDL_Color { (uint8_t)this->Color.R, (uint8_t)this->Color.B, (uint8_t)this->Color.G, (uint8_t)this->Color.A },
        SDL_FPoint { (float)this->TextureCoordinates.X, (float)this->TextureCoordinates.Y }
    };
}

void CacoEngine::Object::AddVertex(Vertex2D vertex)
{
    this->Vertices.push_back(vertex);

    if (this->Vertices.size() == 1)
        this->Position = this->Vertices[0].Position;
}

void CacoEngine::Object::Translate(Vector2D difference) {
    this->Position += difference;

    for (int x = 0; x < this->Vertices.size(); x++)
        this->Vertices[x].Position += difference;
}

std::vector<SDL_Vertex> CacoEngine::Object::GetBuffer()
{
    std::vector<SDL_Vertex> sdlVertices = std::vector<SDL_Vertex>();


    sdlVertices.reserve(this->Vertices.size());

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlVertices.push_back(this->Vertices[x].GetSDLVertex());

    return sdlVertices;
}

CacoEngine::Rectangle::Rectangle(Vector2D dimensions, Vector2D position, RGBA color, Texture texture) : Mesh()
{
    this->Position = position;

    this->mTexture = texture;

    this->AddTriangle(Triangle(Vertex2D(Vector2D(position.X, position.Y), color, Vector2D(0, 0)),
                 Vertex2D(Vector2D(position.X, position.Y + dimensions.Y), color, Vector2D(0, 1)),
                 Vertex2D(Vector2D(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2D(1, 1)), color));

    this->AddTriangle(Triangle(Vertex2D(Vector2D(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2D(1, 1)),
                               Vertex2D(Vector2D(position.X + dimensions.X, position.Y), color, Vector2D(1, 0)),
                               Vertex2D(Vector2D(position.X, position.Y), color, Vector2D(0, 0)), color));
}

CacoEngine::Rectangle::~Rectangle() {}
