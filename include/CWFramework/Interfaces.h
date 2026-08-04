#pragma once

#include "DataModels.h"
#include <tuple>
#include <string>

namespace CWFramework
{
    /**
     * @brief Abstract evaluator interface for unlock requirements.
     */
    class IRequirementEvaluator
    {
    public:
        virtual ~IRequirementEvaluator() = default;

        /**
         * @brief Evaluates if a requirement is satisfied.
         * @param requirement Requirement definition.
         * @return true if met, false otherwise.
         */
        [[nodiscard]] virtual bool IsRequirementMet(const RequirementData& requirement) const = 0;

        /**
         * @brief Gets current progress toward requirement for UI.
         * @param requirement Requirement definition.
         * @return Tuple of (currentValue, targetValue).
         */
        [[nodiscard]] virtual std::tuple<double, double> GetRequirementProgress(const RequirementData& requirement) const = 0;
    };

    /**
     * @brief Abstract handler interface for abilities.
     */
    class IAbilityHandler
    {
    public:
        virtual ~IAbilityHandler() = default;

        virtual bool GrantAbility(const AbilityData& ability) = 0;
        virtual bool RevokeAbility(const AbilityData& ability) = 0;
        virtual bool TriggerActiveAbility(const AbilityData& ability) = 0;
    };

    /**
     * @brief Abstract applier interface for stat modifier effects.
     */
    class IEffectApplier
    {
    public:
        virtual ~IEffectApplier() = default;

        virtual bool ApplyEffect(const EffectData& effect) = 0;
        virtual bool RemoveEffect(const EffectData& effect) = 0;
    };

    /**
     * @brief Abstract processor interface for crafting and refinement recipes.
     */
    class IRecipeProcessor
    {
    public:
        virtual ~IRecipeProcessor() = default;

        [[nodiscard]] virtual bool CanCraftRecipe(const RecipeData& recipe) const = 0;
        virtual bool ExecuteRecipe(const RecipeData& recipe) = 0;
    };
}
