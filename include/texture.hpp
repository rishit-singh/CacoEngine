#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <vector>
#include <string_view>
#include "renderer.hpp"
#include "surface.hpp"

namespace CacoEngine
{
    class Texture
    {
    public:
        int ID;

        SDL_Texture* mTexture;

        Texture(int = 0, SDL_Texture* = nullptr);
        Texture(const Texture&);

        Texture& operator =(const Texture&);

        ~Texture();
    };

    class TextureManager
    {
    public:
        static std::vector<Texture> Textures; // = std::vector<Texture>();

        static Texture CreateTexture(std::string_view path, Renderer& renderer)
        {
            Surface surface = Surface(IMG_Load(path.data()));

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetInstance(), surface.Screen);
        
            return Texture(TextureManager::Textures.size(), texture);
        }
    };
};

#endif // TEXTURE_H_
