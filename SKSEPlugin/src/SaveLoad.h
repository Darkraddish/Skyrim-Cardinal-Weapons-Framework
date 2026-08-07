#pragma once

#include <SKSE/SKSE.h>

namespace CWF {
    class SaveLoad {
    public:
        static void Register(SKSE::SerializationInterface* a_intfc);
        static void SaveCallback(SKSE::SerializationInterface* a_intfc);
        static void LoadCallback(SKSE::SerializationInterface* a_intfc);
        static void RevertCallback(SKSE::SerializationInterface* a_intfc);
    };
}
