#pragma once

#include <RE/Skyrim.h>

namespace CWF {
    class InputHandler : public RE::BSTEventSink<RE::InputEvent*> {
    public:
        static InputHandler& GetSingleton();
        void Register();
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

    private:
        InputHandler() = default;
    };
}
