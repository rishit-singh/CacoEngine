#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include "vertex.hpp"

namespace CacoEngine
{

    class Renderer
    {
    private:
            SDL_Renderer* Instance;

    public:

            RGBA Color;

            void Clear(RGBA = RGBA());
            void SetColor(RGBA);

            SDL_Renderer* GetInstance();

            Renderer(SDL_Window* = nullptr);
            ~Renderer();
    };
}

#endif // RENDERER_H_
