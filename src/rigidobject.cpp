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


CacoEngine::RigidCircle::RigidCircle(CacoEngine::RigidObject2D &object)
    : mCircle(Vector2Df(0, 0))
{
    *this = object;

    this->mCircle = CacoEngine::Circle(this->Position, this->Position.DistanceFrom(this->ObjectMesh.Vertices[0].Position));
    this->Sync();
}

CacoEngine::RigidCircle& CacoEngine::RigidCircle::operator=(RigidObject2D &object) {
    this->ObjectMesh = object.ObjectMesh;
    this->RigidBody = object.RigidBody;
    this->FillColor = object.FillColor;
    this->FillMode = object.FillMode;
    this->mTexture = object.mTexture;

    return *this;
}

CacoEngine::RigidCircle::~RigidCircle() {}

bool CacoEngine::RigidCircle::CollidesWith(Vector2Df point)
{
    std::cout << "CollidesWith(Vector2Df)\n";

    double distance = this->Position.DistanceFrom(point);

    std::cout << "Distance: " << distance << '\n';


    return (distance < this->GetRadius());
}


void CacoEngine::RigidCircle::Sync()
{
    this->Position = this->mCircle.Position;
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

bool CacoEngine::RigidCircle::CollidesWith(RigidCircle &circle)
{
    std::cout << "CenterDistance: " << (this->Position.DistanceFrom(circle.Position)) << std::endl; //  - (circle.GetRadius() + this->GetRadius()));

    CacoEngine::Vector2Df center = this->mCircle.GetCenter(), center1 = circle.Position;


    return (fabs((center.X - center1.X) * (center.X - center1.X) + (center.Y - center1.Y) * (center.Y - center1.Y)) <= (this->GetRadius() + circle.GetRadius() * (this->GetRadius() + circle.GetRadius()))); //  - (circle.GetRadius() + this->GetRadius())) <= 0);
}

// CacoEngine::Box2D::Box2D(CacoEngine::Vector2D dimensions, Vector2D position, RGBA color) : Rectangle(dimensions, position, color), RigidBody2D()
// {
// }
