#ifndef RIGIDOBJECT_H_
#define RIGIDOBJECT_H_

#include "rigidbody.hpp"
#include <cmath>

namespace CacoEngine
{
    class RigidObject2D : public Object
    {
    public:
        RigidBody2D RigidBody;

        virtual bool CollidesWith(RigidObject2D);
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

            bool CollidesWith(RigidCircle&);

            virtual bool CollidesWith(Vector2Df);

            RigidCircle(Vector2Df, double = 1.0f);
            RigidCircle(RigidObject2D&);

            RigidCircle& operator =(RigidObject2D&);

            virtual ~RigidCircle();
    };
}

#endif
