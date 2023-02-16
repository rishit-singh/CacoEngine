#include <SDL_keycode.h>
#include <algorithm>
#include <unordered_map>
#include <iostream>

#include "engine.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "sprite.hpp"
#include "random.hpp"

using namespace CacoEngine;

class Application : public CacoEngine::Engine
{
public:
        int TintIndex;

        RGBA TintColor;

        int SelectedIndex;

        std::unordered_map<std::string_view, Texture> TextureCache;

        int Force;

        void OnInitialize() override
        {
            this->TintIndex = 0;
            this->TintColor = Colors[this->TintIndex];
            this->SelectedIndex = 0;

            this->Force = 800;

            this->TextureCache["cacodemon"] = TextureManager::CreateTexture("cacodemon.png", this->EngineRenderer);
            this->TextureCache["cacodemon_left"] = TextureManager::CreateTexture("cacodemon_left.png", this->EngineRenderer);
            this->TextureCache["cacodemon_right"] = TextureManager::CreateTexture("cacodemon_right.png", this->EngineRenderer);


            this->Objects.push_back(Sprite(this->TextureCache["cacodemon"], Vector2D(200, 200), Vector2D(100, 100)));
        }

        void OnUpdate(int frame) override
        {
            for (; this->Force >= 0; this->Force--)
                this->Objects[this->SelectedIndex].Translate(Vector2D(1, 0));
        }

        void OnMouseClick(SDL_MouseButtonEvent& event)
        {
        }

        void OnKeyPress(SDL_KeyboardEvent& event) override
        {
            if (event.keysym.sym == SDLK_RIGHT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_right"];
                this->Objects[this->SelectedIndex].Translate(Vector2D(10, 0));
            }

            if (event.keysym.sym == SDLK_LEFT)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon_left"];
                this->Objects[this->SelectedIndex].Translate(Vector2D(-10, 0));
            }

            if (event.keysym.sym == SDLK_DOWN)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(Vector2D(0, 10));
            }

            if (event.keysym.sym == SDLK_UP)
            {
                this->Objects[this->SelectedIndex].mTexture = this->TextureCache["cacodemon"];
                this->Objects[this->SelectedIndex].Translate(Vector2D(0, -10));
            }
            if (event.keysym.sym == SDLK_s)
            {
                this->Objects.push_back(Sprite(this->TextureCache["cacodemon"], Vector2D(200, 200), Vector2D(100, 100)));
                this->SelectedIndex++;
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

            this->TintColor = Colors[this->TintIndex];

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

