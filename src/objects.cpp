#include "objects.hpp"
#include "vertex.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <random>

CacoEngine::Object::Object() : ID(0), Position(Vector2Df()), mTexture(Texture()), FillColor(CacoEngine::Colors[(int)CacoEngine::Color::White]), FillMode(RasterizeMode::SolidColor)
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

bool CacoEngine::Object::Equals(Object &object)
{
  return (this->ID == object.ID &&
            this->Position == object.Position &&
            this->FillColor == object.FillColor &&
            this->FillMode == object.FillMode &&
            this->ObjectMesh == object.ObjectMesh);
}

std::vector<SDL_FPoint> CacoEngine::Mesh::GetPoints()
{
    std::vector<SDL_FPoint> sdlPoints = std::vector<SDL_FPoint>();

    for (int x = 0; x < this->Vertices.size(); x++)
        sdlPoints.push_back(this->Vertices[x].GetSDLPoint());

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

bool CacoEngine::Mesh::Equals(Mesh &mesh)
{
  return (this->Vertices == mesh.Vertices);
}

bool CacoEngine::Mesh::operator==(Mesh &mesh)
{
    return this->Equals(mesh);
}

CacoEngine::Object::~Object()
{}

CacoEngine::Triangle::Triangle(CacoEngine::Vertex2Df p, CacoEngine::Vertex2Df p1,
                               CacoEngine::Vertex2Df p2, CacoEngine::RGBA color)
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
    this->ObjectMesh.Vertices = std::vector<CacoEngine::Vertex2Df>();

    for (int x = 0; x < triangle.ObjectMesh.Vertices.size(); x++) // todo: optimize this out
        this->ObjectMesh.Vertices.push_back(triangle.ObjectMesh.Vertices[x]);

    return *this;
}

CacoEngine::Triangle::~Triangle()
{}


void CacoEngine::Object::AddVertex(Vertex2Df vertex)
{
    std::vector<Vertex2Df>& vertices = this->ObjectMesh.Vertices;

    vertices.push_back(vertex);

    if (vertices.size() == 1)
        this->Position = vertices[0].Position;
}

void CacoEngine::Object::Translate(Vector2Df difference)
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

CacoEngine::Mesh::Mesh(std::vector<Vertex2Df> vertices) : Vertices(vertices)
{
}

CacoEngine::Mesh::~Mesh()
{
}

void CacoEngine::Mesh::AddTriangle(Vertex2Df vertex, Vertex2Df vertex1, Vertex2Df vertex2)
{
    this->Vertices.push_back(vertex);
    this->Vertices.push_back(vertex1);
    this->Vertices.push_back(vertex2);
}

CacoEngine::Rectangle::Rectangle(Vector2Df dimensions, Vector2Df position, RGBA color, Texture texture) : Object()
{
    this->Position = position;
    this->mTexture = texture;

 
    this->ObjectMesh.AddTriangle(Vertex2Df(Vector2Df(position.X, position.Y), color, Vector2Df(0, 0)),
                 Vertex2Df(Vector2Df(position.X, position.Y + dimensions.Y), color, Vector2Df(0, 1)),
                 Vertex2Df(Vector2Df(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2Df(1, 1)));

    this->ObjectMesh.AddTriangle(Vertex2Df(Vector2Df(position.X + dimensions.X, position.Y + dimensions.Y), color, Vector2Df(1, 1)),
                               Vertex2Df(Vector2Df(position.X + dimensions.X, position.Y), color, Vector2Df(1, 0)),
                               Vertex2Df(Vector2Df(position.X, position.Y), color, Vector2Df(0, 0)));
}

CacoEngine::Rectangle::~Rectangle() {}


void CacoEngine::Circle::GeneratePoints()
{
    const float diameter = (this->Radius * 2);

    float x = (this->Radius - 1);
    float y = 0;
    float tx = 1;
    float ty = 1;
    float error = (tx - diameter);

    while( x >= y)
    {
        // Each of the following renders an octant of the circle
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X + x, (float)this->Position.Y - y), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X + x, (float)this->Position.Y + y), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X - x, (float)this->Position.Y - y), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X - x, (float)this->Position.Y + y), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X + y, (float)this->Position.Y - x), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X + y, (float)this->Position.Y + x), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X - y, (float)this->Position.Y - x), this->FillColor));
        this->ObjectMesh.Vertices.push_back(Vertex2Df(Vector2Df((float)this->Position.X - y, (float)this->Position.Y + x), this->FillColor));

        if( error <= 0 )
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if( error > 0 )
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

double CacoEngine::Circle::GetRadius()
{
    return this->Radius;
}


CacoEngine::Vector2Df CacoEngine::Circle::GetCenter()
{
    return this->Position;
}

void CacoEngine::Circle::SetRadius(double radius)
{
    this->Radius = radius;

    this->GeneratePoints();
}

CacoEngine::Circle::Circle(Vector2Df center,double radius) : Object(), Radius(radius)
{
    this->Position = center;
    this->FillMode = RasterizeMode::Points;
    this->GeneratePoints();
}

CacoEngine::Circle::~Circle()
{
}
