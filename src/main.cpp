#include "CWFramework/PublicAPI.h"
#include <windows.h>
#include <cstdint>

#define SKSEAPI __stdcall

// SKSE 2.2.x+ Plugin Version Metadata Export
struct SKSEPluginVersionData
{
    uint32_t dataVersion{ 1 };
    uint32_t pluginVersion{ 1 };
    char name[256]{ "Cardinal Weapon Framework" };
    char author[256]{ "Darkraddish" };
    uint32_t addressLibraryVersion{ 1 };
    uint32_t sigStructVersion{ 1 };
    uint32_t compatibleVersions[16]{ 0 };
    uint32_t minimumSKSEVersion{ 0 };
};

extern "C" __declspec(dllexport) constinit SKSEPluginVersionData SKSEPlugin_Version = {
    1,                           // Data Version
    1,                           // Plugin Version
    "Cardinal Weapon Framework", // Name
    "Darkraddish",                // Author
    0,                           // Address Library Version
    0,                           // Signature Struct Version
    { 0 },                       // Compatible Versions
    0                            // Minimum SKSE Version
};

// SKSE Plugin Query Info Export
struct SKSEPluginInfo
{
    uint32_t infoVersion{ 1 };
    const char* name{ "Cardinal Weapon Framework" };
    uint32_t version{ 1 };
};

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const void* a_skse, SKSEPluginInfo* a_info)
{
    if (a_info)
    {
        a_info->infoVersion = 1;
        a_info->name = "Cardinal Weapon Framework";
        a_info->version = 1;
    }
    return true;
}

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
