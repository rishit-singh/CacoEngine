#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include "vertex.hpp"
#include "objects.hpp"

namespace CacoEngine
{
    class RigidBody2D
    {
    public:
        Vector2D Velocity { Vector2D(0, 0) }; 

        Vector2D Force { Vector2D(0, 0) };

        uint64_t LastUpdate;

        void AddForce(Vector2D);
        
        RigidBody2D(Vector2D = Vector2D(), Vector2D = Vector2D());

        virtual ~RigidBody2D();
    };
}

#endif
