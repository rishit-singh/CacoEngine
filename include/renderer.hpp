#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include "vertex.hpp"

namespace CacoEngine
{

    class Renderer
    {
    public:
            SDL_Renderer* Instance;

            RGBA Color;

            void Clear(RGBA = RGBA());
            void SetColor(RGBA);

            Renderer(SDL_Window* = nullptr);
            ~Renderer();
    };
}

#endif // RENDERER_H_
