#include "CWFramework/ExpansionManager.h"
#include <mutex>

namespace CWFramework
{
    void ExpansionManager::RegisterCompanionModule(CompanionModuleType type, const std::string& moduleName)
    {
        std::unique_lock lock(m_mutex);
        m_activeCompanionModules[type] = moduleName.empty() ? "OptionalCompanionModule" : moduleName;
    }

    void ExpansionManager::UnregisterCompanionModule(CompanionModuleType type)
    {
        std::unique_lock lock(m_mutex);
        m_activeCompanionModules.erase(type);
    }

    bool ExpansionManager::IsCompanionModuleActive(CompanionModuleType type) const
    {
        std::shared_lock lock(m_mutex);
        return m_activeCompanionModules.find(type) != m_activeCompanionModules.end();
    }

    std::string ExpansionManager::GetCompanionModuleName(CompanionModuleType type) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_activeCompanionModules.find(type);
        if (it != m_activeCompanionModules.end()) return it->second;
        return {};
    }

    bool ExpansionManager::MergeSeriesExpansionData(const SeriesData& seriesData)
    {
        return Registry::GetInstance().RegisterSeries(seriesData);
    }

    bool ExpansionManager::MergeWeaponTypeExpansionData(const WeaponTypeData& weaponTypeData)
    {
        return Registry::GetInstance().RegisterWeaponType(weaponTypeData);
    }

    void ExpansionManager::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_activeCompanionModules.clear();
    }
}
