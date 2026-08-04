#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SaveManager.h"
#include <shared_mutex>
#include <string>
#include <vector>

namespace CWFramework
{
    /**
     * @brief Legendary Weapon Manager.
     * Manages active weapon selection flow, active form swaps, and equip/unequip events.
     */
    class LegendaryWeaponManager
    {
    public:
        static LegendaryWeaponManager& GetInstance()
        {
            static LegendaryWeaponManager instance;
            return instance;
        }

        // Weapon Selection Flow
        bool InitiateWeaponSelection(const std::string& weaponTypeId);
        bool ConfirmWeaponChoice(const std::string& weaponTypeId);
        [[nodiscard]] std::string GetPendingWeaponChoice() const;
        void CancelWeaponSelection();

        // Form Swaps & Active Form Management
        bool SwapActiveForm(const std::string& newFormId);
        [[nodiscard]] std::string GetActiveFormId() const;

        // Equip / Unequip Event Listeners & Multi-Weapon Prevention
        bool OnWeaponEquipped(const std::string& itemFormId);
        void OnWeaponUnequipped(const std::string& itemFormId);

        [[nodiscard]] bool IsCardinalWeaponEquipped() const;
        [[nodiscard]] std::string GetEquippedCardinalWeaponItemFormId() const;

        // Helper check
        [[nodiscard]] bool IsRegisteredCardinalWeaponItem(const std::string& itemFormId) const;

        // Reset state (for save load / testing)
        void ResetState();

    private:
        LegendaryWeaponManager() = default;
        ~LegendaryWeaponManager() = default;
        LegendaryWeaponManager(const LegendaryWeaponManager&) = delete;
        LegendaryWeaponManager& operator=(const LegendaryWeaponManager&) = delete;

        mutable std::shared_mutex m_mutex;
        std::string m_pendingWeaponChoiceId;
        std::string m_equippedCardinalWeaponItemFormId;
        bool m_isCardinalWeaponEquipped{ false };
    };
}
