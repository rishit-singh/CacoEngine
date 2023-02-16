#include "engine.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include <SDL_keycode.h>
#include <algorithm>
#include <iostream>
#include "sprite.hpp"
#include "random.hpp"

using namespace CacoEngine;

class Application : public CacoEngine::Engine
{
public:
        int TintIndex;

        RGBA TintColor;


        void OnInitialize() override
        {
            this->TintIndex = 0;
            this->TintColor = Colors[this->TintIndex];

            Texture texture = TextureManager::CreateTexture("cacodemon.png", this->EngineRenderer);


            //this->Objects.push_back(Sprite(texture, Vector2D(100, 100), Vector2D(100, 100)));
            this->Objects.push_back(Sprite(texture, Vector2D(200, 200), Vector2D(100, 100)));
        }

        void OnUpdate(int frame) override
        {
        }

        void OnMouseClick(SDL_MouseButtonEvent& event)
        {
            // Rectangle rect = Rectangle(Vector2D(200, 200), this->CursorPosition, this->TintColor);
            // // mesh.AddTriangle(
            // //         Triangle(Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y), this->TintColor, Vector2D(0, 0)),
            // //                     Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y + 100), this->TintColor, Vector2D(0, 1)),
            // //                 Vertex2D(Vector2D(this->CursorPosition.X + 100, this->CursorPosition.Y + 100), this->TintColor, Vector2D(1, 1)), RGBA()));

            // // mesh.AddTriangle(
            // //         Triangle(Vertex2D(Vector2D(this->CursorPosition.X + 100, this->CursorPosition.Y + 100), this->TintColor, Vector2D(1, 1)),
            // //                 Vertex2D(Vector2D(this->CursorPosition.X + 100, this->CursorPosition.Y), this->TintColor, Vector2D(1, 0)),
            // //                 Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y), this->TintColor, Vector2D(0, 0)), RGBA()));


            // this->Objects.push_back(rect);
            // std::cout << "(" << this->CursorPosition.X << ", " << this->CursorPosition.Y << ")" << this->Frame << std::endl;
        }

        void OnKeyPress(SDL_KeyboardEvent& event) override
        {
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
    // CacoEngine::Util::Random random = CacoEngine::Util::Random();

    // for (int x = 0; x < atoi(argv[1]); x++)
    //     std::cout << random.Next(0, 100) << ", ";

    // std::cout << std::endl;

    return 0;
}

