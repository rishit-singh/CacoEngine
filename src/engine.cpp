#include "engine.hpp"
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <iostream>

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

    void Engine::Run()
    {
        this->IsRunning = true;
        this->Window = SDL_CreateWindow(this->Title.data(), 0, 2500, this->Resolution.X, this->Resolution.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        //this->WindowSurface = Surface(this->Window);
        this->EngineRenderer = Renderer(this->Window);

        SDL_Renderer* renderer = SDL_CreateRenderer(this->Window, -1, SDL_RENDERER_ACCELERATED);

        while (this->IsRunning)
        {
            while (SDL_PollEvent(&this->Event))
                switch (this->Event.type)
                {
                    case SDL_QUIT:
                        this->IsRunning = false;

                        break;
                    case SDL_KEYDOWN:
                        //this->OnKeyPress(this->Event.key);

                        if (this->Event.key.keysym.sym == SDLK_DOWN)
                        {
                            this->CursorPosition.Y += 100;
                        }

                        if (this->Event.key.keysym.sym == SDLK_RIGHT)
                        {
                            this->CursorPosition.X += 100;
                        }


                        break;
                    case SDL_MOUSEMOTION:
                        // SDL_GetMouseState(&this->CursorPosition.X, &this->CursorPosition.Y);
                        // this->WindowSurface.SetPixel(Vertex2D(this->CursorPosition, RGB(255, 0, 0)));
                        // std::cout << "(" << this->CursorPosition.X << ", " << this->CursorPosition.Y << ")\n";

                        break;
                }


            this->EngineRenderer.Clear();
            this->EngineRenderer.SetColor(Colors[(int)Color::White]);

            //SDL_RenderDrawLine(renderer, 5, 5, 100, 120);

            Triangle triangle = Triangle(Vertex2D(Vector2D(400, 150), RGBA(255, 0, 0, 255)),
                                      Vertex2D(Vector2D(200, 450), RGBA(255, 0, 0, 255)),
                                      Vertex2D(Vector2D(600, 450), RGBA(255, 0, 0, 255)), RGBA());


            SDL_RenderGeometry(this->EngineRenderer.Instance, nullptr, triangle.GetBuffer().data(), triangle.Vertices.size(), nullptr, 0);
            SDL_RenderPresent(this->EngineRenderer.Instance);

            SDL_Delay(0);
        }
    }


    Engine::Engine(std::string_view title, Vector2D resolution, bool initialize)
        : Title(title), Resolution(resolution), IsRunning(false)
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
        SDL_Quit();

        std::cout << "SDL Aborted.";
    }
}
