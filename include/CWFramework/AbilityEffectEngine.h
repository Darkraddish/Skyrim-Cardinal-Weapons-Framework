#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SaveManager.h"
#include "Interfaces.h"
#include <shared_mutex>
#include <string>
#include <vector>
#include <unordered_map>

namespace CWFramework
{
    /**
     * @brief Ability & Effect Engine.
     * Manages form-bound skills (Equip/Active) vs permanent skills (Passive/Master) and stat modifiers (positive boosts & negative drawbacks).
     */
    class AbilityEffectEngine : public IAbilityHandler, public IEffectApplier
    {
    public:
        static AbilityEffectEngine& GetInstance()
        {
            static AbilityEffectEngine instance;
            return instance;
        }

        // IAbilityHandler interface
        bool GrantAbility(const AbilityData& ability) override;
        bool RevokeAbility(const AbilityData& ability) override;
        bool TriggerActiveAbility(const AbilityData& ability) override;

        // IEffectApplier interface
        bool ApplyEffect(const EffectData& effect) override;
        bool RemoveEffect(const EffectData& effect) override;

        // Lifecycle Events: Form changes & Node Unlocks
        void OnFormSwapped(const std::string& oldFormId, const std::string& newFormId);
        void OnNodeUnlocked(const std::string& nodeId);

        // State Queries
        [[nodiscard]] std::vector<std::string> GetActiveFormBoundAbilityIds() const;
        [[nodiscard]] std::vector<std::string> GetUnlockedPermanentAbilityIds() const;
        [[nodiscard]] std::unordered_map<std::string, double> CalculateNetActorValueModifiers() const;

        // Reset state
        void ResetState();

    private:
        AbilityEffectEngine() = default;
        ~AbilityEffectEngine() override = default;
        AbilityEffectEngine(const AbilityEffectEngine&) = delete;
        AbilityEffectEngine& operator=(const AbilityEffectEngine&) = delete;

        mutable std::shared_mutex m_mutex;

        std::unordered_map<std::string, bool> m_activeFormBoundAbilities;
        std::unordered_map<std::string, bool> m_permanentAbilities;
        std::unordered_map<std::string, EffectData> m_appliedEffects;
    };
}
