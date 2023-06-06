#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <vector>
#include <string_view>
#include <memory>
#include <unordered_map>
#include "vertex.hpp"
#include "objects.hpp"
#include "surface.hpp"
#include "renderer.hpp"
#include "rigidobject.hpp"
#include "key.hpp"

namespace CacoEngine
{
    enum class Extension
    {
        Video = SDL_INIT_VIDEO,
        Audio = SDL_INIT_AUDIO
    };

    class Engine
    {
    protected:
            // Contains all the Extensions
            std::vector<Extension> Extensions; 

            int ExtensionBits;

            SDL_Event Event;

            Renderer EngineRenderer;

            Surface WindowSurface;

            uint8_t* KeyStates;

            std::unordered_map<SDL_Keycode, Key> KeyMap;

            std::vector<std::shared_ptr<Object>> Objects;

            std::vector<std::shared_ptr<RigidObject2D>> RigidObjects;

            bool HasExtension(Extension);

            void UpdatePhysics();

            void MapKey(SDL_KeyboardEvent&);

            void ResetKeyState();

            void Render(SDL_Renderer*, std::vector<std::shared_ptr<Object>>&);
            void Render(SDL_Renderer*, std::vector<std::shared_ptr<RigidObject2D>>&);

    public:
            std::string_view Title;

            Vector2Df Resolution;

            bool IsRunning;

            Vector2D CursorPosition;

            SDL_Window* Window;

            double DeltaTime;

            void Run();

            void AddExtension(Extension);
            void Initialize();

            Key GetKeyState(SDL_Scancode);

            /** Event handlers **/
            virtual void OnKeyPress(SDL_KeyboardEvent&) = 0;
            virtual void OnMouseClick(SDL_MouseButtonEvent&) = 0;
            virtual void OnMouseScroll(SDL_MouseWheelEvent&) = 0;

            virtual void OnInitialize() = 0;
            virtual void OnUpdate(double) = 0;

            Object& AddObject(std::shared_ptr<Object>);
            RigidObject2D& AddObject(std::shared_ptr<RigidObject2D>);

            Object& CreateMesh(std::vector<Vector2Df>);
            
            Engine(std::string_view = "CacoEngine App", Vector2Df = Vector2Df(800, 600), bool = true);
            ~Engine();
    };
}

#endif // ENGINE_H_
