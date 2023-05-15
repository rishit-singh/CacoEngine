#include <SDL_keycode.h>
#include <algorithm>
#include <unordered_map>
#include <iostream>

#include "engine.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "sprite.hpp"
#include "random.hpp"

class Application : public CacoEngine::Engine
{
public:
        int TintIndex;

        CacoEngine::RGBA TintColor;

        int SelectedIndex;

        std::unordered_map<std::string_view, CacoEngine::Texture> TextureCache;

        int Force;

        void OnInitialize() override
        {
            this->TintIndex = 0;
            this->TintColor = CacoEngine::Colors[this->TintIndex];
            this->SelectedIndex = 0;

            this->Force = 800;

            this->TextureCache["cacodemon"] = CacoEngine::TextureManager::CreateTexture("cacodemon.png", this->EngineRenderer);
            this->TextureCache["cacodemon_left"] = CacoEngine::TextureManager::CreateTexture("cacodemon_left.png", this->EngineRenderer);
            this->TextureCache["cacodemon_right"] = CacoEngine::TextureManager::CreateTexture("cacodemon_right.png", this->EngineRenderer);

            this->Objects.push_back(CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2D(200, 200), CacoEngine::Vector2D(100, 100)));
        }

        void OnUpdate(int frame) override
        {
        }

        void OnMouseClick(SDL_MouseButtonEvent& event)
        {
        }

        void OnKeyPress(SDL_KeyboardEvent& event) override
        {
            if (event.keysym.sym == SDLK_RIGHT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_right"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2D(10, 0));
            }

            if (event.keysym.sym == SDLK_LEFT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_left"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2D(-10, 0));
            }

            if (event.keysym.sym == SDLK_DOWN)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2D(0, 10));
            }

            if (event.keysym.sym == SDLK_UP)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(CacoEngine::Vector2D(0, -10));
            }

            if (event.keysym.sym == SDLK_s)
            {
                CacoEngine::Sprite sprite = CacoEngine::Sprite(this->TextureCache["cacodemon"], CacoEngine::Vector2D(200, 200), CacoEngine::Vector2D(100, 100));

                // CacoEngine::Rectangle sprite = CacoEngine::Rectangle(CacoEngine::Vector2D(200, 200), CacoEngine::Vector2D(100)); 


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

        Application(std::string_view appName, CacoEngine::Vector2D windowDimensions) : Engine(appName, windowDimensions)
        {
        }

        ~Application()
        {
        }
};


int main(int argc, char** argv)
{
    Application app = Application(argv[1], CacoEngine::Vector2D(atoi(argv[2]), atoi(argv[3])));

    app.Run();

    return 0;
}
