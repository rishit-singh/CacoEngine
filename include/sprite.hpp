#ifndef SPRITE_H_
#define SPRITE_H_

#include "objects.hpp"
#include "texture.hpp"

namespace CacoEngine
{
    class Sprite : public Rectangle
    {
    public:
            Sprite(Texture, Vector2D = Vector2D(), Vector2D = Vector2D());
            ~Sprite();
    };
};

#endif // SPRITE_H_
