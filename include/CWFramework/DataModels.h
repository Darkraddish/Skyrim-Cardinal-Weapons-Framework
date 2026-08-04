#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace CWFramework
{
    // High-level enums for the framework
    enum class NodeType : uint8_t
    {
        Form = 0,
        PassiveUpgrade,
        AbilityUnlock,
        Transformation,
        RefinementMilestone,
        EventBranch
    };

    enum class RequirementType : uint8_t
    {
        KillCount = 0,
        ItemAbsorption,
        WeaponCopy,
        QuestState,
        PlayerLevel,
        ParentNodeDependency,
        CraftingRequirement,
        LocationRequirement,
        SpecialEventTrigger,
        CustomScript
    };

    enum class AbilityScope : uint8_t
    {
        Equip = 0, // Form-bound: active while form is equipped
        Active,    // Form-bound: manually triggered skill while form is equipped
        Passive,   // Permanent: active across all forms once unlocked
        Master     // Permanent: pinnacle skill, active across all forms
    };

    enum class ModifierType : uint8_t
    {
        FlatAdditive = 0,
        PercentageMultiplier
    };

    // Data Structures
    struct RequirementData
    {
        std::string id;
        RequirementType type{ RequirementType::KillCount };
        std::string description;
        std::string targetId;
        double targetValue{ 0.0 };
        std::string scriptName;
    };

    struct AbilityData
    {
        std::string id;
        std::string name;
        std::string description;
        AbilityScope scope{ AbilityScope::Equip };
        std::string spellFormId;
        std::string perkFormId;
        double cooldown{ 0.0 };
        double magickaCost{ 0.0 };
        double staminaCost{ 0.0 };

        [[nodiscard]] bool IsFormBound() const noexcept
        {
            return scope == AbilityScope::Equip || scope == AbilityScope::Active;
        }

        [[nodiscard]] bool IsPermanent() const noexcept
        {
            return scope == AbilityScope::Passive || scope == AbilityScope::Master;
        }
    };

    struct EffectData
    {
        std::string id;
        std::string name;
        std::string targetActorValue;
        ModifierType modifierType{ ModifierType::FlatAdditive };
        double value{ 0.0 };
    };

    struct RecipeIngredient
    {
        std::string itemId;
        uint32_t count{ 1 };
    };

    struct RecipeData
    {
        std::string id;
        std::string name;
        std::vector<RecipeIngredient> ingredients;
        std::string outputFormId;
        std::string requiredStation;
    };

    struct WeaponFormData
    {
        std::string id;
        std::string seriesId;
        std::string name;
        std::string description;
        std::string modelAsset;
        std::string iconAsset;
        double baseDamage{ 0.0 };
        double baseArmor{ 0.0 };
        double baseWeight{ 0.0 };
        uint32_t value{ 0 };
        std::vector<std::string> abilityIds;
    };

    struct ProgressionNodeData
    {
        std::string id;
        std::string seriesId;
        std::string name;
        std::string description;
        NodeType nodeType{ NodeType::Form };
        double posX{ 0.0 };
        double posY{ 0.0 };
        std::vector<std::string> parentNodeIds;
        std::string unlockedFormId;
        bool isConvertible{ false };
        std::vector<std::string> requirementIds;
        std::vector<std::string> abilityIds;
        std::vector<std::string> effectIds;
        std::vector<std::string> recipeIds;
        std::vector<std::string> revealedBranchSeriesIds;
    };

    struct SeriesData
    {
        std::string id;
        std::string weaponTypeId;
        std::string name;
        std::string description;
        std::string iconAsset;
        std::string rootNodeId;
        std::vector<std::string> nodeIds;
        std::vector<std::string> formIds;
    };

    struct WeaponTypeData
    {
        std::string id;
        std::string name;
        std::string description;
        std::string iconAsset;
        std::vector<std::string> baseSeriesIds;
    };
}
