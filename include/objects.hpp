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

   class Object
    {
    public:
	    int ID;

	    Vector2D Position;

	    Texture mTexture;

	    RGBA FillColor;

		RasterizeMode FillMode;

	    std::vector<Vertex2D> Vertices;

	    virtual std::vector<SDL_Vertex> GetVertexBuffer();

		virtual std::vector<SDL_FPoint> GetPointsF();

		virtual std::vector<SDL_Point> GetPoints();
	 
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

		void SetFillColor(RGBA);

	    void AddTriangle(Triangle triangle);

	    Mesh(std::vector<Triangle> triangles = std::vector<Triangle>());

	    virtual ~Mesh();
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
