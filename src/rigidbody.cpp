#include "rigidbody.hpp"
#include "vertex.hpp"

CacoEngine::RigidBody2D::RigidBody2D(CacoEngine::Vector2Df velocity, CacoEngine::Vector2Df acceleration, CacoEngine::Vector2Df force, double mass)
    : Velocity(velocity), Acceleration(acceleration), Force(force), Mass(mass), LastUpdate(SDL_GetTicks64())
{
}


void CacoEngine::RigidBody2D::AddForce(Vector2Df force)
{
    this->Force += force;
    this->UpdateAcceleration();
}

void CacoEngine::RigidBody2D::UpdateAcceleration()
{
    this->Acceleration = this->Force / this->Mass;
}

CacoEngine::RigidBody2D::~RigidBody2D()
{
}
