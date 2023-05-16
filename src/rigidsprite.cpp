#include "rigidsprite.hpp"
#include "rigidobject.hpp"
#include "texture.hpp"


CacoEngine::RigidSprite::RigidSprite(CacoEngine::Texture texture,
                                     Vector2Df dimensions, Vector2Df position)
    : CacoEngine::RigidObject2D(),
      mSprite(Sprite(texture, dimensions, position)) {
    this->ObjectMesh = this->mSprite.ObjectMesh;
    this->mTexture = this->mSprite.mTexture;
    this->Position = this->mSprite.Position;
    this->FillMode = RasterizeMode::Texture;
}

CacoEngine::RigidSprite::~RigidSprite()
{
}
