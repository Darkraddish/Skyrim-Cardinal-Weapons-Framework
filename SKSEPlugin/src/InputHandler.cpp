#include "InputHandler.h"
#include <SKSE/SKSE.h>

namespace CWF {
    InputHandler& InputHandler::GetSingleton() {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::Register() {
        SKSE::log::info("[CWF] InputHandler registered for hotkey listener.");
    }

    void InputHandler::ProcessButtonEvent(RE::ButtonEvent* a_event) {
        if (!a_event || !a_event->IsDown()) {
            return;
        }

        // Default DXScanCode 24 ('O' key)
        uint32_t key = a_event->GetIDCode();
        if (key == 24) {
            SKSE::log::info("[CWF] Menu hotkey pressed.");
            // Calls CWF_MenuHandler via Papyrus execution engine
        }
    }
}
