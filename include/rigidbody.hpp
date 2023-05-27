#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include "vertex.hpp"
#include "objects.hpp"

namespace CacoEngine
{
    class RigidBody2D
    {
    public:
        Vector2Df Velocity { Vector2Df(0, 0) };

        Vector2Df Acceleration { Vector2Df(0, 0) };

        Vector2Df Force { Vector2Df(0, 0) };

        double Mass { 1.0f };

        uint64_t LastUpdate;

        void AddForce(Vector2Df);

        void UpdateAcceleration();

        RigidBody2D(Vector2Df = Vector2Df(), Vector2Df = Vector2Df(), Vector2Df = Vector2Df(), double = 1.0f);

        virtual ~RigidBody2D();
    };
}

#endif
