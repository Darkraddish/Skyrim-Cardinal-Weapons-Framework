#pragma once

#include "DataModels.h"
#include <string>
#include <vector>

namespace CWFramework
{
    struct ValidationResult
    {
        bool isValid{ true };
        std::vector<std::string> errors;

        void AddError(std::string err)
        {
            isValid = false;
            errors.push_back(std::move(err));
        }
    };

    class SchemaValidator
    {
    public:
        static ValidationResult ValidateWeaponType(const WeaponTypeData& data);
        static ValidationResult ValidateSeries(const SeriesData& data);
        static ValidationResult ValidateWeaponForm(const WeaponFormData& data);
        static ValidationResult ValidateProgressionNode(const ProgressionNodeData& data);
        static ValidationResult ValidateRequirement(const RequirementData& data);
        static ValidationResult ValidateAbility(const AbilityData& data);
        static ValidationResult ValidateEffect(const EffectData& data);
        static ValidationResult ValidateRecipe(const RecipeData& data);
    };
}
