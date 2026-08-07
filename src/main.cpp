#include "CWFramework/PublicAPI.h"
#include <windows.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>

#if __has_include(<SKSE/SKSE.h>) && __has_include(<RE/Skyrim.h>)
#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#define CW_HAS_COMMONLIB 1
#else
#define CW_HAS_COMMONLIB 0
#endif

// SKSE Plugin Version Metadata Export required by skse64_loader
#if CW_HAS_COMMONLIB
extern "C" __declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(1);
    v.PluginName("Cardinal Weapon Framework");
    v.AuthorName("Darkraddish");
    v.UsesAddressLibrary(true);
    v.UsesUpdatedStructs(true);
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
    return v;
}();
#else
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
    SKSEPluginVersionData::kVersion,
    1,
    "Cardinal Weapon Framework",
    "Darkraddish",
    "",
    0,
    SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE | SKSEPluginVersionData::kVersionIndependent_StructsPost629,
    { 0x01064920, 0 },
    0
};
#endif

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

#if !CW_HAS_COMMONLIB
namespace RE
{
    class StaticFunctionTag {};
    namespace BSScript
    {
        class IVirtualMachine
        {
        public:
            virtual ~IVirtualMachine() = default;
            template <typename F>
            bool RegisterFunction(const char* fnName, const char* scriptName, F func, bool isUnboxUnsafe = false)
            {
                return true;
            }
        };
    }
}
#endif

namespace CWFrameworkBindings
{
    // CWFramework_SaveAPI
    bool LockInWeaponChoice(RE::StaticFunctionTag*, std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.LockInWeaponChoice('" + weaponTypeId + "')");
        return CWFramework::SaveManager::GetInstance().LockInWeaponChoice(weaponTypeId);
    }

    bool IsWeaponLocked(RE::StaticFunctionTag*) {
        bool locked = CWFramework::SaveManager::GetInstance().IsWeaponLocked();
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.IsWeaponLocked() -> " + std::string(locked ? "true" : "false"));
        return locked;
    }

    std::string GetChosenWeaponTypeId(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.GetChosenWeaponTypeId()");
        return CWFramework::SaveManager::GetInstance().GetChosenWeaponTypeId();
    }

    void SetCurrentForm(RE::StaticFunctionTag*, std::string formId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.SetCurrentForm('" + formId + "')");
        CWFramework::SaveManager::GetInstance().SetCurrentForm(formId);
    }

    std::string GetCurrentFormId(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.GetCurrentFormId()");
        return CWFramework::SaveManager::GetInstance().GetCurrentFormId();
    }

    bool UnlockNode(RE::StaticFunctionTag*, std::string nodeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.UnlockNode('" + nodeId + "')");
        return CWFramework::SaveManager::GetInstance().UnlockNode(nodeId);
    }

    bool IsNodeUnlocked(RE::StaticFunctionTag*, std::string nodeId) {
        bool unl = CWFramework::SaveManager::GetInstance().IsNodeUnlocked(nodeId);
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.IsNodeUnlocked('" + nodeId + "') -> " + std::string(unl ? "true" : "false"));
        return unl;
    }

    void ResetSaveState(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_SaveAPI.ResetSaveState()");
        CWFramework::SaveManager::GetInstance().ResetSaveState();
    }

    // CWFramework_WeaponManagerAPI
    bool InitiateWeaponSelection(RE::StaticFunctionTag*, std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.InitiateWeaponSelection('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().InitiateWeaponSelection(weaponTypeId);
        LogDiagnostic("  -> InitiateWeaponSelection result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    bool ConfirmWeaponChoice(RE::StaticFunctionTag*, std::string weaponTypeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.ConfirmWeaponChoice('" + weaponTypeId + "')");
        bool res = CWFramework::LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice(weaponTypeId);
        LogDiagnostic("  -> ConfirmWeaponChoice result: " + std::string(res ? "SUCCESS" : "FAILED"));
        return res;
    }

    std::string GetPendingWeaponChoice(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.GetPendingWeaponChoice()");
        return CWFramework::LegendaryWeaponManager::GetInstance().GetPendingWeaponChoice();
    }

    void CancelWeaponSelection(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.CancelWeaponSelection()");
        CWFramework::LegendaryWeaponManager::GetInstance().CancelWeaponSelection();
    }

    bool SwapActiveForm(RE::StaticFunctionTag*, std::string newFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.SwapActiveForm('" + newFormId + "')");
        return CWFramework::LegendaryWeaponManager::GetInstance().SwapActiveForm(newFormId);
    }

    bool OnWeaponEquipped(RE::StaticFunctionTag*, std::string itemFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.OnWeaponEquipped('" + itemFormId + "')");
        return CWFramework::LegendaryWeaponManager::GetInstance().OnWeaponEquipped(itemFormId);
    }

    void OnWeaponUnequipped(RE::StaticFunctionTag*, std::string itemFormId) {
        LogDiagnostic("Papyrus Binding: CWFramework_WeaponManagerAPI.OnWeaponUnequipped('" + itemFormId + "')");
        CWFramework::LegendaryWeaponManager::GetInstance().OnWeaponUnequipped(itemFormId);
    }

    bool IsCardinalWeaponEquipped(RE::StaticFunctionTag*) {
        return CWFramework::LegendaryWeaponManager::GetInstance().IsCardinalWeaponEquipped();
    }

    std::string GetEquippedCardinalWeaponItemFormId(RE::StaticFunctionTag*) {
        return CWFramework::LegendaryWeaponManager::GetInstance().GetEquippedCardinalWeaponItemFormId();
    }

    // CWFramework_UIAPI
    void OpenConstellationMenu(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.OpenConstellationMenu()");
        CWFramework::CardinalTreeUIManager::GetInstance().OpenCardinalTreeMenu();
    }

    void CloseConstellationMenu(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.CloseConstellationMenu()");
        CWFramework::CardinalTreeUIManager::GetInstance().CloseCardinalTreeMenu();
    }

    bool IsConstellationMenuOpen(RE::StaticFunctionTag*) {
        return CWFramework::CardinalTreeUIManager::GetInstance().IsMenuOpen();
    }

    bool SelectAndUnlockNode(RE::StaticFunctionTag*, std::string nodeId) {
        LogDiagnostic("Papyrus Binding: CWFramework_UIAPI.SelectAndUnlockNode('" + nodeId + "')");
        return CWFramework::CardinalTreeUIManager::GetInstance().SelectAndUnlockNode(nodeId);
    }

    std::string GetChosenWeaponTypeName(RE::StaticFunctionTag*) {
        std::string chosenId = CWFramework::SaveManager::GetInstance().GetChosenWeaponTypeId();
        auto typeData = CWFramework::Registry::GetInstance().GetWeaponType(chosenId);
        return typeData ? typeData->name : "";
    }

    std::string GetActiveSeriesName(RE::StaticFunctionTag*) {
        std::string currentFormId = CWFramework::SaveManager::GetInstance().GetCurrentFormId();
        auto formData = CWFramework::Registry::GetInstance().GetWeaponForm(currentFormId);
        if (formData) {
            auto seriesData = CWFramework::Registry::GetInstance().GetSeries(formData->seriesId);
            if (seriesData) return seriesData->name;
        }
        return "";
    }

    std::string GetCurrentFormName(RE::StaticFunctionTag*) {
        std::string currentFormId = CWFramework::SaveManager::GetInstance().GetCurrentFormId();
        auto formData = CWFramework::Registry::GetInstance().GetWeaponForm(currentFormId);
        return formData ? formData->name : "";
    }

    float GetEffectiveDamage(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveDamage(25.0));
    }

    float GetEffectiveArmor(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveArmor(35.0));
    }

    // CWFramework_API
    std::string GetVersion(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_API.GetVersion()");
        return CWFramework::PublicAPI::GetVersion();
    }

    bool IsFrameworkReady(RE::StaticFunctionTag*) {
        LogDiagnostic("Papyrus Binding: CWFramework_API.IsFrameworkReady()");
        return CWFramework::PublicAPI::IsFrameworkReady();
    }

    bool IsZeroContentLoaded(RE::StaticFunctionTag*) {
        return CWFramework::PublicAPI::IsZeroContentLoaded();
    }

    int GetRegisteredWeaponTypeCount(RE::StaticFunctionTag*) {
        return static_cast<int>(CWFramework::PublicAPI::GetRegisteredWeaponTypeCount());
    }

    bool RegisterWeaponType(RE::StaticFunctionTag*, std::string asId, std::string asName, std::string asDescription, std::string asIconAsset) {
        CWFramework::WeaponTypeData wt;
        wt.id = asId; wt.name = asName; wt.description = asDescription; wt.iconAsset = asIconAsset;
        return CWFramework::Registry::GetInstance().RegisterWeaponType(wt);
    }

    bool RegisterSeries(RE::StaticFunctionTag*, std::string asId, std::string asWeaponTypeId, std::string asName, std::string asDescription, std::string asIconAsset) {
        CWFramework::SeriesData s;
        s.id = asId; s.weaponTypeId = asWeaponTypeId; s.name = asName; s.description = asDescription; s.iconAsset = asIconAsset;
        return CWFramework::Registry::GetInstance().RegisterSeries(s);
    }

    bool RegisterWeaponForm(RE::StaticFunctionTag*, std::string asId, std::string asSeriesId, std::string asName, std::string asDescription, std::string asModelAsset) {
        CWFramework::WeaponFormData f;
        f.id = asId; f.seriesId = asSeriesId; f.name = asName; f.description = asDescription; f.modelAsset = asModelAsset;
        return CWFramework::Registry::GetInstance().RegisterWeaponForm(f);
    }

    bool RegisterProgressionNode(RE::StaticFunctionTag*, std::string asId, std::string asSeriesId, std::string asName, int aiNodeType, float afPosX, float afPosY) {
        CWFramework::ProgressionNodeData n;
        n.id = asId; n.seriesId = asSeriesId; n.name = asName;
        n.nodeType = static_cast<CWFramework::NodeType>(aiNodeType);
        n.posX = afPosX; n.posY = afPosY;
        return CWFramework::Registry::GetInstance().RegisterProgressionNode(n);
    }

    // CWFramework_AbilityAPI
    bool TriggerActiveSkill(RE::StaticFunctionTag*, std::string asAbilityId) {
        auto ab = CWFramework::Registry::GetInstance().GetAbility(asAbilityId);
        return ab ? CWFramework::AbilityEffectEngine::GetInstance().TriggerActiveAbility(*ab) : false;
    }

    bool IsAbilityActive(RE::StaticFunctionTag*, std::string asAbilityId) {
        return CWFramework::AbilityEffectEngine::GetInstance().IsAbilityActive(asAbilityId);
    }

    float GetNetActorValueModifier(RE::StaticFunctionTag*, std::string asActorValue) {
        return static_cast<float>(CWFramework::AbilityEffectEngine::GetInstance().GetNetActorValueModifier(asActorValue));
    }

    // CWFramework_AnimationAPI
    std::string GetOARWeaponFormConditionVariable(RE::StaticFunctionTag*) {
        return CWFramework::AnimationCosmeticEngine::GetInstance().GetOARWeaponFormConditionVariable();
    }

    std::string GetOARWeaponTypeConditionVariable(RE::StaticFunctionTag*) {
        return CWFramework::AnimationCosmeticEngine::GetInstance().GetOARWeaponTypeConditionVariable();
    }

    void TriggerTransformationEffects(RE::StaticFunctionTag*, std::string asNewFormId) {
        CWFramework::AnimationCosmeticEngine::GetInstance().TriggerTransformationEffects(asNewFormId);
    }

    // CWFramework_ExpansionAPI
    void RegisterCompanionModule(RE::StaticFunctionTag*, int aiModuleType, std::string asModuleName) {
        CWFramework::ExpansionManager::GetInstance().RegisterCompanionModule(static_cast<CWFramework::CompanionModuleType>(aiModuleType), asModuleName);
    }

    void UnregisterCompanionModule(RE::StaticFunctionTag*, int aiModuleType) {
        CWFramework::ExpansionManager::GetInstance().UnregisterCompanionModule(static_cast<CWFramework::CompanionModuleType>(aiModuleType));
    }

    bool IsCompanionModuleActive(RE::StaticFunctionTag*, int aiModuleType) {
        return CWFramework::ExpansionManager::GetInstance().IsCompanionModuleActive(static_cast<CWFramework::CompanionModuleType>(aiModuleType));
    }

    // CWFramework_ProgressionAPI
    bool UnlockProgressionNode(RE::StaticFunctionTag*, std::string asNodeId) {
        return CWFramework::ProgressionEngine::GetInstance().UnlockProgressionNode(asNodeId);
    }

    bool ResolveConvertibleNode(RE::StaticFunctionTag*, std::string asNodeId, std::string asOutcomeFormId) {
        return CWFramework::ProgressionEngine::GetInstance().ResolveConvertibleNode(asNodeId, asOutcomeFormId);
    }

    void AddFlatLevelBonus(RE::StaticFunctionTag*, float afBonus) {
        CWFramework::ProgressionEngine::GetInstance().AddFlatLevelBonus(static_cast<double>(afBonus));
    }

    float GetLevelBonus(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetLevelBonus());
    }

    bool UpgradeRefinementTier(RE::StaticFunctionTag*) {
        return CWFramework::ProgressionEngine::GetInstance().UpgradeRefinementTier();
    }

    int GetRefinementLevel(RE::StaticFunctionTag*) {
        return CWFramework::ProgressionEngine::GetInstance().GetRefinementLevel();
    }

    float GetRefinementMultiplier(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetRefinementMultiplier());
    }

    void AddMasteryXp(RE::StaticFunctionTag*, float afXpAmount) {
        CWFramework::ProgressionEngine::GetInstance().AddMasteryXp(static_cast<double>(afXpAmount));
    }

    float GetMasteryXp(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetMasteryXp());
    }

    float GetMasteryMultiplier(RE::StaticFunctionTag*) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().GetMasteryMultiplier());
    }

    float CalculateEffectiveDamage(RE::StaticFunctionTag*, float afBaseDamage) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveDamage(static_cast<double>(afBaseDamage)));
    }

    float CalculateEffectiveArmor(RE::StaticFunctionTag*, float afBaseArmor) {
        return static_cast<float>(CWFramework::ProgressionEngine::GetInstance().CalculateEffectiveArmor(static_cast<double>(afBaseArmor)));
    }

    // CWFramework_RequirementAPI
    bool CanUnlockNode(RE::StaticFunctionTag*, std::string asNodeId) {
        return CWFramework::RequirementEvaluator::GetInstance().CanUnlockNodeById(asNodeId);
    }

    float GetRequirementCurrentProgress(RE::StaticFunctionTag*, std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        if (!req) return 0.0f;
        auto [cur, target] = CWFramework::RequirementEvaluator::GetInstance().GetRequirementProgress(*req);
        return static_cast<float>(cur);
    }

    float GetRequirementTargetValue(RE::StaticFunctionTag*, std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        if (!req) return 0.0f;
        auto [cur, target] = CWFramework::RequirementEvaluator::GetInstance().GetRequirementProgress(*req);
        return static_cast<float>(target);
    }

    bool IsRequirementMet(RE::StaticFunctionTag*, std::string asReqId) {
        auto req = CWFramework::Registry::GetInstance().GetRequirement(asReqId);
        return req ? CWFramework::RequirementEvaluator::GetInstance().IsRequirementMet(*req) : false;
    }

    void RecordEnemyKill(RE::StaticFunctionTag*, std::string asActorBaseId, int aiCount) {
        CWFramework::RequirementEvaluator::GetInstance().RecordKill(asActorBaseId, static_cast<uint32_t>(aiCount > 0 ? aiCount : 1));
    }

    void RecordItemAbsorption(RE::StaticFunctionTag*, std::string asItemFormId, int aiCount) {
        CWFramework::RequirementEvaluator::GetInstance().RecordItemAbsorption(asItemFormId, static_cast<uint32_t>(aiCount > 0 ? aiCount : 1));
    }

    void RecordWeaponCopy(RE::StaticFunctionTag*, std::string asWeaponFormId) {
        CWFramework::RequirementEvaluator::GetInstance().RecordWeaponCopy(asWeaponFormId);
    }

    void RecordQuestStage(RE::StaticFunctionTag*, std::string asQuestId, int aiStage) {
        CWFramework::RequirementEvaluator::GetInstance().RecordQuestStage(asQuestId, aiStage);
    }

    void RecordPlayerLevel(RE::StaticFunctionTag*, int aiLevel) {
        CWFramework::RequirementEvaluator::GetInstance().RecordPlayerLevel(aiLevel);
    }

    void TriggerSpecialEvent(RE::StaticFunctionTag*, std::string asEventId) {
        CWFramework::RequirementEvaluator::GetInstance().TriggerSpecialEvent(asEventId);
    }

    // CWFramework_ValidationAPI
    bool ValidateContentPackIntegrity(RE::StaticFunctionTag*, std::string asPackId) {
        auto result = CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();
        return result.isValid;
    }

    bool IsPackLoaded(RE::StaticFunctionTag*, std::string asPackId) {
        return CWFramework::ContentValidationEngine::GetInstance().IsPackLoaded(asPackId);
    }

    // Papyrus registration callback
    bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* a_vm)
    {
        LogDiagnostic("RegisterPapyrusFunctions callback invoked by Papyrus VM. Binding CWFramework Papyrus APIs...");

        if (!a_vm)
        {
            LogDiagnostic("RegisterPapyrusFunctions: VM pointer is null!");
            return false;
        }

        // 1. CWFramework_SaveAPI
        a_vm->RegisterFunction("IsWeaponLocked", "CWFramework_SaveAPI", IsWeaponLocked);
        a_vm->RegisterFunction("LockInWeaponChoice", "CWFramework_SaveAPI", LockInWeaponChoice);
        a_vm->RegisterFunction("GetChosenWeaponTypeId", "CWFramework_SaveAPI", GetChosenWeaponTypeId);
        a_vm->RegisterFunction("SetCurrentForm", "CWFramework_SaveAPI", SetCurrentForm);
        a_vm->RegisterFunction("GetCurrentFormId", "CWFramework_SaveAPI", GetCurrentFormId);
        a_vm->RegisterFunction("UnlockNode", "CWFramework_SaveAPI", UnlockNode);
        a_vm->RegisterFunction("IsNodeUnlocked", "CWFramework_SaveAPI", IsNodeUnlocked);
        a_vm->RegisterFunction("ResetSaveState", "CWFramework_SaveAPI", ResetSaveState);

        // 2. CWFramework_WeaponManagerAPI
        a_vm->RegisterFunction("InitiateWeaponSelection", "CWFramework_WeaponManagerAPI", InitiateWeaponSelection);
        a_vm->RegisterFunction("ConfirmWeaponChoice", "CWFramework_WeaponManagerAPI", ConfirmWeaponChoice);
        a_vm->RegisterFunction("GetPendingWeaponChoice", "CWFramework_WeaponManagerAPI", GetPendingWeaponChoice);
        a_vm->RegisterFunction("CancelWeaponSelection", "CWFramework_WeaponManagerAPI", CancelWeaponSelection);
        a_vm->RegisterFunction("SwapActiveForm", "CWFramework_WeaponManagerAPI", SwapActiveForm);
        a_vm->RegisterFunction("OnWeaponEquipped", "CWFramework_WeaponManagerAPI", OnWeaponEquipped);
        a_vm->RegisterFunction("OnWeaponUnequipped", "CWFramework_WeaponManagerAPI", OnWeaponUnequipped);
        a_vm->RegisterFunction("IsCardinalWeaponEquipped", "CWFramework_WeaponManagerAPI", IsCardinalWeaponEquipped);
        a_vm->RegisterFunction("GetEquippedCardinalWeaponItemFormId", "CWFramework_WeaponManagerAPI", GetEquippedCardinalWeaponItemFormId);

        // 3. CWFramework_UIAPI
        a_vm->RegisterFunction("OpenConstellationMenu", "CWFramework_UIAPI", OpenConstellationMenu);
        a_vm->RegisterFunction("CloseConstellationMenu", "CWFramework_UIAPI", CloseConstellationMenu);
        a_vm->RegisterFunction("IsConstellationMenuOpen", "CWFramework_UIAPI", IsConstellationMenuOpen);
        a_vm->RegisterFunction("SelectAndUnlockNode", "CWFramework_UIAPI", SelectAndUnlockNode);
        a_vm->RegisterFunction("GetChosenWeaponTypeName", "CWFramework_UIAPI", GetChosenWeaponTypeName);
        a_vm->RegisterFunction("GetActiveSeriesName", "CWFramework_UIAPI", GetActiveSeriesName);
        a_vm->RegisterFunction("GetCurrentFormName", "CWFramework_UIAPI", GetCurrentFormName);
        a_vm->RegisterFunction("GetEffectiveDamage", "CWFramework_UIAPI", GetEffectiveDamage);
        a_vm->RegisterFunction("GetEffectiveArmor", "CWFramework_UIAPI", GetEffectiveArmor);

        // 4. CWFramework_API
        a_vm->RegisterFunction("GetVersion", "CWFramework_API", GetVersion);
        a_vm->RegisterFunction("IsFrameworkReady", "CWFramework_API", IsFrameworkReady);
        a_vm->RegisterFunction("IsZeroContentLoaded", "CWFramework_API", IsZeroContentLoaded);
        a_vm->RegisterFunction("GetRegisteredWeaponTypeCount", "CWFramework_API", GetRegisteredWeaponTypeCount);
        a_vm->RegisterFunction("RegisterWeaponType", "CWFramework_API", RegisterWeaponType);
        a_vm->RegisterFunction("RegisterSeries", "CWFramework_API", RegisterSeries);
        a_vm->RegisterFunction("RegisterWeaponForm", "CWFramework_API", RegisterWeaponForm);
        a_vm->RegisterFunction("RegisterProgressionNode", "CWFramework_API", RegisterProgressionNode);

        // 5. CWFramework_AbilityAPI
        a_vm->RegisterFunction("TriggerActiveSkill", "CWFramework_AbilityAPI", TriggerActiveSkill);
        a_vm->RegisterFunction("IsAbilityActive", "CWFramework_AbilityAPI", IsAbilityActive);
        a_vm->RegisterFunction("GetNetActorValueModifier", "CWFramework_AbilityAPI", GetNetActorValueModifier);

        // 6. CWFramework_AnimationAPI
        a_vm->RegisterFunction("GetOARWeaponFormConditionVariable", "CWFramework_AnimationAPI", GetOARWeaponFormConditionVariable);
        a_vm->RegisterFunction("GetOARWeaponTypeConditionVariable", "CWFramework_AnimationAPI", GetOARWeaponTypeConditionVariable);
        a_vm->RegisterFunction("TriggerTransformationEffects", "CWFramework_AnimationAPI", TriggerTransformationEffects);

        // 7. CWFramework_ExpansionAPI
        a_vm->RegisterFunction("RegisterCompanionModule", "CWFramework_ExpansionAPI", RegisterCompanionModule);
        a_vm->RegisterFunction("UnregisterCompanionModule", "CWFramework_ExpansionAPI", UnregisterCompanionModule);
        a_vm->RegisterFunction("IsCompanionModuleActive", "CWFramework_ExpansionAPI", IsCompanionModuleActive);

        // 8. CWFramework_ProgressionAPI
        a_vm->RegisterFunction("UnlockProgressionNode", "CWFramework_ProgressionAPI", UnlockProgressionNode);
        a_vm->RegisterFunction("ResolveConvertibleNode", "CWFramework_ProgressionAPI", ResolveConvertibleNode);
        a_vm->RegisterFunction("AddFlatLevelBonus", "CWFramework_ProgressionAPI", AddFlatLevelBonus);
        a_vm->RegisterFunction("GetLevelBonus", "CWFramework_ProgressionAPI", GetLevelBonus);
        a_vm->RegisterFunction("UpgradeRefinementTier", "CWFramework_ProgressionAPI", UpgradeRefinementTier);
        a_vm->RegisterFunction("GetRefinementLevel", "CWFramework_ProgressionAPI", GetRefinementLevel);
        a_vm->RegisterFunction("GetRefinementMultiplier", "CWFramework_ProgressionAPI", GetRefinementMultiplier);
        a_vm->RegisterFunction("AddMasteryXp", "CWFramework_ProgressionAPI", AddMasteryXp);
        a_vm->RegisterFunction("GetMasteryXp", "CWFramework_ProgressionAPI", GetMasteryXp);
        a_vm->RegisterFunction("GetMasteryMultiplier", "CWFramework_ProgressionAPI", GetMasteryMultiplier);
        a_vm->RegisterFunction("CalculateEffectiveDamage", "CWFramework_ProgressionAPI", CalculateEffectiveDamage);
        a_vm->RegisterFunction("CalculateEffectiveArmor", "CWFramework_ProgressionAPI", CalculateEffectiveArmor);

        // 9. CWFramework_RequirementAPI
        a_vm->RegisterFunction("CanUnlockNode", "CWFramework_RequirementAPI", CanUnlockNode);
        a_vm->RegisterFunction("GetRequirementCurrentProgress", "CWFramework_RequirementAPI", GetRequirementCurrentProgress);
        a_vm->RegisterFunction("GetRequirementTargetValue", "CWFramework_RequirementAPI", GetRequirementTargetValue);
        a_vm->RegisterFunction("IsRequirementMet", "CWFramework_RequirementAPI", IsRequirementMet);
        a_vm->RegisterFunction("RecordEnemyKill", "CWFramework_RequirementAPI", RecordEnemyKill);
        a_vm->RegisterFunction("RecordItemAbsorption", "CWFramework_RequirementAPI", RecordItemAbsorption);
        a_vm->RegisterFunction("RecordWeaponCopy", "CWFramework_RequirementAPI", RecordWeaponCopy);
        a_vm->RegisterFunction("RecordQuestStage", "CWFramework_RequirementAPI", RecordQuestStage);
        a_vm->RegisterFunction("RecordPlayerLevel", "CWFramework_RequirementAPI", RecordPlayerLevel);
        a_vm->RegisterFunction("TriggerSpecialEvent", "CWFramework_RequirementAPI", TriggerSpecialEvent);

        // 10. CWFramework_ValidationAPI
        a_vm->RegisterFunction("ValidateContentPackIntegrity", "CWFramework_ValidationAPI", ValidateContentPackIntegrity);
        a_vm->RegisterFunction("IsPackLoaded", "CWFramework_ValidationAPI", IsPackLoaded);

        LogDiagnostic("RegisterPapyrusFunctions: Registered ALL native API functions successfully with Papyrus VM.");
        return true;
    }
}

#if CW_HAS_COMMONLIB
SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);

    LogDiagnostic("CommonLibSSE SKSEPluginLoad initialized.");

    auto papyrus = SKSE::GetPapyrusInterface();
    if (papyrus)
    {
        papyrus->Register(CWFrameworkBindings::RegisterPapyrusFunctions);
        LogDiagnostic("CommonLibSSE Papyrus Interface registered successfully.");
    }

    if (!CWFramework::PublicAPI::IsFrameworkReady())
    {
        return false;
    }

    CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();
    std::thread(InputMonitorThread).detach();

    return true;
}
#else
extern "C" __declspec(dllexport) bool __stdcall SKSEPlugin_Load(const void* a_skse)
{
    LogDiagnostic("SKSEPlugin_Load called (Standalone mode).");

    if (!CWFramework::PublicAPI::IsFrameworkReady())
    {
        return false;
    }

    CWFramework::ContentValidationEngine::GetInstance().ValidateRegisteredContentPackData();
    std::thread(InputMonitorThread).detach();

    LogDiagnostic("CWFramework.dll loaded successfully.");
    return true;
}
#endif
