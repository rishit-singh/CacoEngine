#include "objects.hpp"
#include "vertex.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <random>

CacoEngine::Object::Object() : ID(0), Position(Vector2D()), mTexture(Texture()), FillColor(CacoEngine::Colors[(int)CacoEngine::Color::White]), FillMode(RasterizeMode::SolidColor)
{
}

CacoEngine::Object::Object(const Object &object)
{
    *this = object;
}

CacoEngine::Object& CacoEngine::Object::operator =(const Object& object)
{
    this->ID = object.ID;
    this->ObjectMesh.Vertices = object.ObjectMesh.Vertices;
    this->mTexture = object.mTexture;
    this->Position = this->ObjectMesh.Vertices[0].Position;
    this->FillColor = object.FillColor;
    this->FillMode = object.FillMode;

    return *this;
}

std::vector<SDL_Point> CacoEngine::Mesh::GetPoints()
{
    std::vector<SDL_Point> sdlPoints = std::vector<SDL_Point>();

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlPoints.push_back(this->Vertices[x].GetSDLPoint());

    return sdlPoints;
}

std::vector<SDL_FPoint> CacoEngine::Mesh::GetPointsF()
{
    std::vector<SDL_FPoint> sdlPoints = std::vector<SDL_FPoint>();

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlPoints.push_back(this->Vertices[x].GetSDLPointF());

    return sdlPoints;
}

std::vector<SDL_Vertex> CacoEngine::Mesh::GetVertexBuffer()
{
    std::vector<SDL_Vertex> sdlVertices = std::vector<SDL_Vertex>();

    sdlVertices.reserve(this->Vertices.size());

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlVertices.push_back(this->Vertices[x].GetSDLVertex());

    return sdlVertices;
}

CacoEngine::Object::~Object()
{}

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
    this->ObjectMesh.Vertices = std::vector<CacoEngine::Vertex2D>();

    for (int x = 0; x < triangle.ObjectMesh.Vertices.size(); x++) // todo: optimize this out
        this->ObjectMesh.Vertices.push_back(triangle.ObjectMesh.Vertices[x]);

    return *this;
}

CacoEngine::Triangle::~Triangle() {}



void CacoEngine::Object::AddVertex(Vertex2D vertex)
{
    std::vector<Vertex2D>& vertices = this->ObjectMesh.Vertices;

    vertices.push_back(vertex);

    if (vertices.size() == 1)
        this->Position = vertices[0].Position;
}

void CacoEngine::Object::Translate(Vector2D difference) 
{
    this->Position += difference;

    for (int x = 0; x < this->ObjectMesh.Vertices.size(); x++)
        this->ObjectMesh.Vertices[x].Position += difference;
}


void CacoEngine::Object::SetFillColor(RGBA color)
{
    for (int x = 0; x < this->ObjectMesh.Vertices.size(); x++)
        this->ObjectMesh.Vertices[x].Color = color;
}

CacoEngine::Mesh::Mesh(std::vector<Vertex2D> vertices) : Vertices(vertices)
{
}

CacoEngine::Mesh::~Mesh()
{
}

void CacoEngine::Mesh::AddTriangle(Vertex2D vertex, Vertex2D vertex1, Vertex2D vertex2)
{
    this->Vertices.push_back(vertex);
    this->Vertices.push_back(vertex1);
    this->Vertices.push_back(vertex2);
}

CacoEngine::Rectangle::Rectangle(Vector2D dimensions, Vector2D position, RGBA color, Texture texture) : Object()
{
    this->Position = position;
    this->mTexture = texture;

 
    this->ObjectMesh.AddTriangle(Vertex2D(Vector2D(position.X, position.Y), color, Vector2D(0, 0)),
                 Vertex2D(Vector2D(position.X, position.Y + dimensions.Y), color, Vector2D(0, 1)),
                 Vertex2D(Vector2D(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2D(1, 1)));

    this->ObjectMesh.AddTriangle(Vertex2D(Vector2D(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2D(1, 1)),
                               Vertex2D(Vector2D(position.X + dimensions.X, position.Y), color, Vector2D(1, 0)),
                               Vertex2D(Vector2D(position.X, position.Y), color, Vector2D(0, 0)));
}

CacoEngine::Rectangle::~Rectangle() {}

