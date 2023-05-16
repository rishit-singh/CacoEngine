#include "box.hpp"
#include "objects.hpp"

CacoEngine::Box2D::Box2D(Vector2D dimensions, Vector2D position, RGBA color) : RigidObject2D()
{
    this->ObjectMesh = Rectangle(dimensions, position, color).ObjectMesh;
}

CacoEngine::Box2D::~Box2D()
{
}
