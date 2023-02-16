#include "engine.hpp"
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <iostream>
#include <vector>
#include "objects.hpp"
#include "texture.hpp"

namespace CacoEngine
{
    void Engine::AddExtension(Extension extension)
    {
        this->Extensions.push_back(extension);
    }

    void Engine::Initialize()
    {
        this->ExtensionBits = (int)this->Extensions[0];

        for (int x = 1; x < this->Extensions.size(); x++)
          this->ExtensionBits |= (int)this->Extensions[x];

        if (SDL_Init(this->ExtensionBits) == -1) {
            std::cout << "Failed to initialize SDL: " << SDL_GetError() << '\n';
            return;
        }
    }

    void Engine::OnKeyPress(SDL_KeyboardEvent& event)
    {
        if (event.keysym.sym == SDLK_ESCAPE)
            this->IsRunning = false;
    }

    void Engine::OnMouseClick(SDL_MouseButtonEvent& event)
    {
    }

    void Engine::OnMouseScroll(SDL_MouseWheelEvent& event)
    {
    }

    void Engine::OnInitialize()
    {
    }

    void Engine::OnUpdate(int frame)
    {}


    void Engine::Run()
    {
        this->IsRunning = true;
        this->Window = SDL_CreateWindow(this->Title.data(), 0, 2500, this->Resolution.X, this->Resolution.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        this->EngineRenderer = Renderer(this->Window);

        IMG_Init(IMG_INIT_PNG);

        this->OnInitialize();

        while (this->IsRunning)
        {
            while (SDL_PollEvent(&this->Event))
                switch (this->Event.type)
                {
                    case SDL_QUIT:
                        this->IsRunning = false;

                        break;
                    case SDL_KEYDOWN:
                        if (this->Event.key.keysym.sym == SDLK_ESCAPE)
                            this->IsRunning = false;

                        this->OnKeyPress(this->Event.key);
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        SDL_GetMouseState(&this->CursorPosition.X, &this->CursorPosition.Y);
                        this->OnMouseClick(this->Event.button);
                        break;

                    case SDL_MOUSEWHEEL:
                        this->OnMouseScroll(this->Event.wheel);
                        break;
                }


            this->EngineRenderer.Clear();
            this->EngineRenderer.SetColor(Colors[(int)Color::White]);



            for (int x = 0; x < this->Objects.size(); x++)
                SDL_RenderGeometry(this->EngineRenderer.Instance, this->Objects[x].mTexture.mTexture, this->Objects[x].GetBuffer().data(), this->Objects[x].Vertices.size(), nullptr, 0);


            SDL_RenderPresent(this->EngineRenderer.Instance);
            SDL_Delay(0);

            this->OnUpdate(++this->Frame);
        }
    }



    Engine::Engine(std::string_view title, Vector2D resolution, bool initialize)
        : Title(title), Resolution(resolution), IsRunning(false), Frame(0)
    {
        this->Extensions = {
            Extension::Video,
            Extension::Audio
        };

        if (initialize)
            this->Initialize();
    }

    Engine::~Engine()
    {
        SDL_DestroyWindow(this->Window);

        IMG_Quit();
        SDL_Quit();

        std::cout << "SDL Aborted.";
    }
}
