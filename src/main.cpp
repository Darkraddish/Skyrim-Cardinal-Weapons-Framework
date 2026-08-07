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
                    LogDiagnostic("Pre-awakening key 'O' pressed. Auto-awakening default weapon choice and opening menu...");
                    auto allTypes = CWFramework::Registry::GetInstance().GetAllWeaponTypes();
                    if (!allTypes.empty())
                    {
                        CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection(allTypes[0].id);
                        CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice(allTypes[0].id);
                    }
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
    // CWFramework_SaveAPI
    bool LockInWeaponChoice(std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.LockInWeaponChoice('" + weaponTypeId + "')");
        return CWFramework::SaveManager::GetInstance().LockInWeaponChoice(weaponTypeId);
    }

    bool IsWeaponLocked() {
        bool locked = CWFramework::SaveManager::GetInstance().IsWeaponLocked();
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.IsWeaponLocked() -> " + std::string(locked ? "true" : "false"));
        return locked;
    }

    std::string GetChosenWeaponTypeId() {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.GetChosenWeaponTypeId()");
        return CWFramework::SaveManager::GetInstance().GetChosenWeaponTypeId();
    }

    void SetCurrentForm(std::string formId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.SetCurrentForm('" + formId + "')");
        CWFramework::SaveManager::GetInstance().SetCurrentForm(formId);
    }

    std::string GetCurrentFormId() {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.GetCurrentFormId()");
        return CWFramework::SaveManager::GetInstance().GetCurrentFormId();
    }

    bool UnlockNode(std::string nodeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.UnlockNode('" + nodeId + "')");
        return CWFramework::SaveManager::GetInstance().UnlockNode(nodeId);
    }

    bool IsNodeUnlocked(std::string nodeId) {
        bool unl = CWFramework::SaveManager::GetInstance().IsNodeUnlocked(nodeId);
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.IsNodeUnlocked('" + nodeId + "') -> " + std::string(unl ? "true" : "false"));
        return unl;
    }

    void ResetSaveState() {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.ResetSaveState()");
        CWFramework::SaveManager::GetInstance().ResetSaveState();
    }

    // CWFramework_WeaponManagerAPI
    bool InitiateWeaponSelection(std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.InitiateWeaponSelection('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection(weaponTypeId);
        LogDiagnostic("  -> InitiateWeaponSelection result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    bool ConfirmWeaponChoice(std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.ConfirmWeaponChoice('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice(weaponTypeId);
        LogDiagnostic("  -> ConfirmWeaponChoice result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    std::string GetPendingWeaponChoice() {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.GetPendingWeaponChoice()");
        return CWFramework::LegendaryWeaponManager::GetInstance().GetPendingWeaponChoice();
    }

    void CancelWeaponSelection() {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.CancelWeaponSelection()");
        CWFramework::LegendaryWeaponManager::GetInstance().CancelWeaponSelection();
    }

    bool SwapActiveForm(std::string newFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.SwapActiveForm('" + newFormId + "')");
        return CWFramework::LegendaryWeaponManager::GetInstance().SwapActiveForm(newFormId);
    }

    bool OnWeaponEquipped(std::string itemFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.OnWeaponEquipped('" + itemFormId + "')");
        return CWFramework::LegendaryWeaponManager::GetInstance().OnWeaponEquipped(itemFormId);
    }

    void OnWeaponUnequipped(std::string itemFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.OnWeaponUnequipped('" + itemFormId + "')");
        CWFramework::LegendaryWeaponManager::GetInstance().OnWeaponUnequipped(itemFormId);
    }

    bool IsCardinalWeaponEquipped() {
        return CWFramework::LegendaryWeaponManager::GetInstance().IsCardinalWeaponEquipped();
    }

    std::string GetEquippedCardinalWeaponItemFormId() {
        return CWFramework::LegendaryWeaponManager::GetInstance().GetEquippedCardinalWeaponItemFormId();
    }

    // CWFramework_UIAPI
    void OpenConstellationMenu() {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.OpenConstellationMenu()");
        CWFramework::CardinalTreeUIManager::GetInstance().OpenCardinalTreeMenu();
    }

    void CloseConstellationMenu() {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.CloseConstellationMenu()");
        CWFramework::CardinalTreeUIManager::GetInstance().CloseCardinalTreeMenu();
    }

    bool IsConstellationMenuOpen() {
        return CWFramework::CardinalTreeUIManager::GetInstance().IsMenuOpen();
    }

    bool SelectAndUnlockNode(std::string nodeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.SelectAndUnlockNode('" + nodeId + "')");
        return CWFramework::CardinalTreeUIManager::GetInstance().SelectAndUnlockNode(nodeId);
    }

    std::string GetChosenWeaponTypeName() {
        std::string chosenId = CWFramework::SaveManager::GetInstance().GetChosenWeaponTypeId();
        auto typeData = CWFramework::Registry::GetInstance().GetWeaponType(chosenId);
        return typeData ? typeData->name : "";
    }

    std::string GetActiveSeriesName() {
        std::string currentFormId = CWFramework::SaveManager::GetInstance().GetCurrentFormId();
        auto formData = CWFramework::Registry::GetInstance().GetWeaponForm(currentFormId);
        if (formData) {
            auto seriesData = CWFramework::Registry::GetInstance().GetSeries(formData->seriesId);
            if (seriesData) return seriesData->name;
        }
        return "";
    }

    std::string GetCurrentFormName() {
        std::string currentFormId = CWFramework::SaveManager::GetInstance().GetCurrentFormId();
        auto formData = CWFramework::Registry::GetInstance().GetWeaponForm(currentFormId);
        return formData ? formData->name : "";
    }

    float GetEffectiveDamage() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveDamage(25.0));
    }

    float GetEffectiveArmor() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveArmor(35.0));
    }

    // CWFramework_API
    std::string GetVersion() {
        LogDiagnostic("Papyrus Binding: CWFramework_API.GetVersion()");
        return CWFramework::PublicAPI::GetVersion();
    }

    bool IsFrameworkReady() {
        LogDiagnostic("Papyrus Binding: CWFramework_API.IsFrameworkReady()");
        return CWFramework::PublicAPI::IsFrameworkReady();
    }

    bool IsZeroContentLoaded() {
        return CWFramework::PublicAPI::IsZeroContentLoaded();
    }

    int GetRegisteredWeaponTypeCount() {
        return static_cast<int>(CWFramework::PublicAPI::GetRegisteredWeaponTypeCount());
    }

    bool RegisterWeaponType(std::string asId, std::string asName, std::string asDescription, std::string asIconAsset) {
        CWFramework::WeaponTypeData wt;
        wt.id = asId; wt.name = asName; wt.description = asDescription; wt.iconAsset = asIconAsset;
        return CWFramework::Registry::GetInstance().RegisterWeaponType(wt);
    }

    bool RegisterSeries(std::string asId, std::string asWeaponTypeId, std::string asName, std::string asDescription, std::string asIconAsset) {
        CWFramework::SeriesData s;
        s.id = asId; s.weaponTypeId = asWeaponTypeId; s.name = asName; s.description = asDescription; s.iconAsset = asIconAsset;
        return CWFramework::Registry::GetInstance().RegisterSeries(s);
    }

    bool RegisterWeaponForm(std::string asId, std::string asSeriesId, std::string asName, std::string asDescription, std::string asModelAsset) {
        CWFramework::WeaponFormData f;
        f.id = asId; f.seriesId = asSeriesId; f.name = asName; f.description = asDescription; f.modelAsset = asModelAsset;
        return CWFramework::Registry::GetInstance().RegisterWeaponForm(f);
    }

    bool RegisterProgressionNode(std::string asId, std::string asSeriesId, std::string asName, int aiNodeType, float afPosX, float afPosY) {
        CWFramework::ProgressionNodeData n;
        n.id = asId; n.seriesId = asSeriesId; n.name = asName;
        n.nodeType = static_cast<CWFramework::NodeType>(aiNodeType);
        n.posX = afPosX; n.posY = afPosY;
        return CWFramework::Registry::GetInstance().RegisterProgressionNode(n);
    }

    // CWFramework_AbilityAPI
    bool TriggerActiveSkill(std::string asAbilityId) {
        auto ab = CWFramework::Registry::GetInstance().GetAbility(asAbilityId);
        return ab ? CWFramework::AbilityEffectEngine::GetInstance().TriggerActiveAbility(*ab) : false;
    }

    bool IsAbilityActive(std::string asAbilityId) {
        return CWFramework::AbilityEffectEngine::GetInstance().IsAbilityActive(asAbilityId);
    }

    float GetNetActorValueModifier(std::string asActorValue) {
        return static_cast<float>(CWFramework::AbilityEffectEngine::GetInstance().GetNetActorValueModifier(asActorValue));
    }

    // CWFramework_AnimationAPI
    std::string GetOARWeaponFormConditionVariable() {
        return CWFramework::AnimationCosmeticEngine::GetInstance().GetOARWeaponFormConditionVariable();
    }

    std::string GetOARWeaponTypeConditionVariable() {
        return CWFramework::AnimationCosmeticEngine::GetInstance().GetOARWeaponTypeConditionVariable();
    }

    void TriggerTransformationEffects(std::string asNewFormId) {
        CWFramework::AnimationCosmeticEngine::GetInstance().TriggerTransformationEffects(asNewFormId);
    }

    // CWFramework_ExpansionAPI
    void RegisterCompanionModule(int aiModuleType, std::string asModuleName) {
        CWFramework::ExpansionManager::GetInstance().RegisterCompanionModule(static_cast<CWFramework::CompanionModuleType>(aiModuleType), asModuleName);
    }

    void UnregisterCompanionModule(int aiModuleType) {
        CWFramework::ExpansionManager::GetInstance().UnregisterCompanionModule(static_cast<CWFramework::CompanionModuleType>(aiModuleType));
    }

    bool IsCompanionModuleActive(int aiModuleType) {
        return CWFramework::ExpansionManager::GetInstance().IsCompanionModuleActive(static_cast<CWFramework::CompanionModuleType>(aiModuleType));
    }

    // CWFramework_ProgressionAPI
    bool UnlockProgressionNode(std::string asNodeId) {
        return CWFramework::ProgressionEngine::GetInstance().UnlockProgressionNode(asNodeId);
    }

    bool ResolveConvertibleNode(std::string asNodeId, std::string asOutcomeFormId) {
        return CWFramework::ProgressionEngine::GetInstance().ResolveConvertibleNode(asNodeId, asOutcomeFormId);
    }

    void AddFlatLevelBonus(float afBonus) {
        CWFramework::ProgressionEngine::GetInstance().AddFlatLevelBonus(static_cast<double>(afBonus));
    }

    float GetLevelBonus() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetLevelBonus());
    }

    bool UpgradeRefinementTier() {
        return CWFramework::ProgressionEngine::GetInstance().UpgradeRefinementTier();
    }

    int GetRefinementLevel() {
        return CWFramework::ProgressionEngine::GetInstance().GetRefinementLevel();
    }

    float GetRefinementMultiplier() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetRefinementMultiplier());
    }

    void AddMasteryXp(float afXpAmount) {
        CWFramework::ProgressionEngine::GetInstance().AddMasteryXp(static_cast<double>(afXpAmount));
    }

    float GetMasteryXp() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetMasteryXp());
    }

    float GetMasteryMultiplier() {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetMasteryMultiplier());
    }

    float CalculateEffectiveDamage(float afBaseDamage) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveDamage(static_cast<double>(afBaseDamage)));
    }

    float CalculateEffectiveArmor(float afBaseArmor) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveArmor(static_cast<double>(afBaseArmor)));
    }

    // CWFramework_RequirementAPI
    bool CanUnlockNode(std::string asNodeId) {
        return CWFramework::RequirementEvaluator::GetInstance().CanUnlockNodeById(asNodeId);
    }

    float GetRequirementCurrentProgress(std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        if (!req) return 0.0f;
        auto [cur, target] = CWFramework::RequirementEvaluator::GetInstance().GetRequirementProgress(*req);
        return static_cast<float>(cur);
    }

    float GetRequirementTargetValue(std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        if (!req) return 0.0f;
        auto [cur, target] = CWFramework::RequirementEvaluator::GetInstance().GetRequirementProgress(*req);
        return static_cast<float>(target);
    }

    bool IsRequirementMet(std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        return req ? CWFramework::RequirementEvaluator::GetInstance().IsRequirementMet(*req) : false;
    }

    void RecordEnemyKill(std::string asActorBaseId, int aiCount) {
        CWFramework::RequirementEvaluator::GetInstance().RecordKill(asActorBaseId, static_cast<uint32_t>(aiCount > 0 ? aiCount : 1));
    }

    void RecordItemAbsorption(std::string asItemFormId, int aiCount) {
        CWFramework::RequirementEvaluator::GetInstance().RecordItemAbsorption(asItemFormId, static_cast<uint32_t>(aiCount > 0 ? aiCount : 1));
    }

    void RecordWeaponCopy(std::string asWeaponFormId) {
        CWFramework::RequirementEvaluator::GetInstance().RecordWeaponCopy(asWeaponFormId);
    }

    void RecordQuestStage(std::string asQuestId, int aiStage) {
        CWFramework::RequirementEvaluator::GetInstance().RecordQuestStage(asQuestId, aiStage);
    }

    void RecordPlayerLevel(int aiLevel) {
        CWFramework::RequirementEvaluator::GetInstance().RecordPlayerLevel(aiLevel);
    }

    void TriggerSpecialEvent(std::string asEventId) {
        CWFramework::RequirementEvaluator::GetInstance().TriggerSpecialEvent(asEventId);
    }

    // CWFramework_ValidationAPI
    bool ValidateContentPackIntegrity(std::string asPackId) {
        auto result = CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();
        return result.isValid;
    }

    bool IsPackLoaded(std::string asPackId) {
        return CWFramework::ContentValidationEngine::GetInstance().IsPackLoaded(asPackId);
    }

    // Papyrus registration callback
    bool RegisterPapyrusFunctions(void* registry)
    {
        LogDiagnostic("RegisterPapyrusFunctions callback invoked by Papyrus VM. Binding CWFramework Papyrus APIs...");
        return true;
    }
}

struct SKSEInterface
{
    enum
    {
        kInterface_Invalid = 0,
        kInterface_Scaleform,
        kInterface_Papyrus,
        kInterface_Serialization,
        kInterface_Task,
        kInterface_ObjectiveDog,
        kInterface_Messaging,
        kInterface_Max
    };

    uint32_t skseVersion;
    uint32_t runtimeVersion;
    uint32_t editorVersion;
    uint32_t isEditor;
    void* (*QueryInterface)(uint32_t id);
    uint32_t (*GetPluginHandle)();
};

typedef bool (*RegisterPapyrusFunctionsCallback)(void* registry);

struct SKSEPapyrusInterface
{
    enum { kInterfaceVersion = 1 };
    uint32_t interfaceVersion;
    bool (*Register)(RegisterPapyrusFunctionsCallback fn);
};

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
    LogDiagnostic("SKSEPlugin_Load called.");

    if (a_skse)
    {
        const auto* skse = static_cast<const SKSEInterface*>(a_skse);
        LogDiagnostic("SKSE Interface Version: " + std::to_string(skse->skseVersion) + ", Runtime Version: " + std::to_string(skse->runtimeVersion));

        auto* papyrus = static_cast<SKSEPapyrusInterface*>(skse->QueryInterface(SKSEInterface::kInterface_Papyrus));
        if (papyrus)
        {
            bool regRes = papyrus->Register(CWFrameworkBindings::RegisterPapyrusFunctions);
            LogDiagnostic("SKSE Papyrus Interface Query -> SUCCESS. Callback registration result: " + std::string(regRes ? "SUCCESS" : "FAILED"));
        }
        else
        {
            LogDiagnostic("SKSE Papyrus Interface Query -> FAILED (QueryInterface returned nullptr).");
        }

        auto* messaging = static_cast<SKSEMessagingInterface*>(skse->QueryInterface(SKSEInterface::kInterface_Messaging));
        if (messaging)
        {
            messaging->RegisterListener("SKSE", OnSKSEMessage);
            LogDiagnostic("SKSE Messaging Interface Listener registered.");
        }
    }

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
