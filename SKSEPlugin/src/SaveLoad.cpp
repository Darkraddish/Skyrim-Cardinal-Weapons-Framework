#include "SaveLoad.h"

namespace CWF {
    constexpr uint32_t kSerializationVersion = 1;
    constexpr uint32_t kSerializationUniqueID = 'CWFS';

    void SaveLoad::Register(const SKSE::SerializationInterface* a_intfc) {
        if (!a_intfc) return;
        auto intfc = const_cast<SKSE::SerializationInterface*>(a_intfc);
        intfc->SetUniqueID(kSerializationUniqueID);
        intfc->SetSaveCallback(SaveCallback);
        intfc->SetLoadCallback(LoadCallback);
        intfc->SetRevertCallback(RevertCallback);
        SKSE::log::info("[CWF] Co-save persistence interface registered.");
    }

    void SaveLoad::SaveCallback(SKSE::SerializationInterface* a_intfc) {
        SKSE::log::info("[CWF] Saving progression state to co-save...");
    }

    void SaveLoad::LoadCallback(SKSE::SerializationInterface* a_intfc) {
        SKSE::log::info("[CWF] Loading progression state from co-save...");
    }

    void SaveLoad::RevertCallback(SKSE::SerializationInterface* a_intfc) {
        SKSE::log::info("[CWF] Reverting progression state...");
    }
}
