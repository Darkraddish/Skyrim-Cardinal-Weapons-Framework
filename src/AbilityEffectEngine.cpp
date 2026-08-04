#include "CWFramework/AbilityEffectEngine.h"
#include <mutex>

namespace CWFramework
{
    bool AbilityEffectEngine::GrantAbility(const AbilityData& ability)
    {
        if (ability.id.empty()) return false;
        std::unique_lock lock(m_mutex);

        if (ability.IsFormBound())
        {
            m_activeFormBoundAbilities[ability.id] = true;
        }
        else if (ability.IsPermanent())
        {
            m_permanentAbilities[ability.id] = true;
        }
        return true;
    }

    bool AbilityEffectEngine::RevokeAbility(const AbilityData& ability)
    {
        if (ability.id.empty()) return false;
        std::unique_lock lock(m_mutex);

        if (ability.IsFormBound())
        {
            m_activeFormBoundAbilities.erase(ability.id);
        }
        else if (ability.IsPermanent())
        {
            m_permanentAbilities.erase(ability.id);
        }
        return true;
    }

    bool AbilityEffectEngine::TriggerActiveAbility(const AbilityData& ability)
    {
        if (ability.id.empty() || ability.scope != AbilityScope::Active) return false;
        std::shared_lock lock(m_mutex);

        // Check if form-bound active ability is currently active
        auto it = m_activeFormBoundAbilities.find(ability.id);
        if (it != m_activeFormBoundAbilities.end() && it->second)
        {
            // Successfully triggered active skill
            return true;
        }
        return false;
    }

    bool AbilityEffectEngine::ApplyEffect(const EffectData& effect)
    {
        if (effect.id.empty()) return false;
        std::unique_lock lock(m_mutex);

        // Can apply both positive boosts and negative drawbacks simultaneously!
        m_appliedEffects[effect.id] = effect;
        return true;
    }

    bool AbilityEffectEngine::RemoveEffect(const EffectData& effect)
    {
        if (effect.id.empty()) return false;
        std::unique_lock lock(m_mutex);

        m_appliedEffects.erase(effect.id);
        return true;
    }

    void AbilityEffectEngine::OnFormSwapped(const std::string& oldFormId, const std::string& newFormId)
    {
        std::unique_lock lock(m_mutex);

        // Revoke form-bound skills from old form
        if (!oldFormId.empty())
        {
            auto oldFormOpt = Registry::GetInstance().GetWeaponForm(oldFormId);
            if (oldFormOpt.has_value())
            {
                for (const auto& abiId : oldFormOpt->abilityIds)
                {
                    m_activeFormBoundAbilities.erase(abiId);
                }
            }
        }

        // Grant form-bound skills for new form
        if (!newFormId.empty())
        {
            auto newFormOpt = Registry::GetInstance().GetWeaponForm(newFormId);
            if (newFormOpt.has_value())
            {
                for (const auto& abiId : newFormOpt->abilityIds)
                {
                    auto abiOpt = Registry::GetInstance().GetAbility(abiId);
                    if (abiOpt.has_value() && abiOpt->IsFormBound())
                    {
                        m_activeFormBoundAbilities[abiId] = true;
                    }
                }
            }
        }
    }

    void AbilityEffectEngine::OnNodeUnlocked(const std::string& nodeId)
    {
        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (!nodeOpt.has_value()) return;

        const auto& nodeData = nodeOpt.value();

        // Grant abilities attached to node
        for (const auto& abiId : nodeData.abilityIds)
        {
            auto abiOpt = Registry::GetInstance().GetAbility(abiId);
            if (abiOpt.has_value())
            {
                GrantAbility(abiOpt.value());
            }
        }

        // Apply effects attached to node
        for (const auto& effId : nodeData.effectIds)
        {
            auto effOpt = Registry::GetInstance().GetEffect(effId);
            if (effOpt.has_value())
            {
                ApplyEffect(effOpt.value());
            }
        }
    }

    std::vector<std::string> AbilityEffectEngine::GetActiveFormBoundAbilityIds() const
    {
        std::shared_lock lock(m_mutex);
        std::vector<std::string> result;
        for (const auto& [id, active] : m_activeFormBoundAbilities)
        {
            if (active) result.push_back(id);
        }
        return result;
    }

    std::vector<std::string> AbilityEffectEngine::GetUnlockedPermanentAbilityIds() const
    {
        std::shared_lock lock(m_mutex);
        std::vector<std::string> result;
        for (const auto& [id, active] : m_permanentAbilities)
        {
            if (active) result.push_back(id);
        }
        return result;
    }

    std::unordered_map<std::string, double> AbilityEffectEngine::CalculateNetActorValueModifiers() const
    {
        std::shared_lock lock(m_mutex);
        std::unordered_map<std::string, double> netModifiers;

        for (const auto& [id, effect] : m_appliedEffects)
        {
            // Aggregates positive boosts and negative drawbacks per stat
            netModifiers[effect.targetActorValue] += effect.value;
        }

        return netModifiers;
    }

    void AbilityEffectEngine::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_activeFormBoundAbilities.clear();
        m_permanentAbilities.clear();
        m_appliedEffects.clear();
    }
}
