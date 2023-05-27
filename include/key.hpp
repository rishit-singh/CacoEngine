#include <SDL_events.h>
#include <SDL_keyboard.h>

namespace CacoEngine
{
    struct Key
    {
        SDL_Scancode Symbol;

        bool IsHeld;


        Key();
        Key(SDL_Scancode, bool = false);

        Key(const Key&);
        Key& operator =(const Key&);
    };
}
