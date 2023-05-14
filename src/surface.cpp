#include "surface.hpp"
#include <SDL_surface.h>

namespace CacoEngine
{
    Surface::Surface(SDL_Window *window)
    {
        if (window)
            this->Screen = SDL_GetWindowSurface(window);
    
    }

    Surface::Surface(SDL_Surface* surface) : Screen(surface)
    {
    }

    Surface::~Surface()
    {
        SDL_FreeSurface(this->Screen);
    }

    void Surface::SetPixel(CacoEngine::Vertex2D pixel)
    {
        SDL_LockSurface(this->Screen);

        uint8_t* pixelArray = (uint8_t*)this->Screen->pixels;

        pixelArray[pixel.Position.Y * this->Screen->pitch + pixel.Position.X * this->Screen->format->BytesPerPixel + 0] = pixel.Color.G;
        pixelArray[pixel.Position.Y * this->Screen->pitch + pixel.Position.X * this->Screen->format->BytesPerPixel + 1] = pixel.Color.B;
        pixelArray[pixel.Position.Y * this->Screen->pitch + pixel.Position.X * this->Screen->format->BytesPerPixel + 2] = pixel.Color.R;

        SDL_UnlockSurface(this->Screen);
    }
}
 

 
