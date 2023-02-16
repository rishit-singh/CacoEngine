#include "texture.hpp"
#include <SDL_render.h>



CacoEngine::Texture::Texture(int id, SDL_Texture* texture)
{
    this->ID = id;
    this->mTexture = texture;
}

CacoEngine::Texture::Texture(const CacoEngine::Texture &texture)
{
    *this = texture;
}

CacoEngine::Texture &CacoEngine::Texture::operator=(const Texture &texture)
{
    this->ID = texture.ID;
    this->mTexture = texture.mTexture;

    return *this;
}

CacoEngine::Texture::~Texture()
{
}

std::vector<CacoEngine::Texture> CacoEngine::TextureManager::Textures = std::vector<CacoEngine::Texture>();
