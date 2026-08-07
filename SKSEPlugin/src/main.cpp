#include <SKSE/SKSE.h>
#include "DataLoader.h"
#include "InputHandler.h"
#include "SaveLoad.h"

namespace {
    void InitializeLogging() {
        auto path = SKSE::log::log_directory();
        if (!path) {
            return;
        }

        *path /= "CWFramework.log";
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));

        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
    }

    void MessagingHandler(SKSE::MessagingInterface::Message* a_msg) {
        if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            SKSE::log::info("[CWF] kDataLoaded received. Loading content packs...");
            CWF::DataLoader::GetSingleton().LoadAllContentPacks();
            CWF::InputHandler::GetSingleton().Register();
        }
    }
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) {
    InitializeLogging();
    SKSE::log::info("[CWF] SKSEPlugin_Load initialized. Version: 1.0.0");

    SKSE::Init(a_skse);

    auto messaging = SKSE::GetMessagingInterface();
    if (messaging) {
        messaging->RegisterListener(MessagingHandler);
    }

    auto serialization = SKSE::GetSerializationInterface();
    if (serialization) {
        CWF::SaveLoad::Register(serialization);
    }

    return true;
}

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(1);
    v.PluginName("Cardinal Weapon Framework");
    v.Author("Cardinal Team");
    v.UsesAddressLibrary(true);
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
    return v;
}();
