#ifndef RIGIDSPRITE_H_
#define RIGIDSPRITE_H_

#include "sprite.hpp"
#include "rigidobject.hpp"
#include "vertex.hpp"
namespace CacoEngine
{
    class RigidSprite : public CacoEngine::RigidObject2D
    {
    protected:
            Sprite mSprite;

    public:

        RigidSprite(Texture, Vector2Df, Vector2Df);

        virtual ~RigidSprite();
    };
}

#endif // RIGIDSPRITE_H_
