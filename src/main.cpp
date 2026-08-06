#include "CWFramework/PublicAPI.h"
#include <windows.h>
#include <cstdint>

#define SKSEAPI __stdcall

// SKSE 2.2.x+ Plugin Version Metadata Export matching official SKSE64 PluginAPI.h
struct SKSEPluginVersionData
{
    enum { kVersion = 1 };

    enum
    {
        kVersionIndependent_AddressLibraryPostAE = 1 << 0,
        kVersionIndependent_Signatures = 1 << 1,
        kVersionIndependent_StructsPost629 = 1 << 2,
    };

    enum
    {
        kVersionIndependentEx_NoStructUse = 1 << 0,
    };

    uint32_t dataVersion{ kVersion };
    uint32_t pluginVersion{ 1 };
    char name[256]{ "Cardinal Weapon Framework" };
    char author[256]{ "Darkraddish" };
    char supportEmail[252]{ "" };
    uint32_t versionIndependenceEx{ 0 };
    uint32_t versionIndependence{ kVersionIndependent_AddressLibraryPostAE | kVersionIndependent_StructsPost629 };
    uint32_t compatibleVersions[16]{ 0x01064920, 0 };
    uint32_t seVersionRequired{ 0 };
};

extern "C" __declspec(dllexport) constinit SKSEPluginVersionData SKSEPlugin_Version = {
    SKSEPluginVersionData::kVersion,                             // dataVersion
    1,                                                           // pluginVersion
    "Cardinal Weapon Framework",                                 // name
    "Darkraddish",                                               // author
    "",                                                          // supportEmail
    0,                                                           // versionIndependenceEx
    SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE | SKSEPluginVersionData::kVersionIndependent_StructsPost629, // versionIndependence
    { 0x01064920, 0 },                                           // compatibleVersions
    0                                                            // seVersionRequired
};

#include <fstream>
#include <iostream>
#include <chrono>

namespace
{
    std::atomic<bool> g_running{ true };

    void LogDiagnostic(const std::string& msg)
    {
        static std::ofstream logFile(R"(c:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition\Data\SKSE\Plugins\CWFramework.log)", std::ios::app);
        if (logFile.is_open())
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            char timeBuf[100];
            ctime_s(timeBuf, sizeof(timeBuf), &time);
            std::string tStr(timeBuf);
            if (!tStr.empty() && tStr.back() == '\n') tStr.pop_back();
            logFile << "[" << tStr << "] " << msg << std::endl;
            logFile.flush();
        }
    }

    void InputMonitorThread()
    {
        LogDiagnostic("InputMonitorThread started monitoring 'O' key (0x4F)...");
        bool oWasDown = false;
        while (g_running)
        {
            bool oIsDown = (GetAsyncKeyState(0x4F) & 0x8000) != 0;
            if (oIsDown && !oWasDown)
            {
                bool isLocked = CWFramework::SaveManager::GetInstance().IsWeaponLocked();
                LogDiagnostic("Key 'O' pressed. IsWeaponLocked = " + std::string(isLocked ? "true" : "false"));
                if (isLocked)
                {
                    LogDiagnostic("Opening Cardinal Tree UI Menu...");
                    CWFramework::CardinalTreeUIManager::GetInstance().OpenCardinalTreeMenu();
                }
                else
                {
                    LogDiagnostic("Pre-awakening key 'O' pressed. Weapon not awakened yet. Player must read 'The Legend of the Four Cardinal Weapons'.");
                }
            }
            oWasDown = oIsDown;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

// C++ Papyrus Native Handlers & Framework Entry Points
namespace CWFrameworkBindings
{
    bool IsWeaponLocked() {
        bool locked = CWFramework::SaveManager::GetInstance().IsWeaponLocked();
        LogDiagnostic("Papyrus Binding: IsWeaponLocked() -> " + std::string(locked ? "true" : "false"));
        return locked;
    }

    bool InitiateWeaponSelection(std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: InitiateWeaponSelection('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection(weaponTypeId);
        LogDiagnostic("  -> InitiateWeaponSelection result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    bool ConfirmWeaponChoice(std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: ConfirmWeaponChoice('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice(weaponTypeId);
        LogDiagnostic("  -> ConfirmWeaponChoice result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    std::string GetVersion() {
        LogDiagnostic("Papyrus Binding: GetVersion()");
        return CWFramework::PublicAPI::GetVersion();
    }

    bool IsFrameworkReady() {
        LogDiagnostic("Papyrus Binding: IsFrameworkReady()");
        return CWFramework::PublicAPI::IsFrameworkReady();
    }
}

struct SKSEMessagingInterface
{
    enum { kVersion = 2 };
    enum
    {
        kMessage_PostLoad = 0,
        kMessage_PostPostLoad,
        kMessage_PreLoadGame,
        kMessage_PostLoadGame,
        kMessage_SaveGame,
        kMessage_DeleteGame,
        kMessage_InputLoaded,
        kMessage_NewGame,
        kMessage_DataLoaded
    };

    typedef void (*EventCallback)(struct SKSEMessage* msg);

    uint32_t interfaceVersion;
    bool (*RegisterListener)(const char* sender, EventCallback handler);
    bool (*Dispatch)(uint32_t messageType, void* data, uint32_t dataLen, const char* receiver);
};

struct SKSEMessage
{
    const char* sender;
    uint32_t type;
    uint32_t dataLen;
    void* data;
};

namespace
{
    void OnSKSEMessage(SKSEMessage* msg)
    {
        if (!msg) return;
        if (msg->type == SKSEMessagingInterface::kMessage_PostLoadGame || msg->type == SKSEMessagingInterface::kMessage_NewGame || msg->type == SKSEMessagingInterface::kMessage_DataLoaded)
        {
            LogDiagnostic("SKSE Message received (Game Load / Data Loaded). Auto-initializing framework quests...");
        }
    }
}

// SKSE Plugin Load Entry Point Export
extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const void* a_skse)
{
    // Initialize Framework native engines
    if (!CWFramework::PublicAPI::IsFrameworkReady())
    {
        return false;
    }

    // Perform Stage 1 Load-Time Content Validation
    CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();

    // Spawn C++ Input Monitor for 'O' Hotkey
    std::thread(InputMonitorThread).detach();

    LogDiagnostic("CWFramework.dll loaded successfully.");
    return true;
}
