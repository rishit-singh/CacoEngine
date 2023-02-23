#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <vector>
#include <array>
#include <iostream>
#include "vertex.hpp"
#include "texture.hpp"


namespace CacoEngine
{

    class Object
    {
    public:
            int ID;

            Vector2D Position;

            Texture mTexture;

            std::vector<Vertex2D> Vertices;

            virtual std::vector<SDL_Vertex> GetBuffer();

            void AddVertex(Vertex2D);

            void Translate(Vector2D);

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

    class Mesh : public Object
    {
    protected:
            void Initialize();
    public:
            std::vector<Triangle> Triangles;

            void AddTriangle(Triangle triangle)
            {
                this->Triangles.push_back(triangle);

                int end = this->Triangles.size() - 1;

                for (int x = 0; x < 3; x++)
                    this->AddVertex(this->Triangles[end].Vertices[x]);
            }

            Mesh(std::vector<Triangle> triangles = std::vector<Triangle>()) : Object(), Triangles(triangles)
            {
                for (int x = 0; x < triangles.size(); x++)
                    this->AddTriangle(triangles[x]);
            }

            virtual ~Mesh()
            {
            }
    };

    class Rectangle : public Mesh
    {
    public:
            Vector2D Dimensions;


            Rectangle(Vector2D, Vector2D, RGBA = Colors[(int)Color::White], Texture = Texture());
            virtual ~Rectangle();
    };
}

#endif // OBJECTS_H_
