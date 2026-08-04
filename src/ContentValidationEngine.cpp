#include "CWFramework/ContentValidationEngine.h"
#include <mutex>

namespace CWFramework
{
    ValidationResult ContentValidationEngine::ValidateRegisteredContentPackData() const
    {
        ValidationResult result;

        // Verify all registered WeaponTypes
        auto types = Registry::GetInstance().GetAllWeaponTypes();
        for (const auto& wt : types)
        {
            auto valRes = SchemaValidator::ValidateWeaponType(wt);
            if (!valRes.isValid)
            {
                for (const auto& err : valRes.errors) result.AddError(err);
            }
        }

        return result;
    }

    ValidationResult ContentValidationEngine::VerifyNodePreUnlock(const std::string& nodeId) const
    {
        ValidationResult result;
        if (!IsItemAvailable(nodeId))
        {
            result.AddError("Node '" + nodeId + "' marked unavailable: " + GetUnavailableReason(nodeId));
            return result;
        }

        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (!nodeOpt.has_value())
        {
            result.AddError("Node '" + nodeId + "' does not exist in registry.");
            return result;
        }

        return result;
    }

    ValidationResult ContentValidationEngine::VerifyWeaponChoicePreSelection(const std::string& weaponTypeId) const
    {
        ValidationResult result;
        if (!IsItemAvailable(weaponTypeId))
        {
            result.AddError("WeaponType '" + weaponTypeId + "' marked unavailable.");
            return result;
        }

        auto wtOpt = Registry::GetInstance().GetWeaponType(weaponTypeId);
        if (!wtOpt.has_value())
        {
            result.AddError("WeaponType '" + weaponTypeId + "' missing from registry.");
            return result;
        }

        return result;
    }

    void ContentValidationEngine::MarkItemUnavailable(const std::string& itemId, const std::string& reason)
    {
        if (itemId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_unavailableItems[itemId] = reason.empty() ? "Content missing or corrupt" : reason;
    }

    bool ContentValidationEngine::IsItemAvailable(const std::string& itemId) const
    {
        std::shared_lock lock(m_mutex);
        return m_unavailableItems.find(itemId) == m_unavailableItems.end();
    }

    std::string ContentValidationEngine::GetUnavailableReason(const std::string& itemId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_unavailableItems.find(itemId);
        if (it != m_unavailableItems.end()) return it->second;
        return {};
    }

    void ContentValidationEngine::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_unavailableItems.clear();
    }
}
