#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <iostream>

#include "engine.hpp"
#include "objects.hpp"
#include "rigidbody.hpp"
#include "rigidsprite.hpp"
#include "rigidobject.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "rigidsprite.hpp"
#include "random.hpp"
#include "box.hpp"
#include "vertex.hpp"



class Application : public CacoEngine::Engine
{
public:

        int TintIndex;

        CacoEngine::RGBA TintColor;

        int SelectedIndex;

        std::unordered_map<std::string_view, CacoEngine::Texture> TextureCache;

        CacoEngine::RigidBody2D Metrics;

        void OnInitialize() override
        {
            this->TintIndex = 0;
            this->TintColor = CacoEngine::Colors[this->TintIndex];
            this->SelectedIndex = 0;

            this->TextureCache["cacodemon"] = CacoEngine::TextureManager::CreateTexture("cacodemon.png", this->EngineRenderer);
            this->TextureCache["cacodemon_left"] = CacoEngine::TextureManager::CreateTexture("cacodemon_left.png", this->EngineRenderer);
            this->TextureCache["cacodemon_right"] = CacoEngine::TextureManager::CreateTexture("cacodemon_right.png", this->EngineRenderer);

            this->AddObject((CacoEngine::Object)CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(600, 600)));

            CacoEngine::Box2D box = CacoEngine::Box2D(CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

            box.RigidBody = CacoEngine::RigidBody2D(CacoEngine::Vector2Df(0, 50), CacoEngine::Vector2Df(0, 50));

            CacoEngine::RigidSprite rgSprite = CacoEngine::RigidSprite(this->TextureCache["cacodemon"],CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

            // this->AddObject(box);
            //
            // this->AddObject(rgSprite);
            //
            this->AddObject(CacoEngine::RigidCircle(CacoEngine::Vector2Df(200, 200), 50));

            // this->AddObject(CacoEngine::Box2D(CacoEngine::Vector2Df(100, 100), CacoEngine::Vector2Df(), CacoEngine::Colors[(int)CacoEngine::Color::White]));


            // this->Metrics = CacoEngine::RigidBody2D(CacoEngine::Vector2Df(0, 5), CacoEngine::Vector2Df(0, 50));
        }

        void OnUpdate(double frame) override
        {
            this->EngineRenderer.SetColor(CacoEngine::Colors[(int)CacoEngine::Color::White]);
            // DrawCircle(this->EngineRenderer.GetInstance(), 100, 100, 100);
        }

        void OnMouseClick(SDL_MouseButtonEvent& event) override
        {
        }

        void OnKeyPress(SDL_KeyboardEvent& event) override
        {
            if (event.keysym.sym == SDLK_RIGHT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_right"];

                this->Metrics.Velocity += (this->Metrics.Acceleration * this->DeltaTime);

                std::cout << "Velocity: " << this->Metrics.Velocity.Y << std::endl;

                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2Df(this->Metrics.Velocity.Y * this->DeltaTime, 0));
            }

            if (event.keysym.sym == SDLK_LEFT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_left"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2Df(-200 * this->DeltaTime, 0));
            }

            if (event.keysym.sym == SDLK_DOWN)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2Df(0, 200 * this->DeltaTime));
            }

            if (event.keysym.sym == SDLK_UP)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2Df(0, -200 * this->DeltaTime));
            }

            if (event.keysym.sym == SDLK_s)
            {

                CacoEngine::Sprite sprite = CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

                // CacoEngine::Rectangle sprite = CacoEngine::Rectangle(CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100));


                // sprite.SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Red]);

                // sprite.mTexture = this->TextureCache["cacodemon"];
                // sprite.FillMode = CacoEngine::RasterizeMode::Texture;

                this->Objects.push_back(sprite);
                this->SelectedIndex++;
            }

            if (event.keysym.sym == SDLK_w)
            {
                CacoEngine::Object& object = this->Objects[this->SelectedIndex];
                if (object.FillMode == CacoEngine::RasterizeMode::WireFrame)
                    object.FillMode = CacoEngine::RasterizeMode::Texture;
                else
                   object.FillMode = CacoEngine::RasterizeMode::WireFrame; 
            }
            
            if (event.keysym.sym == SDLK_c)
                if (++this->SelectedIndex >= this->Objects.size())
                    this->SelectedIndex = 0;

            if (event.keysym.sym == SDLK_d)
                if (--this->SelectedIndex < 0)
                    this->SelectedIndex = this->Objects.size() - 1;
        }

        void OnMouseScroll(SDL_MouseWheelEvent &event) override
        {
            if (event.y > 0)
                this->TintIndex++;

            if (event.y < 0)
                this->TintIndex--;

            if (this->TintIndex < 0)
                this->TintIndex = 4;
            else if (this->TintIndex > 4)
                this->TintIndex = 0;

            this->TintColor = CacoEngine::Colors[this->TintIndex];
        }

        Application(std::string_view appName, CacoEngine::Vector2Df windowDimensions) : Engine(appName, windowDimensions)
        {
        }

        ~Application()
        {
        }
};


int main(int argc, char** argv)
{
    Application app = Application(argv[1], CacoEngine::Vector2Df(atoi(argv[2]), atoi(argv[3])));

    app.Run();

    return 0;
}
