#include "CWFramework/LegendaryWeaponManager.h"
#include <mutex>

namespace CWFramework
{
    bool LegendaryWeaponManager::InitiateWeaponSelection(const std::string& weaponTypeId)
    {
        if (weaponTypeId.empty()) return false;
        
        // Cannot initiate selection if player has ALREADY locked in a weapon!
        if (SaveManager::GetInstance().IsWeaponLocked())
        {
            return false;
        }

        // Verify weapon type exists in Registry
        auto wtOpt = Registry::GetInstance().GetWeaponType(weaponTypeId);
        if (!wtOpt.has_value())
        {
            return false;
        }

        std::unique_lock lock(m_mutex);
        m_pendingWeaponChoiceId = weaponTypeId;
        return true;
    }

    bool LegendaryWeaponManager::ConfirmWeaponChoice(const std::string& weaponTypeId)
    {
        std::unique_lock lock(m_mutex);

        // Verify pending choice matches
        if (m_pendingWeaponChoiceId.empty() || m_pendingWeaponChoiceId != weaponTypeId)
        {
            return false;
        }

        // Confirm lock in SaveManager
        bool lockSuccess = SaveManager::GetInstance().LockInWeaponChoice(weaponTypeId);
        if (lockSuccess)
        {
            m_pendingWeaponChoiceId.clear();
            return true;
        }
        return false;
    }

    std::string LegendaryWeaponManager::GetPendingWeaponChoice() const
    {
        std::shared_lock lock(m_mutex);
        return m_pendingWeaponChoiceId;
    }

    void LegendaryWeaponManager::CancelWeaponSelection()
    {
        std::unique_lock lock(m_mutex);
        m_pendingWeaponChoiceId.clear();
    }

    bool LegendaryWeaponManager::SwapActiveForm(const std::string& newFormId)
    {
        if (newFormId.empty()) return false;

        // Verify form exists in Registry
        auto formOpt = Registry::GetInstance().GetWeaponForm(newFormId);
        if (!formOpt.has_value())
        {
            return false;
        }

        // Update current form in SaveManager
        SaveManager::GetInstance().SetCurrentForm(newFormId);
        return true;
    }

    std::string LegendaryWeaponManager::GetActiveFormId() const
    {
        return SaveManager::GetInstance().GetCurrentFormId();
    }

    bool LegendaryWeaponManager::OnWeaponEquipped(const std::string& itemFormId)
    {
        if (itemFormId.empty()) return false;

        // Check if item is a cardinal weapon
        if (IsRegisteredCardinalWeaponItem(itemFormId))
        {
            std::unique_lock lock(m_mutex);

            // MULTI-WEAPON PREVENTION: If player ALREADY has a cardinal weapon equipped, REJECT second equip!
            if (m_isCardinalWeaponEquipped && m_equippedCardinalWeaponItemFormId != itemFormId)
            {
                // Reject equipping multiple cardinal weapons simultaneously
                return false;
            }

            m_isCardinalWeaponEquipped = true;
            m_equippedCardinalWeaponItemFormId = itemFormId;
            return true;
        }

        return true; // Non-cardinal weapon items equip normally
    }

    void LegendaryWeaponManager::OnWeaponUnequipped(const std::string& itemFormId)
    {
        std::unique_lock lock(m_mutex);
        if (m_equippedCardinalWeaponItemFormId == itemFormId)
        {
            m_isCardinalWeaponEquipped = false;
            m_equippedCardinalWeaponItemFormId.clear();
        }
    }

    bool LegendaryWeaponManager::IsCardinalWeaponEquipped() const
    {
        std::shared_lock lock(m_mutex);
        return m_isCardinalWeaponEquipped;
    }

    std::string LegendaryWeaponManager::GetEquippedCardinalWeaponItemFormId() const
    {
        std::shared_lock lock(m_mutex);
        return m_equippedCardinalWeaponItemFormId;
    }

    bool LegendaryWeaponManager::IsRegisteredCardinalWeaponItem(const std::string& itemFormId) const
    {
        if (itemFormId.empty()) return false;
        // Check if item matches any registered weapon form ID or model asset
        auto formOpt = Registry::GetInstance().GetWeaponForm(itemFormId);
        return formOpt.has_value();
    }

    void LegendaryWeaponManager::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_pendingWeaponChoiceId.clear();
        m_equippedCardinalWeaponItemFormId.clear();
        m_isCardinalWeaponEquipped = false;
    }
}
