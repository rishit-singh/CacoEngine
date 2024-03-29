#include "key.hpp"

CacoEngine::Key::Key()
{
}

CacoEngine::Key::Key(SDL_Scancode symbol, bool held)
    : Symbol(symbol), IsHeld(held) {}


CacoEngine::Key::Key(const Key &key)
{
    *this = key;
}

CacoEngine::Key& CacoEngine::Key::operator=(const Key &key)
{
    this->Symbol = key.Symbol;
    this->IsHeld = key.IsHeld;

    return *this;
}
