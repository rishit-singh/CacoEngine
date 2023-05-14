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

        virtual void CollidesWith(RigidBody2D&) = 0; 

        RigidBody2D();
    };
}

#endif
