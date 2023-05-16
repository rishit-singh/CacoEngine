#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <vector>
#include <array>
#include <iostream>
#include "vertex.hpp"
#include "texture.hpp"


namespace CacoEngine
{
    enum class RasterizeMode
    {
        WireFrame,
        SolidColor,
        Texture
    };

    class Mesh
    {
    protected:
        void Initialize();

    public:
        std::vector<Vertex2D> Vertices;

        void AddTriangle(Vertex2D, Vertex2D, Vertex2D);

        std::vector<SDL_Vertex> GetVertexBuffer();

        std::vector<SDL_FPoint> GetPointsF();

        std::vector<SDL_Point> GetPoints();

        Mesh(std::vector<Vertex2D>  = std::vector<Vertex2D>());

        virtual ~Mesh();
    };


    class Object
    {
    public:
        int ID;

        Vector2D Position;

        Texture mTexture;

        RGBA FillColor;

        RasterizeMode FillMode;

        Mesh ObjectMesh;

        void AddVertex(Vertex2D);

        void Translate(Vector2D);

        void SetFillColor(RGBA);

        Object();
        Object(const Object&);

        Object& operator =(const Object& object);

        virtual ~Object();
    };

    class Triangle : public Object
    {
    public:
        Triangle(Vertex2D, Vertex2D, Vertex2D, RGBA);
        Triangle(const Triangle&);

        Triangle& operator =(const Triangle&);

        virtual ~Triangle();
    };

    class Rectangle : public Object
    {
    public:
        Vector2D Dimensions;

        Rectangle(Vector2D, Vector2D, RGBA = Colors[(int)Color::White], Texture = Texture());
        virtual ~Rectangle();
    };
}

#endif // OBJECTS_H_
