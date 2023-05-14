#include "sprite.hpp"
#include "texture.hpp"

CacoEngine::Sprite::Sprite(CacoEngine::Texture texture, Vector2D dimensions, Vector2D position)
    : Rectangle(dimensions, position,
                CacoEngine::Colors[(int)CacoEngine::Color::White], texture)
{
    this->FillMode = RasterizeMode::Texture;
}

CacoEngine::Sprite::~Sprite() {}
