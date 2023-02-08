#include "objects.hpp"

CacoEngine::Vector2D::Vector2D(int x, int y) :  X(x), Y(y)
{
}

CacoEngine::Vertex2D::Vertex2D(Vector2D position, RGB color) : Position(position), Color(color)
{
}

CacoEngine::RGB::RGB(int r, int g, int b) : R(r), G(g), B(b) {

}

CacoEngine::RGB CacoEngine::Colors[3] = {
    RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255)
};
