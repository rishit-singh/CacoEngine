#include "objects.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <random>

CacoEngine::Vector2D::Vector2D(int x, int y) :  X(x), Y(y)
{
}

CacoEngine::Vertex2D::Vertex2D(Vector2D position, RGBA color) : Position(position), Color(color)
{
}

CacoEngine::RGBA::RGBA(int r, int g, int b, int a) : R(r), G(g), B(b), A(a)
{
}

CacoEngine::RGBA CacoEngine::Colors[5] = {RGBA(255, 0, 0), RGBA(0, 255, 0),
                                         RGBA(0, 0, 255), RGBA(255, 255, 255),
                                         RGBA(0, 0, 0)};

CacoEngine::Object::Object() : ID(0) {}

CacoEngine::Object::Object(const Object &object) {
    *this = object;
}

CacoEngine::Object& CacoEngine::Object::operator =(const Object& object) {
    this->ID = object.ID;
    this->Vertices = object.Vertices;

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

CacoEngine::Triangle::Triangle(const Triangle &triangle) {
    *this = triangle;
}

CacoEngine::Triangle &CacoEngine::Triangle::operator =(const Triangle& triangle) {
    this->ID = triangle.ID;
    this->Vertices = std::vector<CacoEngine::Vertex2D>();

    for (int x = 0; x < triangle.Vertices.size(); x++)
        this->Vertices.push_back(triangle.Vertices[x]);
}

CacoEngine::Triangle::~Triangle() {}


SDL_Vertex CacoEngine::Vertex2D::GetSDLVertex()
{
    return {
        SDL_FPoint { (float)this->Position.X, (float)this->Position.Y },
        SDL_Color { (uint8_t)this->Color.R, (uint8_t)this->Color.B, (uint8_t)this->Color.G, (uint8_t)this->Color.A },
        SDL_FPoint { 0 }
    };
}

void CacoEngine::Object::AddVertex(Vertex2D vertex)
{
    this->Vertices.push_back(vertex);
}

std::vector<SDL_Vertex> CacoEngine::Object::GetBuffer()
{
    std::vector<SDL_Vertex> sdlVertices = std::vector<SDL_Vertex>();


    sdlVertices.reserve(this->Vertices.size());

    SDL_Vertex vertex;

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlVertices.push_back(this->Vertices[x].GetSDLVertex());

    return sdlVertices;
}
