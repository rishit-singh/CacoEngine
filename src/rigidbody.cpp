#include "rigidbody.hpp"

CacoEngine::RigidBody2D::RigidBody2D(CacoEngine::Vector2Df velocity, CacoEngine::Vector2Df acceleration, CacoEngine::Vector2Df force)
    : Velocity(velocity), Acceleration(acceleration), Force(force), LastUpdate(SDL_GetTicks64())
{
}

CacoEngine::RigidBody2D::~RigidBody2D()
{
}
