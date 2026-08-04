#pragma once

#include "DataModels.h"
#include "Registry.h"
#include <shared_mutex>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace CWFramework
{
    enum class CompanionModuleType : uint8_t
    {
        Economy = 0,
        Survival,
        Leveling,
        Animation
    };

    /**
     * @brief Expansion Support & Companion Module Hooks Manager.
     * Handles dynamic series/weapon expansion data merging and optional companion module hooks.
     */
    class ExpansionManager
    {
    public:
        static ExpansionManager& GetInstance()
        {
            static ExpansionManager instance;
            return instance;
        }

        // Companion Module Hook Management
        void RegisterCompanionModule(CompanionModuleType type, const std::string& moduleName);
        void UnregisterCompanionModule(CompanionModuleType type);
        [[nodiscard]] bool IsCompanionModuleActive(CompanionModuleType type) const;
        [[nodiscard]] std::string GetCompanionModuleName(CompanionModuleType type) const;

        // Series & Weapon Type Data Merger APIs
        bool MergeSeriesExpansionData(const SeriesData& seriesData);
        bool MergeWeaponTypeExpansionData(const WeaponTypeData& weaponTypeData);

        // Reset state
        void ResetState();

    private:
        ExpansionManager() = default;
        ~ExpansionManager() = default;
        ExpansionManager(const ExpansionManager&) = delete;
        ExpansionManager& operator=(const ExpansionManager&) = delete;

        mutable std::shared_mutex m_mutex;
        std::unordered_map<CompanionModuleType, std::string> m_activeCompanionModules;
    };
}
