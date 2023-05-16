#include "rigidbody.hpp"

CacoEngine::RigidBody2D::RigidBody2D(CacoEngine::Vector2D velocity, CacoEngine::Vector2D acceleration, CacoEngine::Vector2D force)
    : Velocity(velocity), Acceleration(acceleration), Force(force), LastUpdate(SDL_GetTicks64())
{
}

CacoEngine::RigidBody2D::~RigidBody2D()
{
}
