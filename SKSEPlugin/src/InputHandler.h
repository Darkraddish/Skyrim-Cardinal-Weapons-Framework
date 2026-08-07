#pragma once

#include <RE/Skyrim.h>

namespace CWF {
    class InputHandler {
    public:
        static InputHandler& GetSingleton();
        void Register();
        void ProcessButtonEvent(RE::ButtonEvent* a_event);

    private:
        InputHandler() = default;
    };
}
