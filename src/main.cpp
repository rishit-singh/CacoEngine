#include "engine.hpp"
#include "tools.hpp"
#include <iostream>
#include "random.hpp"

int main(int argc, char **argv)
{
    CacoEngine::Engine engine = CacoEngine::Engine(argv[1], CacoEngine::Vector2D(atoi(argv[2]), atoi(argv[3])));

    engine.Run();
    // CacoEngine::Util::Random random = CacoEngine::Util::Random();

    // for (int x = 0; x < atoi(argv[1]); x++)
    //     std::cout << random.Next(0, 100) << ", ";

    // std::cout << std::endl;

    return 0;
}
