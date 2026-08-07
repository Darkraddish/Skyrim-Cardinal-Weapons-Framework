#include "InputHandler.h"
#include <SKSE/SKSE.h>

namespace CWF {
    InputHandler& InputHandler::GetSingleton() {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::Register() {
        auto input = RE::BSInputDeviceManager::GetSingleton();
        if (input) {
            input->AddEventSink(this);
            SKSE::log::info("[CWF] InputHandler successfully registered with BSInputDeviceManager!");
        } else {
            SKSE::log::error("[CWF] Failed to get BSInputDeviceManager.");
        }
    }

    RE::BSEventNotifyControl InputHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (auto event = *a_event; event; event = event->next) {
            if (auto button = event->AsButtonEvent()) {
                if (button->IsDown() && button->GetIDCode() == 24) { // DXScanCode 24 ('O' key)
                    SKSE::log::info("[CWF] Hotkey 'O' pressed! Triggering menu notification...");
                    RE::DebugNotification("Cardinal Weapon Framework: Hotkey 'O' Pressed!");
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}
