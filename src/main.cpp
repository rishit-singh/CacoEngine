#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <memory>
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
#include "collider.hpp"

class Application : public CacoEngine::Engine
{
public:
        int TintIndex;

        CacoEngine::RGBA TintColor;

        int SelectedIndex;

        std::unordered_map<std::string_view, CacoEngine::Texture> TextureCache;

        CacoEngine::RigidBody2D Metrics;

        std::shared_ptr<CacoEngine::RigidCircle> rgCircle { std::make_unique<CacoEngine::RigidCircle>(CacoEngine::RigidCircle(CacoEngine::Vector2Df(200, 200), 50)) };

        std::vector<CacoEngine::RigidCircle> RigidCircles;

        CacoEngine::Object& GetObject(size_t index)
        {
            size_t size, rigidIndex;

            if (index >= (size = this->Objects.size()) && (rigidIndex = (index - size)) >= this->RigidObjects.size())
              return *this->RigidObjects[this->RigidObjects.size() - 1];

            else if (index >= size)
                return *this->RigidObjects[rigidIndex];

            return *this->Objects[index];
        }

        void OnInitialize() override
        {
            this->TintIndex = 0;
            this->TintColor = CacoEngine::Colors[this->TintIndex];
            this->SelectedIndex = 0;

            this->TextureCache["cacodemon"] = CacoEngine::TextureManager::CreateTexture("cacodemon.png", this->EngineRenderer);
            this->TextureCache["cacodemon_left"] = CacoEngine::TextureManager::CreateTexture("cacodemon_left.png", this->EngineRenderer);
            this->TextureCache["cacodemon_right"] = CacoEngine::TextureManager::CreateTexture("cacodemon_right.png", this->EngineRenderer);

            this->AddObject(std::make_shared<CacoEngine::Object>(CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(600, 600))));

            CacoEngine::Box2D box = CacoEngine::Box2D(CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

            box.RigidBody = CacoEngine::RigidBody2D(CacoEngine::Vector2Df(0, 50), CacoEngine::Vector2Df(0, 50));

            this->AddObject(std::shared_ptr<CacoEngine::RigidObject2D>(std::make_unique<CacoEngine::RigidCircle>(*this->rgCircle)));

            this->RigidCircles.push_back(*this->rgCircle);

//            CacoEngine::RigidSprite rgSprite = CacoEngine::RigidSprite(this->TextureCache["cacodemon"],CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

            // this->AddObject(box);
            //
            // this->AddObject(rgSprite);
            //
           // this->AddObject(std::shared_ptr<CacoEngine::RigidObject2D>(std::make_unique<CacoEngine::RigidCircle>(CacoEngine::Vector2Df(200, 200), 50)));
            // this->RigidObjects[this->RigidObjects.size() - 1].RigidBody = CacoEngine::RigidBody2D(CacoEngine::Vector2D());
            // this->AddObject(CacoEngine::Box2D(CacoEngine::Vector2Df(100, 100),
            //                                   CacoEngine::Vector2Df(), CacoEngine::Colors[(int)CacoEngine::Color::White]));
            // this->Metrics = CacoEngine::RigidBody2D(CacoEngine::Vector2Df(0, 5), CacoEngine::Vector2Df(0, 50));

        }

        void OnUpdate(double frame) override
        {
            this->EngineRenderer.SetColor(CacoEngine::Colors[(int)CacoEngine::Color::White]);
            // DrawCircle(this->EngineRenderer.GetInstance( ), 100, 100, 100);

            CacoEngine::RigidCircleCollider collider = CacoEngine::RigidCircleCollider(this->RigidCircles, this->RigidObjects);

            CacoEngine::RigidObject2D& object = *this->RigidObjects[this->RigidObjects.size() - 1];

            std::cout << "Cursor: (" << this->CursorPosition.X << ", " << this->CursorPosition.Y << ")\n";

            // for (int x = 0; x < this->RigidCircles.size(); x++)
            //     if (collider.CollidesWith(RigidCircles[x]))
            //     {
            //         object.SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Red]);

            //         std::cout << "Object collides.\n";
            //     }
            //     else
            //         object.SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Green]);

            // std::cout << "ObjectCount: " << this->RigidObjects.size() + this->Objects.size() << '\


            auto objects = collider.GetObjects();

            collider.Handle();

//.            for (int x = 0; x < objects.size(); x++)
   //             objects[x]->SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::White]);


            if (object.Position.X > 1000)
            {
                std::cout << "Beyond threshold" << std::endl;
                object.RigidBody.AddForce(CacoEngine::Vector2Df(-200, 0));
            }
        }

        void OnMouseClick(SDL_MouseButtonEvent& event) override
        {
            // this->rgCircle = std::make_shared<CacoEngine::RigidCircle>(*this->RigidObjects[this->RigidObjects.size() - 1]);
            this->RigidCircles.push_back(CacoEngine::RigidCircle(CacoEngine::Vector2Df(this->CursorPosition.X, this->CursorPosition.Y), 50));

            this->AddObject(std::shared_ptr<CacoEngine::RigidObject2D>(std::make_unique<CacoEngine::RigidCircle>(this->RigidCircles[this->RigidCircles.size() - 1])));

            this->SelectedIndex++;
        }

        void OnKeyPress(SDL_KeyboardEvent& event) override
        {
            if (event.keysym.sym == SDLK_RIGHT)
            {
                this->GetObject(this->SelectedIndex).mTexture = this->TextureCache["cacodemon_right"];

                // this->Metrics.Velocity += (this->Metrics.Acceleration * this->DeltaTime);

                this->GetObject(this->SelectedIndex).Translate(CacoEngine::Vector2Df(this->Metrics.Velocity.Y * this->DeltaTime, 0));
            }

            if (event.keysym.sym == SDLK_LEFT)
            {
                // this->GetObject(this->SelectedIndex).mTexture = this->TextureCache["cacodemon_left"];
                // this->GetObject(this->SelectedIndex).Translate(CacoEngine::Vector2Df(-200 * this->DeltaTime, 0));
                this->RigidObjects[this->RigidObjects.size() - 1]->RigidBody.AddForce(CacoEngine::Vector2Df(-100, 0));
            }

            if (event.keysym.sym == SDLK_DOWN)
            {
                // this->GetObject(this->SelectedIndex).mTexture = this->TextureCache["cacodemon"];
                // this->GetObject(this->SelectedIndex).Translate(CacoEngine::Vector2Df(0, 200 * this->DeltaTime));
                this->RigidObjects[this->RigidObjects.size() - 1]->RigidBody.AddForce(CacoEngine::Vector2Df(0, 100));
            }

            if (event.keysym.sym == SDLK_UP)
            {
                // this->GetObject(this->SelectedIndex).mTexture = this->TextureCache["cacodemon"];
                // this->GetObject(this->SelectedIndex).Translate(CacoEngine::Vector2Df(0, -200 * this->DeltaTime));
                this->RigidObjects[this->RigidObjects.size() - 1]->RigidBody.AddForce(CacoEngine::Vector2Df(100, 0));
            }

            if (event.keysym.sym == SDLK_SPACE)
            {
                this->GetObject(this->SelectedIndex).Translate(CacoEngine::Vector2Df(0, 700));
            }

            if (event.keysym.sym == SDLK_s)
            {

                CacoEngine::Sprite sprite = CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100, 100));

                // CacoEngine::Rectangle sprite = CacoEngine::Rectangle(CacoEngine::Vector2Df(200, 200), CacoEngine::Vector2Df(100));


                // sprite.SetFillColor(CacoEngine::Colors[(int)CacoEngine::Color::Red]);

                // sprite.mTexture = this->TextureCache["cacodemon"];
                // sprite.FillMode = CacoEngine::RasterizeMode::Texture;

                // this->Objects.push_back(std::make_shared<CacoEngine::Object>(sprite));
                this->AddObject(std::shared_ptr<CacoEngine::RigidObject2D>(std::make_unique<CacoEngine::RigidCircle>(CacoEngine::Vector2Df(0, 0), 50)));
                this->SelectedIndex++;
            }

            if (event.keysym.sym == SDLK_w)
            {
                CacoEngine::Object& object = this->GetObject(this->SelectedIndex); //this->Objects[this->SelectedIndex];
                if (object.FillMode == CacoEngine::RasterizeMode::WireFrame)
                    object.FillMode = CacoEngine::RasterizeMode::Texture;
                else
                   object.FillMode = CacoEngine::RasterizeMode::WireFrame; 
            }
            
            if (event.keysym.sym == SDLK_c)
            {
                if (++this->SelectedIndex >= (this->Objects.size() + this->RigidObjects.size()))
                    this->SelectedIndex = 0;
            }

            if (event.keysym.sym == SDLK_d)
                if (--this->SelectedIndex < 0)
                    this->SelectedIndex = (this->Objects.size() +  this->RigidObjects.size()) - 1;
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
