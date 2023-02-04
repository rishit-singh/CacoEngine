#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL.h>
#include <vector>
#include <string_view>
#include "objects.hpp"

namespace CacoEngine
{
    enum class Extension
    {
        Video = SDL_INIT_VIDEO,
        Audio = SDL_INIT_VIDEO
    };

    class Engine
    {
    protected:
            std::vector<Extension> Extensions;

            int ExtensionBits;
    public:
            std::string_view Title;

            Vector2D Resolution;

            void AddExtension(Extension);
            void Run();
            void Initialize();

            Engine(std::string_view = "CacoEngine App", Vector2D = Vector2D(800, 600), bool = true);
            ~Engine();
    };
}
#endif // ENGINE_H_
