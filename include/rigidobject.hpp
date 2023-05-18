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

        RigidObject2D& operator =(const RigidObject2D&);

        RigidObject2D();
        RigidObject2D(const RigidObject2D&);

        virtual ~RigidObject2D();
    };


    class RigidCircle : public RigidObject2D
    {
    protected:
            Circle mCircle;

            void Sync();
    public:
            double GetRadius();
            void SetRadius(double);

            RigidCircle(Vector2Df, double = 1.0f);
            virtual ~RigidCircle();
    };
}

#endif
