#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <vector>
#include <string_view>
#include "vertex.hpp"
#include "objects.hpp"
#include "surface.hpp"
#include "renderer.hpp"
#include "rigidobject.hpp"

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

            std::vector<Object> Objects;

            std::vector<RigidObject2D> RigidObjects;

            bool HasExtension(Extension);

            void UpdatePhysics();

            void Render(SDL_Renderer*, std::vector<Object>&); 
            void Render(SDL_Renderer*, std::vector<RigidObject2D>&); 

    public:
            std::string_view Title;

            Vector2D Resolution;

            bool IsRunning;

            Vector2D CursorPosition;

            SDL_Window* Window;

            double ElapsedTime;

            void Run();

            void AddExtension(Extension);
            void Initialize();

            /** Event handlers **/
            virtual void OnKeyPress(SDL_KeyboardEvent&) = 0;
            virtual void OnMouseClick(SDL_MouseButtonEvent&) = 0;
            virtual void OnMouseScroll(SDL_MouseWheelEvent&) = 0;

            virtual void OnInitialize() = 0;
            virtual void OnUpdate(double) = 0;

            Object& AddObject(Object);
            RigidObject2D& AddObject(RigidObject2D);

            Object& CreateMesh(std::vector<Vector2D>);
            
            Engine(std::string_view = "CacoEngine App", Vector2D = Vector2D(800, 600), bool = true);
            ~Engine();
    };
}
#endif // ENGINE_H_
