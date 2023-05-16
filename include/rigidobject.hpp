#ifndef RIGIDOBJECT_H_
#define RIGIDOBJECT_H_

#include "rigidbody.hpp"

namespace CacoEngine
{
    class RigidObject2D : public Object
    {
    public:
        RigidBody2D RigidBody;

        virtual bool CollidesWith(RigidObject2D&);
        virtual bool CollidesWith(Vector2Df);

        // RigidObject2D& operator =(const RigidObject2D&);

        RigidObject2D();
        // RigidObject2D(const RigidBody2D&);

        virtual ~RigidObject2D();
    };
}

#endif
