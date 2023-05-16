#include "box.hpp"


CacoEngine::Box2D::Box2D(Vector2D dimensions, Vector2D position, RGBA color) : Rectangle(dimensions, position, color), RigidObject2D()
{
}

CacoEngine::Box2D::~Box2D()
{
}
