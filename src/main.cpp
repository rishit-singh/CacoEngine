#include "engine.hpp"
#include "tools.hpp"
#include <iostream>
#include "random.hpp"

using namespace CacoEngine;

class Application : public CacoEngine::Engine
{
public:
        void OnInitialize() override
        {
        }

        void OnUpdate(int frame) override
        {
                Mesh mesh;
                mesh.AddTriangle(
                        Triangle(Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y), Colors[(int)Color::White], Vector2D(0, 0)),
                                    Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y + 50), Colors[(int)Color::White], Vector2D(0, 1)),
                                Vertex2D(Vector2D(this->CursorPosition.X + 50, this->CursorPosition.Y + 50), Colors[(int)Color::White], Vector2D(1, 1)), RGBA()));

                mesh.AddTriangle(
                        Triangle(Vertex2D(Vector2D(this->CursorPosition.X + 50, this->CursorPosition.Y + 50), Colors[(int)Color::White], Vector2D(1, 1)),
                                Vertex2D(Vector2D(this->CursorPosition.X + 50, this->CursorPosition.Y), Colors[(int)Color::White], Vector2D(1, 0)),
                                Vertex2D(Vector2D(this->CursorPosition.X, this->CursorPosition.Y), Colors[(int)Color::White], Vector2D(0, 0)), RGBA()));


                this->Objects.push_back(mesh);
                std::cout << "(" << this->CursorPosition.X << ", " << this->CursorPosition.Y << ")\n";
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

