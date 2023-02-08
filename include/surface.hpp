#ifndef SURFACE_H_
#define SURFACE_H_

#include <SDL2/SDL.h>
#include <SDL_video.h>

#include "objects.hpp"

namespace CacoEngine
{
    class Surface
    {
    public:
        SDL_Surface* Screen;

        void SetPixel(Vertex2D);

        Surface(SDL_Window* = nullptr);
        ~Surface();
    };
} // namespace CacoEngine

#endif // SURFACE_H_
