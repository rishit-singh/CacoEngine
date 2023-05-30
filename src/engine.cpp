#include "engine.hpp"
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <iostream>
#include <vector>
#include "objects.hpp"
#include "rigidobject.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include <memory>

namespace CacoEngine
{
    void Engine::AddExtension(Extension extension)
    {
        if (!this->HasExtension(extension))
            this->Extensions.push_back(extension);
    }

    bool Engine::HasExtension(Extension extension)
    {
        for (int x = 0; x < this->Extensions.size(); x++)
            if (this->Extensions[x] == extension)
                return true;

        return false;
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

    void Engine::MapKey(SDL_KeyboardEvent& event)
    {
        if (this->KeyMap.contains(event.keysym.sym))
            this->KeyMap[event.keysym.sym].IsHeld = true;
        else
            this->KeyMap[event.keysym.sym] = Key(event.keysym.scancode, true);
    }

    void Engine::ResetKeyState()
    {
        for (auto x = this->KeyMap.begin(); x != this->KeyMap.end(); x++)
            x->second.IsHeld = false;
    }



    Object& Engine::AddObject(std::unique_ptr<Object> object)
    {
        return *this->Objects.emplace_back(std::move(object));
    }
    
    RigidObject2D& Engine::AddObject(std::unique_ptr<RigidObject2D> object)
    {
        return *this->RigidObjects.emplace_back(std::move(object));
    }

    void Engine::OnKeyPress(SDL_KeyboardEvent& event)
    {
        this->MapKey(event);

        if (event.keysym.sym == SDLK_ESCAPE)
            this->IsRunning = false;
    }

    Key Engine::GetKeyState(SDL_Scancode keyCode)
    {
        return Key(keyCode, (this->KeyStates[keyCode] != 0));
    }

    void Engine::UpdatePhysics()
    {   
        double gravity = 9.8f;

        for (int x = 0; x < this->RigidObjects.size(); x++)
        {
            RigidObject2D& object = *this->RigidObjects[x];
            double dT = this->DeltaTime;

            object.RigidBody.Acceleration *= dT;

            std::cout << "Acceleration: X:" << object.RigidBody.Acceleration.X
                << " Y: " << object.RigidBody.Acceleration.Y << '\n';
            std::cout << "Force: X:" << object.RigidBody.Force.X
                << " Y: " << object.RigidBody.Force.Y << '\n';

            object.RigidBody.Velocity += (object.RigidBody.Acceleration);

            object.Translate(Vector2Df(object.RigidBody.Velocity.X * dT, object.RigidBody.Velocity.Y * dT));

            if (object.Position.Y > 800)
                object.Translate(Vector2Df(0, -(object.Position.Y - 800)));

            object.RigidBody.Velocity = Vector2Df(0, 0);

            if ((object.RigidBody.Force.X - 1) < 0 || (object.RigidBody.Force.Y - 1) < 0)
            {
                object.RigidBody.Force.X = 0;
                object.RigidBody.Force.Y = 0;

                std::cout << "Force is 0. " << std::endl;
            }
            else
            {
                object.RigidBody.Force -= 1;
                std::cout << "Decreasing force" << std::endl;
            }

            object.RigidBody.UpdateAcceleration();

            object.RigidBody.LastUpdate = this->DeltaTime;
        }
    }

    void Engine::Render(SDL_Renderer* renderer, std::vector<std::unique_ptr<Object>>& objects)
    {

        for (int x = 0; x < objects.size(); x++)
        {
            Object& object = *objects[x];

            this->EngineRenderer.SetColor(object.FillColor);

            if (object.FillMode == RasterizeMode::WireFrame)
                SDL_RenderDrawLinesF(renderer = this->EngineRenderer.GetInstance(), object.ObjectMesh.GetPoints().data(), object.ObjectMesh.Vertices.size());

            else if (object.FillMode == RasterizeMode::Points)
                SDL_RenderDrawPointsF(renderer = this->EngineRenderer.GetInstance(), object.ObjectMesh.GetPoints().data(), object.ObjectMesh.Vertices.size());

            else
                SDL_RenderGeometry((renderer = this->EngineRenderer.GetInstance()),
                                    (object.FillMode == RasterizeMode::Texture) ? object.mTexture.mTexture : nullptr,
                                    object.ObjectMesh.GetVertexBuffer().data(),
                                    object.ObjectMesh.Vertices.size(),
                                    nullptr, 0);

        }

        // SDL_RenderPresent(renderer);
        // SDL_Delay(0);
    }

    void Engine::Render(SDL_Renderer* renderer, std::vector<std::unique_ptr<RigidObject2D>>& objects)
    {
        for (int x = 0; x < objects.size(); x++)
        {
            RigidObject2D& object = *objects[x];

            // std::unique_ptr<RigidObject2D> ptr = objects[x];


            this->EngineRenderer.SetColor((object).FillColor);

            if (object.FillMode == RasterizeMode::WireFrame)
                SDL_RenderDrawLinesF(renderer = this->EngineRenderer.GetInstance(), object.ObjectMesh.GetPoints().data(), object.ObjectMesh.Vertices.size());

            else if (object.FillMode == RasterizeMode::Points)
                SDL_RenderDrawPointsF(renderer = this->EngineRenderer.GetInstance(), object.ObjectMesh.GetPoints().data(), object.ObjectMesh.Vertices.size());
            else
                SDL_RenderGeometry((renderer = this->EngineRenderer.GetInstance()),
                                    (object.FillMode == RasterizeMode::Texture) ? object.mTexture.mTexture : nullptr,
                                    object.ObjectMesh.GetVertexBuffer().data(),
                                    object.ObjectMesh.Vertices.size(),
                                    nullptr, 0);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(0);
    }


    void Engine::Run()
    {
        SDL_Renderer* renderer = nullptr;

        Object object;

        std::vector<SDL_Point*> vertexBuffer;

        this->IsRunning = true;
        this->Window = SDL_CreateWindow(this->Title.data(), 0, 2500, this->Resolution.X, this->Resolution.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        this->EngineRenderer = Renderer(this->Window);

        IMG_Init(IMG_INIT_PNG);

        this->OnInitialize();

        uint64_t prev = 0, current = 0;


        while (this->IsRunning)
        {
            SDL_GetMouseState(&this->CursorPosition.X, &this->CursorPosition.Y);

            this->ResetKeyState();

            prev = SDL_GetPerformanceCounter();
            while (SDL_PollEvent(&this->Event)){
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
                        this->OnMouseClick(this->Event.button);
                        break;

                    case SDL_MOUSEWHEEL:
                        this->OnMouseScroll(this->Event.wheel);
                        break;
                }


                SDL_PumpEvents();
                this->KeyStates = const_cast<uint8_t*>(SDL_GetKeyboardState(NULL));
            }

            this->EngineRenderer.Clear();
            this->EngineRenderer.SetColor(Colors[(int)Color::White]);


            this->Render(renderer, this->Objects);
            this->Render(renderer, this->RigidObjects);

            this->UpdatePhysics();

            current = SDL_GetPerformanceCounter();

            this->DeltaTime = (double)((current - prev) / (double)SDL_GetPerformanceFrequency());

            this->OnUpdate(this->DeltaTime);

            // SDL_PumpEvents(Vx);
        }
    }

    Engine::Engine(std::string_view title, Vector2Df resolution, bool initialize)
        : Objects(std::vector<std::unique_ptr<Object>>()), RigidObjects(std::vector<std::unique_ptr<RigidObject2D>>()), Title(title), Resolution(resolution), IsRunning(false), DeltaTime(0)
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
