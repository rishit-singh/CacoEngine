#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <SDL2/SDL.h>
#include <vector>
#include <array>

namespace CacoEngine
{
    struct RGBA
    {
        int R;
        int G;
        int B;
        int A;

        RGBA(int = 0, int = 0, int = 0, int = 255);
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

        RGBA Color;

        SDL_Vertex GetSDLVertex();

        Vertex2D(Vector2D = Vector2D(), RGBA = RGBA());
    };

    enum class Color
    {
        Red,
        Blue,
        Green,
        White,
        Black
    };

    extern RGBA Colors[5];

    class Object
    {
    public:
            int ID;

            std::vector<Vertex2D> Vertices;

            virtual std::vector<SDL_Vertex> GetBuffer();

            void AddVertex(Vertex2D);

            Object();
            Object(const Object&);

            Object& operator =(const Object& object);

            ~Object();
    };

    class Triangle : public Object
    {
    public:
            Triangle(Vertex2D, Vertex2D, Vertex2D, RGBA);
            Triangle(const Triangle&);

            Triangle& operator =(const Triangle&);

            ~Triangle();
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
            }

            std::vector<SDL_Vertex> GetBuffer() override
            {
                std::vector<SDL_Vertex> vertices = std::vector<SDL_Vertex>(),
                    temp;

                for (int x = 0; x < this->Triangles.size(); x++)
                {
                    temp = this->Triangles[x].GetBuffer();

                    for (int y = 0; y < temp.size(); x++)
                        vertices.push_back(temp[y]);//vertices.push_back(this->Trianges[x]);
                }
                
                return vertices;
            }

            Mesh(std::vector<Triangle> triangles = std::vector<Triangle>()) : Object(), Triangles(triangles)
            {
            }

            ~Mesh()
            {
            }
    };
}

#endif // OBJECTS_H_
