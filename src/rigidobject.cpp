#include "rigidobject.hpp"
#include "vertex.hpp"

CacoEngine::RigidObject2D::RigidObject2D() : Object()
{
}

// CacoEngine::RigidObject2D::RigidObject2D(const CacoEngine::RigidObject2D& object)
// {
//     *this = object;
// }


// CacoEngine::RigidObject2D& CacoEngine::RigidObject2D::operator =(const CacoEngine::RigidObject2D& object)
// {
//     this->ID = object.ID;
//     this->Position = object.Position;
//     this->ObjectMesh.Vertices = object.ObjectMesh.Vertices;
//     this->FillColor = object.FillColor;
//     this->FillMode = object.FillMode;
//     this->RigidBody = object.RigidBody;

//     return *this;
// }

CacoEngine::RigidObject2D::~RigidObject2D()
{
}

bool CacoEngine::RigidObject2D::CollidesWith(CacoEngine::RigidObject2D& object)
{
    return false;
}

bool CacoEngine::RigidObject2D::CollidesWith(CacoEngine::Vector2Df point)
{
    return false;
}
// CacoEngine::Box2D::Box2D(CacoEngie::Vector2D dimensions, Vector2D position, RGBA color) : Rectangle(dimensions, position, color), RigidBody2D()
// {
// }

