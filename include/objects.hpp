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
        Points,
        WireFrame,
        SolidColor,
        Texture
    };

    class Mesh
    {
    protected:
        void Initialize();

    public:
        std::vector<Vertex2Df> Vertices;

        void AddTriangle(Vertex2Df, Vertex2Df, Vertex2Df);

        std::vector<SDL_Vertex> GetVertexBuffer();

        std::vector<SDL_FPoint> GetPoints();

        Mesh(std::vector<Vertex2Df>  = std::vector<Vertex2Df>());

        virtual ~Mesh();
    };


    class Object
    {
    public:
        int ID;

        Vector2Df Position;

        Texture mTexture;

        RGBA FillColor;

        RasterizeMode FillMode;

        Mesh ObjectMesh;

        void AddVertex(Vertex2Df);

        void Translate(Vector2Df);

        void SetFillColor(RGBA);

        Object();
        Object(const Object&);

        Object& operator =(const Object& object);

        virtual ~Object();
    };

    class Triangle : public Object
    {
    public:
        Triangle(Vertex2Df, Vertex2Df, Vertex2Df, RGBA);
        Triangle(const Triangle&);

        Triangle& operator =(const Triangle&);

        virtual ~Triangle();
    };

    class Rectangle : public Object
    {
    public:
        Vector2Df Dimensions;

        Rectangle(Vector2Df, Vector2Df, RGBA = Colors[(int)Color::White], Texture = Texture());
        virtual ~Rectangle();
    };

    class Circle : public Object
    {
    protected:
            void GeneratePoints();

            double Radius;
    public:
            double GetRadius();

            Vector2Df GetCenter();

            void SetRadius(double);

            Circle(Vector2Df, double =  1);
            virtual ~Circle();
    };
}

#endif // OBJECTS_H_
