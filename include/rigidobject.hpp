#ifndef RIGIDOBJECT_H_
#define RIGIDOBJECT_H_

#include "rigidbody.hpp"

namespace CacoEngine
{
    class RigidObject2D : public Object
    {
    public:
        RigidBody2D RigidBody;

        bool CollidesWith(RigidObject2D&);

        // RigidObject2D& operator =(const RigidObject2D&);

        RigidObject2D();
        // RigidObject2D(const RigidBody2D&);

        virtual ~RigidObject2D();
    };
}

#endif
