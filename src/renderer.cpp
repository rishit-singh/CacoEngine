#include "renderer.hpp"
#include <SDL_pixels.h>
#include <SDL_render.h>

CacoEngine::Renderer::Renderer(SDL_Window *window)
{
    if (window)
        this->Instance = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

CacoEngine::Renderer::~Renderer() {}

void CacoEngine::Renderer::Clear(CacoEngine::RGBA color)
{
    this->Color = color;

    SDL_SetRenderDrawColor(this->Instance, this->Color.R, this->Color.G, this->Color.B, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->Instance);
}

void CacoEngine::Renderer::SetColor(RGBA color)
{
    this->Color = color;

    SDL_SetRenderDrawColor(this->Instance, this->Color.R, this->Color.G, this->Color.B, SDL_ALPHA_OPAQUE);
}

SDL_Renderer *CacoEngine::Renderer::GetInstance()
{
    return this->Instance;
}
