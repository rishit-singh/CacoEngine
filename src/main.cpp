#include "engine.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    CacoEngine::Engine engine = CacoEngine::Engine(argv[1], CacoEngine::Vector2D(atoi(argv[2]), atoi(argv[3])));

    engine.Run();


    return 0;
}
