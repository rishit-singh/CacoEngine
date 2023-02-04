#include "engine.hpp"
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
            std::cout << "Failed to initialize SDL\n";
            return;
        }
        std::cout << "SDL Initialized.\n";
    }

    void Engine::Run()
    {
    }


    Engine::Engine(std::string_view title, Vector2D resolution, bool initialize)
        : Title(title), Resolution(resolution)
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
        SDL_Quit();
        std::cout << "SDL Aborted.\n";
    }
}
