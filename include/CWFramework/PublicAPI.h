#pragma once

#include "Registry.h"
#include <string>
#include <vector>

namespace CWFramework
{
    class PublicAPI
    {
    public:
        // Returns framework version string
        static std::string GetVersion() noexcept
        {
            return "1.0.0-Phase1";
        }

        // Framework status check
        static bool IsFrameworkReady() noexcept
        {
            return true;
        }

        // Zero-content load status check
        static bool IsZeroContentLoaded() noexcept
        {
            return Registry::GetInstance().GetWeaponTypeCount() == 0;
        }

        // Registration shortcuts
        static bool RegisterWeaponType(const WeaponTypeData& data)
        {
            return Registry::GetInstance().RegisterWeaponType(data);
        }

        static bool RegisterSeries(const SeriesData& data)
        {
            return Registry::GetInstance().RegisterSeries(data);
        }

        static bool RegisterWeaponForm(const WeaponFormData& data)
        {
            return Registry::GetInstance().RegisterWeaponForm(data);
        }

        static bool RegisterProgressionNode(const ProgressionNodeData& data)
        {
            return Registry::GetInstance().RegisterProgressionNode(data);
        }

        static bool RegisterRequirement(const RequirementData& data)
        {
            return Registry::GetInstance().RegisterRequirement(data);
        }

        static bool RegisterAbility(const AbilityData& data)
        {
            return Registry::GetInstance().RegisterAbility(data);
        }

        static bool RegisterEffect(const EffectData& data)
        {
            return Registry::GetInstance().RegisterEffect(data);
        }

        static bool RegisterRecipe(const RecipeData& data)
        {
            return Registry::GetInstance().RegisterRecipe(data);
        }

        // Query shortcuts
        static size_t GetRegisteredWeaponTypeCount() noexcept
        {
            return Registry::GetInstance().GetWeaponTypeCount();
        }

        static std::vector<WeaponTypeData> GetAllWeaponTypes()
        {
            return Registry::GetInstance().GetAllWeaponTypes();
        }
    };
}
