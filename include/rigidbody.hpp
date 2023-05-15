#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include "vertex.hpp"

namespace CacoEngine
{
    class RigidBody2D
    {
    public:
        Vector2D Velocity { Vector2D(0, 0) }; 

        Vector2D Force { Vector2D(0, 0) };

        uint64_t LastUpdate;

        virtual void CollidesWith(RigidBody2D&) = 0; 

        RigidBody2D(Vector2D = Vector2D(), Vector2D = Vector2D());
    };
}

#endif
