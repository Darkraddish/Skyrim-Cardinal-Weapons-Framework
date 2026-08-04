#pragma once

#include "Registry.h"
#include "SaveManager.h"
#include "LegendaryWeaponManager.h"
#include "RequirementEvaluator.h"
#include <string>
#include <vector>

namespace CWFramework
{
    class PublicAPI
    {
    public:
        // Returns framework version string
        static std::string GetVersion() noexcept
        {
            return "1.0.0-Phase1";
        }

        // Framework status check
        static bool IsFrameworkReady() noexcept
        {
            return true;
        }

        // Zero-content load status check
        static bool IsZeroContentLoaded() noexcept
        {
            return Registry::GetInstance().GetWeaponTypeCount() == 0;
        }

        // Registration shortcuts
        static bool RegisterWeaponType(const WeaponTypeData& data)
        {
            return Registry::GetInstance().RegisterWeaponType(data);
        }

        static bool RegisterSeries(const SeriesData& data)
        {
            return Registry::GetInstance().RegisterSeries(data);
        }

        static bool RegisterWeaponForm(const WeaponFormData& data)
        {
            return Registry::GetInstance().RegisterWeaponForm(data);
        }

        static bool RegisterProgressionNode(const ProgressionNodeData& data)
        {
            return Registry::GetInstance().RegisterProgressionNode(data);
        }

        static bool RegisterRequirement(const RequirementData& data)
        {
            return Registry::GetInstance().RegisterRequirement(data);
        }

        static bool RegisterAbility(const AbilityData& data)
        {
            return Registry::GetInstance().RegisterAbility(data);
        }

        static bool RegisterEffect(const EffectData& data)
        {
            return Registry::GetInstance().RegisterEffect(data);
        }

        static bool RegisterRecipe(const RecipeData& data)
        {
            return Registry::GetInstance().RegisterRecipe(data);
        }

        // Query shortcuts
        static size_t GetRegisteredWeaponTypeCount() noexcept
        {
            return Registry::GetInstance().GetWeaponTypeCount();
        }

        static std::vector<WeaponTypeData> GetAllWeaponTypes()
        {
            return Registry::GetInstance().GetAllWeaponTypes();
        }

        // Save & Persistence System Shortcuts
        static bool LockInWeaponChoice(const std::string& weaponTypeId)
        {
            return SaveManager::GetInstance().LockInWeaponChoice(weaponTypeId);
        }

        static bool IsWeaponLocked()
        {
            return SaveManager::GetInstance().IsWeaponLocked();
        }

        static std::string GetChosenWeaponTypeId()
        {
            return SaveManager::GetInstance().GetChosenWeaponTypeId();
        }

        static bool SetCurrentForm(const std::string& formId)
        {
            return SaveManager::GetInstance().SetCurrentForm(formId);
        }

        static std::string GetCurrentFormId()
        {
            return SaveManager::GetInstance().GetCurrentFormId();
        }

        static bool UnlockNode(const std::string& nodeId)
        {
            return SaveManager::GetInstance().UnlockNode(nodeId);
        }

        static bool IsNodeUnlocked(const std::string& nodeId)
        {
            return SaveManager::GetInstance().IsNodeUnlocked(nodeId);
        }

        // Legendary Weapon Lifecycle & Management Shortcuts
        static bool InitiateWeaponSelection(const std::string& weaponTypeId)
        {
            return LegendaryWeaponManager::GetInstance().InitiateWeaponSelection(weaponTypeId);
        }

        static bool ConfirmWeaponChoice(const std::string& weaponTypeId)
        {
            return LegendaryWeaponManager::GetInstance().ConfirmWeaponChoice(weaponTypeId);
        }

        static bool SwapActiveForm(const std::string& newFormId)
        {
            return LegendaryWeaponManager::GetInstance().SwapActiveForm(newFormId);
        }

        static bool OnWeaponEquipped(const std::string& itemFormId)
        {
            return LegendaryWeaponManager::GetInstance().OnWeaponEquipped(itemFormId);
        }

        static void OnWeaponUnequipped(const std::string& itemFormId)
        {
            LegendaryWeaponManager::GetInstance().OnWeaponUnequipped(itemFormId);
        }

        // Requirement Engine Shortcuts
        static bool CanUnlockNode(const ProgressionNodeData& node)
        {
            return RequirementEvaluator::GetInstance().CanUnlockNode(node);
        }

        static bool CanUnlockNodeById(const std::string& nodeId)
        {
            return RequirementEvaluator::GetInstance().CanUnlockNodeById(nodeId);
        }

        static std::tuple<double, double> GetRequirementProgress(const RequirementData& requirement)
        {
            return RequirementEvaluator::GetInstance().GetRequirementProgress(requirement);
        }

        static void RecordEnemyKill(const std::string& actorBaseId, uint32_t count = 1)
        {
            RequirementEvaluator::GetInstance().RecordKill(actorBaseId, count);
        }

        static void RecordItemAbsorption(const std::string& itemFormId, uint32_t count = 1)
        {
            RequirementEvaluator::GetInstance().RecordItemAbsorption(itemFormId, count);
        }
    };
}
