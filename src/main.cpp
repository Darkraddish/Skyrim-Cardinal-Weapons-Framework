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
                    LogDiagnostic("Pre-awakening key 'O' pressed. Auto-awakening default Cardinal Shield and opening menu...");
                    CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection("CW_WT_SHIELD");
                    CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice("CW_WT_SHIELD");
                    CWFramework::CardinalTreeUIManager::GetInstance().OpenCardinalTreeMenu();
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

void RunFrameworkUnitTests()
{
    std::string testLogPath = "Data/SKSE/Plugins/CWFramework_UnitTest.log";
    std::ofstream testLog(testLogPath, std::ios::out | std::ios::trunc);
    if (!testLog.is_open()) return;

    testLog << "=====================================================\n";
    testLog << "   CARDINAL WEAPONS FRAMEWORK AUTOMATED UNIT TESTS   \n";
    testLog << "=====================================================\n\n";

    int totalTests = 0;
    int passedTests = 0;

    auto RunTest = [&](const std::string& name, std::function<bool()> testFunc) {
        totalTests++;
        testLog << "[TEST #" << totalTests << "] " << name << " ... ";
        try {
            bool ok = testFunc();
            if (ok) {
                passedTests++;
                testLog << "PASSED\n";
            } else {
                testLog << "FAILED\n";
            }
        } catch (const std::exception& e) {
            testLog << "EXCEPTION: " << e.what() << "\n";
        }
    };

    // Test 1: Native Plugin Version & Framework Readiness
    RunTest("PublicAPI Readiness & Version Check", []() {
        if (!CWFramework::PublicAPI::IsFrameworkReady()) return false;
        std::string ver = CWFramework::PublicAPI::GetVersion();
        return ver == "1.0.0";
    });

    // Test 2: Registry Singleton & Base Form Registration
    RunTest("Weapon Type Registry Pre-Registration", []() {
        auto types = CWFramework::Registry::GetInstance().GetAllWeaponTypes();
        bool hasShield = false;
        for (const auto& t : types) {
            if (t.id == "CW_WT_SHIELD") hasShield = true;
        }
        return hasShield;
    });

    // Test 3: Save API Initial Lock State
    RunTest("SaveManager Initial Unlocked State", []() {
        return !CWFramework::SaveManager::GetInstance().IsWeaponLocked();
    });

    // Test 4: Legendary Weapon Selection Lifecycle
    RunTest("LegendaryWeaponManager Initiate & Confirm Choice", []() {
        bool initOk = CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection("CW_WT_SHIELD");
        if (!initOk) return false;
        bool confirmOk = CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice("CW_WT_SHIELD");
        if (!confirmOk) return false;
        bool isLockedNow = CWFramework::SaveManager::GetInstance().IsWeaponLocked();
        std::string activeForm = CWFramework::SaveManager::GetInstance().GetChosenWeaponTypeId();
        
        // Reset state for game runtime testing
        CWFramework::SaveManager::GetInstance().ResetSaveState();
        return isLockedNow && activeForm == "CW_WT_SHIELD";
    });

    // Test 5: Plugin ESP Binary Record File Verification
    RunTest("CardinalWeaponsFramework.esp File Existence & Header Check", []() {
        std::ifstream esp("Data/CardinalWeaponsFramework.esp", std::ios::binary);
        if (!esp.is_open()) return false;
        char header[4];
        esp.read(header, 4);
        esp.close();
        return std::string(header, 4) == "TES4";
    });

    testLog << "\n=====================================================\n";
    testLog << "  SUMMARY: " << passedTests << " / " << totalTests << " TESTS PASSED (" << (passedTests * 100 / totalTests) << "% SUCCESS)\n";
    testLog << "=====================================================\n";
    testLog.close();
}

// SKSE Plugin Load Entry Point Export
extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const void* a_skse)
{
    // Initialize Framework native engines
    if (!CWFramework::PublicAPI::IsFrameworkReady())
    {
        return false;
    }

    // Run Automated Unit Tests on Startup
    RunFrameworkUnitTests();

    // Perform Stage 1 Load-Time Content Validation
    CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();

    // Spawn C++ Input Monitor for 'O' Hotkey
    std::thread(InputMonitorThread).detach();

    LogDiagnostic("CWFramework.dll loaded successfully.");
    return true;
}
