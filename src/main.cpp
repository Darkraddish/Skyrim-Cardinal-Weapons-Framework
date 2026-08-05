#include "CWFramework/PublicAPI.h"
#include <windows.h>
#include <cstdint>

#define SKSEAPI __stdcall

// SKSE 2.2.x+ Plugin Version Metadata Export
struct SKSEPluginVersionData
{
    uint32_t dataVersion{ 1 };
    uint32_t pluginVersion{ 1 }; // Version 0, Build 1
    char name[256]{ "Cardinal Weapon Framework v0.1.0" };
    char author[256]{ "Darkraddish" };
    uint32_t addressLibraryVersion{ 1 };
    uint32_t sigStructVersion{ 1 };
    uint32_t compatibleVersions[16]{ 0 };
    uint32_t minimumSKSEVersion{ 0 };
};

extern "C" __declspec(dllexport) constinit SKSEPluginVersionData SKSEPlugin_Version = {
    1,                                  // Data Version
    1,                                  // Plugin Version (Version 0, Build 1)
    "Cardinal Weapon Framework v0.1.0", // Name
    "Darkraddish",                       // Author
    1,                                  // Address Library Version (1 = AE / Address Library compatible)
    1,                                  // Signature Struct Version (1 = Uses Updated Structs)
    { 0 },                              // 0 = Compatible with all Address Library versions
    0                                   // Minimum SKSE Version
};

// SKSE Plugin Load Entry Point Export
extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const void* a_skse)
{
    // Initialize Framework native engines
    if (!CWFramework::PublicAPI::IsFrameworkReady())
    {
        return false;
    }
    return true;
}
