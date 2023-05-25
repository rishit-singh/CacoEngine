#include <SDL_events.h>
#include <SDL_keyboard.h>

namespace CacoEngine
{
    struct Key
    {
        SDL_Scancode Symbol;

        bool IsHeld;

        Key(SDL_Keycode = 0, bool = false);

        Key(const Key&);
        Key& operator =(const Key&);
    };
}
