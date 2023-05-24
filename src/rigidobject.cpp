#include "rigidobject.hpp"
#include "objects.hpp"
#include "vertex.hpp"

CacoEngine::RigidObject2D::RigidObject2D() : Object()
{
}

CacoEngine::RigidObject2D::RigidObject2D(const CacoEngine::RigidObject2D& object)
{
    *this = object;
}

CacoEngine::RigidObject2D& CacoEngine::RigidObject2D::operator =(const CacoEngine::RigidObject2D& object)
{
    this->ID = object.ID;
    this->Position = object.Position;
    this->ObjectMesh.Vertices = object.ObjectMesh.Vertices;
    this->FillColor = object.FillColor;
    this->FillMode = object.FillMode;
    this->RigidBody = object.RigidBody;

    return *this;
}

CacoEngine::RigidObject2D::~RigidObject2D()
{
}

bool CacoEngine::RigidObject2D::CollidesWith(CacoEngine::RigidObject2D object)
{
    return false;
}

bool CacoEngine::RigidObject2D::CollidesWith(CacoEngine::Vector2Df point)
{
    return false;
}

CacoEngine::RigidCircle::RigidCircle(CacoEngine::Vector2Df origin, double radius)
    : RigidObject2D(), mCircle(Circle(origin, radius))
{
    this->Sync();
}

CacoEngine::RigidCircle::~RigidCircle() {}

bool CacoEngine::RigidCircle::CollidesWith(Vector2Df point)
{
    std::cout << "CollidesWith(Vector2Df)\n";

    double distance = this->Position.DistanceFrom(point);

    std::cout << "Distance: " << distance << '\n';

     std::cout << "Position: "
              << "(" << this->Position.X << ", " << this->Position.Y << ")\n";

    return (distance < this->GetRadius());
}

void CacoEngine::RigidCircle::Sync()
{
    this->ObjectMesh = this->mCircle.ObjectMesh;
}

double CacoEngine::RigidCircle::GetRadius()
{
    return this->mCircle.GetRadius();
}

void CacoEngine::RigidCircle::SetRadius(double radius)
{
    this->mCircle.SetRadius(radius);
    this->Sync();
}

bool CacoEngine::RigidCircle::CollidesWith(Circle &circle)
{
    return ((this->Position.DistanceFrom(circle.Position) - (circle.GetRadius())) < 0);
}

// CacoEngine::Box2D::Box2D(CacoEngine::Vector2D dimensions, Vector2D position, RGBA color) : Rectangle(dimensions, position, color), RigidBody2D()
// {
// }

