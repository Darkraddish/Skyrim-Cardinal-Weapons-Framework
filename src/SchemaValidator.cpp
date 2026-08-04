#include "CWFramework/SchemaValidator.h"

namespace CWFramework
{
    ValidationResult SchemaValidator::ValidateWeaponType(const WeaponTypeData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("WeaponType ID cannot be empty.");
        if (data.name.empty()) res.AddError("WeaponType Name cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateSeries(const SeriesData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("Series ID cannot be empty.");
        if (data.weaponTypeId.empty()) res.AddError("Series weaponTypeId cannot be empty.");
        if (data.name.empty()) res.AddError("Series Name cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateWeaponForm(const WeaponFormData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("WeaponForm ID cannot be empty.");
        if (data.seriesId.empty()) res.AddError("WeaponForm seriesId cannot be empty.");
        if (data.name.empty()) res.AddError("WeaponForm Name cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateProgressionNode(const ProgressionNodeData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("ProgressionNode ID cannot be empty.");
        if (data.seriesId.empty()) res.AddError("ProgressionNode seriesId cannot be empty.");
        if (data.name.empty()) res.AddError("ProgressionNode Name cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateRequirement(const RequirementData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("Requirement ID cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateAbility(const AbilityData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("Ability ID cannot be empty.");
        if (data.name.empty()) res.AddError("Ability Name cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateEffect(const EffectData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("Effect ID cannot be empty.");
        if (data.name.empty()) res.AddError("Effect Name cannot be empty.");
        if (data.targetActorValue.empty()) res.AddError("Effect targetActorValue cannot be empty.");
        return res;
    }

    ValidationResult SchemaValidator::ValidateRecipe(const RecipeData& data)
    {
        ValidationResult res;
        if (data.id.empty()) res.AddError("Recipe ID cannot be empty.");
        if (data.name.empty()) res.AddError("Recipe Name cannot be empty.");
        if (data.ingredients.empty()) res.AddError("Recipe ingredients cannot be empty.");
        return res;
    }
}
