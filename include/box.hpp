#ifndef BOX_H_
#define BOX_H_

#include "objects.hpp"
    #include "rigidobject.hpp"

namespace CacoEngine
{
    class Box2D : public Rectangle, public RigidObject2D
    {
    public:
        Box2D(Vector2D, Vector2D, RGBA = Colors[(int)Color::White]);

        virtual ~Box2D();
    };
}

#endif
