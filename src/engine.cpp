#include "engine.hpp"
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <iostream>
#include <vector>
#include "objects.hpp"
#include "rigidobject.hpp"
#include "texture.hpp"


namespace CacoEngine
{
    bool IsBufferEqual(std::vector<SDL_FPoint> left, std::vector<SDL_FPoint> right)
    {
        for (int x = 0; x < left.size(); x++)
            if (!(left[x].x == right[x].x && left[x].y == right[x].y))
                return false;

        return true;
    }

    std::vector<SDL_FPoint> PixelizeCircle( SDL_FPoint center, int radius )
    {
        std::vector<SDL_FPoint> points;


        const float diameter = (radius * 2);

        float x = (radius - 1);
        float y = 0;
        float tx = 1;
        float ty = 1;
        float error = (tx - diameter);

        while( x >= y )
        {
            // Each of the following renders an octant of the circle
            points.push_back( { center.x + x, center.y - y } );
            points.push_back( { center.x + x, center.y + y } );
            points.push_back( { center.x - x, center.y - y } );
            points.push_back( { center.x - x, center.y + y } );
            points.push_back( { center.x + y, center.y - x } );
            points.push_back( { center.x + y, center.y + x } );
            points.push_back( { center.x - y, center.y - x } );
            points.push_back( { center.x - y, center.y + x } );

            if( error <= 0 )
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if( error > 0 )
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }

        return points; // RVO FTW
    }


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

    Object& Engine::AddObject(Object object)
    {
        this->Objects.push_back(object);
        return this->Objects[this->Objects.size() - 1];
    }
    
    RigidObject2D& Engine::AddObject(RigidObject2D object)
    {
        this->RigidObjects.push_back(object);
        
        return this->RigidObjects[this->RigidObjects.size() - 1];
    }

    void Engine::OnKeyPress(SDL_KeyboardEvent& event)
    {
        if (event.keysym.sym == SDLK_ESCAPE)
            this->IsRunning = false;
    }

    void Engine::UpdatePhysics()
    {   
        int gravity = 1000;

        for (int x = 0; x < this->RigidObjects.size(); x++)
        {
            RigidObject2D& object = this->RigidObjects[x];
            double dT = this->DeltaTime;

            object.RigidBody.Velocity +=  Vector2Df(0, dT * gravity);

            object.Translate(Vector2Df(object.RigidBody.Velocity.X * dT, object.RigidBody.Velocity.Y * dT));

            if (object.Position.Y > 400)
                object.Translate(Vector2Df(0, -(object.Position.Y - 400)));

            object.RigidBody.LastUpdate = this->DeltaTime;
        }
    }

    void Engine::Render(SDL_Renderer* renderer, std::vector<Object>& objects)
    {


        for (int x = 0; x < objects.size(); x++)
        {
            Object& object = objects[x];

            this->EngineRenderer.SetColor((object).FillColor);

            if (object.FillMode == RasterizeMode::WireFrame)
                SDL_RenderDrawLinesF(renderer = this->EngineRenderer.GetInstance(), object.ObjectMesh.GetPoints().data(), object.ObjectMesh.Vertices.size());

            else if (object.FillMode == RasterizeMode::Points)
                SDL_RenderDrawPointsF(renderer = this->EngineRenderer.GetInstance(),
                                      object.ObjectMesh.GetPoints().data(),
                                      object.ObjectMesh.Vertices.size());

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

    void Engine::Render(SDL_Renderer* renderer, std::vector<RigidObject2D>& objects)
    {
        for (int x = 0; x < objects.size(); x++)
        {
            RigidObject2D& object = objects[x];

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

            prev = SDL_GetPerformanceCounter();

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
                        this->OnMouseClick(this->Event.button);
                        break;

                    case SDL_MOUSEWHEEL:
                        this->OnMouseScroll(this->Event.wheel);
                        break;
                }


            this->EngineRenderer.Clear();
            this->EngineRenderer.SetColor(Colors[(int)Color::White]);


            this->Render(renderer, this->Objects);
            this->Render(renderer, this->RigidObjects);

            this->UpdatePhysics();


            current = SDL_GetPerformanceCounter();

            this->DeltaTime = (double)((current - prev) / (double)SDL_GetPerformanceFrequency());

            this->OnUpdate(this->DeltaTime);
        }
    }

    Engine::Engine(std::string_view title, Vector2Df resolution, bool initialize)
        : Objects(std::vector<Object>()), RigidObjects(std::vector<RigidObject2D>()), Title(title), Resolution(resolution), IsRunning(false), DeltaTime(0)
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
