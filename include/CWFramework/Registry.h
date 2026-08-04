#pragma once

#include "DataModels.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include <shared_mutex>

namespace CWFramework
{
    /**
     * @brief Central Data Registry managing content pack registrations.
     * Guaranteed clean loading when 0 content packs are registered.
     */
    class Registry
    {
    public:
        static Registry& GetInstance()
        {
            static Registry instance;
            return instance;
        }

        // Registration APIs
        bool RegisterWeaponType(const WeaponTypeData& data);
        bool RegisterSeries(const SeriesData& data);
        bool RegisterWeaponForm(const WeaponFormData& data);
        bool RegisterProgressionNode(const ProgressionNodeData& data);
        bool RegisterRequirement(const RequirementData& data);
        bool RegisterAbility(const AbilityData& data);
        bool RegisterEffect(const EffectData& data);
        bool RegisterRecipe(const RecipeData& data);

        // Query APIs
        [[nodiscard]] std::optional<WeaponTypeData> GetWeaponType(const std::string& id) const;
        [[nodiscard]] std::optional<SeriesData> GetSeries(const std::string& id) const;
        [[nodiscard]] std::optional<WeaponFormData> GetWeaponForm(const std::string& id) const;
        [[nodiscard]] std::optional<ProgressionNodeData> GetProgressionNode(const std::string& id) const;
        [[nodiscard]] std::optional<RequirementData> GetRequirement(const std::string& id) const;
        [[nodiscard]] std::optional<AbilityData> GetAbility(const std::string& id) const;
        [[nodiscard]] std::optional<EffectData> GetEffect(const std::string& id) const;
        [[nodiscard]] std::optional<RecipeData> GetRecipe(const std::string& id) const;

        [[nodiscard]] std::vector<WeaponTypeData> GetAllWeaponTypes() const;
        [[nodiscard]] std::vector<SeriesData> GetSeriesForWeaponType(const std::string& weaponTypeId) const;
        [[nodiscard]] std::vector<ProgressionNodeData> GetNodesForSeries(const std::string& seriesId) const;

        [[nodiscard]] size_t GetWeaponTypeCount() const;
        [[nodiscard]] size_t GetSeriesCount() const;
        [[nodiscard]] size_t GetWeaponFormCount() const;
        [[nodiscard]] size_t GetProgressionNodeCount() const;

        // Reset / Clear
        void Clear();

    private:
        Registry() = default;
        ~Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        mutable std::shared_mutex m_mutex;

        std::unordered_map<std::string, WeaponTypeData> m_weaponTypes;
        std::unordered_map<std::string, SeriesData> m_seriesMap;
        std::unordered_map<std::string, WeaponFormData> m_weaponForms;
        std::unordered_map<std::string, ProgressionNodeData> m_nodes;
        std::unordered_map<std::string, RequirementData> m_requirements;
        std::unordered_map<std::string, AbilityData> m_abilities;
        std::unordered_map<std::string, EffectData> m_effects;
        std::unordered_map<std::string, RecipeData> m_recipes;
    };
}
